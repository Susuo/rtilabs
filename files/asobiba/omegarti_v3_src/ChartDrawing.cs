/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Drawing;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

using Travis.Util;
using Zanetti;
using Zanetti.Data;
using Zanetti.Indicators;
using Zanetti.Arithmetic;
using Zanetti.Arithmetic.Series;
#if DOJIMA
using DojimaChart = Zanetti.Dojima.DojimaChart;
#endif

namespace Zanetti.UI
{
	internal class RedrawValue {
		public int _lastDrawn;
		public int _nextToBeDrawn;
	}

	/// <summary>
	/// ChartDrawing �̊T�v�̐����ł��B
	/// </summary>
	internal class ChartDrawing
	{
		
		private ChartCanvas _owner;
		private Preference _pref; //�`��̂��낢��ȉߒ��ŎQ�Ƃ���̂Ń����o�Ƃ��ĕێ�
		private LayoutInfo _layout;
		private int _maximumValueWindowItemCount;

		private AbstractBrand _brand;
		private int _firstDateIndex;
		private int _firstDate;
		private int _lastDate;
		private double[] _relativisedMultipliers;

        //hacked by rti.
        //�������������Ƃ���ɔ��������Ɣ��������̃��C����ǉ�����.
        private RedrawValue _AutoTradeStartDateLine;
        private RedrawValue _AutoTradeEndDateLine;
        //--------------------

		private RedrawValue _dateLine;
		private RedrawValue _priceLine;

        private bool _scaleIsInvalidated;
		private Trans _priceTrans; //price->y���W�ϊ�
        private double _highPrice; //�\�����Ă���l�i�͈̔�
        private double _lowPrice;
        private int _priceScaleStep;  //�l�i�����������̕� ����̐����{���Ƃɐ�������

		private Trans _volumeTrans; //volume->y���W�ϊ�
		private double _volumeScaleStep;
		
		private Win32.POINT _dummyPoint;

		//�L���v�V�������������Č����Ȃ�����E�����߂̃f�[�^
		private ArrayList _tipEntries;
		private bool _tipBuilding;

        //���i�яo����
        private AccumulativeVolume _accumulativeVolume;

        //FreeLine
		private ArrayList _freeLines;

		public ChartDrawing(ChartCanvas owner) {
			_owner = owner;

			_dateLine = new RedrawValue();
			_dateLine._lastDrawn = -1;
			_dateLine._nextToBeDrawn = -1;
			_priceLine = new RedrawValue();

			_freeLines = new ArrayList();
            _scaleIsInvalidated = true;
            _accumulativeVolume = new AccumulativeVolume();

            //hacked by rti.
            //�������������Ƃ���ɔ��������Ɣ��������̃��C��������������
            _AutoTradeStartDateLine = new RedrawValue();
            _AutoTradeStartDateLine._lastDrawn = -1;
            _AutoTradeStartDateLine._nextToBeDrawn = -1;
            _AutoTradeEndDateLine = new RedrawValue();
            _AutoTradeEndDateLine._lastDrawn = -1;
            _AutoTradeEndDateLine._nextToBeDrawn = -1;
            //--------------------
        
        }

		public void SetBrand(AbstractBrand br) {
			_brand = br;
#if DOJIMA
			_halfDailyDataFarm = null;
#endif
			//����������邲�ƂɃN���A
			ClearScale();
			_relativisedMultipliers = new double[Env.CurrentIndicators.IndicatorCount];
		}

		public void ClearScale() {
            if(!Env.Preference.ScaleLock) {
                _priceTrans = null;
                _freeLines.Clear();
            }
            _scaleIsInvalidated = true;
            _tipEntries = null;
        }
		public ArrayList TipEntries {
			get {
				return _tipEntries;
			}
		}
		public int FirstDateIndex {
			get {
				return _firstDateIndex;
			}
			set {
				_firstDateIndex = value;
			}
		}
		public RedrawValue DateLine {
			get {
				return _dateLine;
			}
		}
		public RedrawValue PriceLine {
			get {
				return _priceLine;
			}
			set {
				_priceLine = value;
			}
		}

        //���ɕ`�悷����̂����߂�
        public void UpdateDateLineIndex(int index) {
            if(_dateLine._nextToBeDrawn != index) {
                if(index != -1) {
                    DataFarm farm = _owner.GetBrand().ReserveFarm();
					if(farm.IsEmpty || _scaleIsInvalidated)
						_accumulativeVolume.Available = false;
					else {
						int price_pitch = Math.Max(1, _priceScaleStep / AccumulativeVolume.DATA_PER_SCALELINE);
						_accumulativeVolume.Fill(farm, index, _accumulativeVolume.StartPrice, price_pitch, _accumulativeVolume.EndPrice);
					}
                }
            }
            _dateLine._nextToBeDrawn = index;
        }

        //hacked by rti
        //�������������Ƃ��̊J�n�ƏI���Ƀ��C��������
        //�����܂ŃR�s�y�ɂȂ�ƁA�I���W�i���̃N���X�̃��\�b�h�ɂ����������B�B�B
        public void UpdateDateAutoTradingLineIndex(int startIndex , int endIndex)
        {
            _AutoTradeStartDateLine._nextToBeDrawn = startIndex;
            _AutoTradeEndDateLine._nextToBeDrawn = endIndex;
        }
        //---------------------

        //���݂̐ݒ�ŕ\���ł�����t�͈�
		public int FirstDate {
			get {
				DataFarm farm = _owner.GetBrand().ReserveFarm();
				return farm.GetByIndex(_firstDateIndex).Date;
			}
		}
		public int LastDate {
			get {
				DataFarm farm = _owner.GetBrand().ReserveFarm();
				return farm.GetByIndex(Math.Min(farm.FilledLength, _firstDateIndex+Env.Layout.DisplayColumnCount)-1).Date;
			}
		}


		public int MaximumValueWindowItemCount {
			get {
				return _maximumValueWindowItemCount;
			}
		}
		public int FreeLineCount {
			get {
				return _freeLines.Count;
			}
		}
		public IEnumerable FreeLines {
			get {
				return _freeLines;
			}
		}


		public void PaintMain(Graphics g, Rectangle clip) {
			if(_brand==null) return;
			if(!Env.CurrentIndicators.Valid) return;

			_pref = Env.Preference;
			_layout = Env.Layout;
			IntPtr hdc = g.GetHdc();
			Win32.SetBkColor(hdc, Util.ToCOLORREF(_pref.BackBrush.Color));
			Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));
			Win32.SelectObject(hdc, _pref.DefaultHFont);

			if(_tipEntries==null) {
				_tipEntries = new ArrayList();
				_tipBuilding = true;
			}
			else
				_tipBuilding = false;

			try {
				if(clip.IntersectsWith(_layout.ChartBodyRect))
					DrawChartBody(hdc, clip);
				if(_pref.ShowPrice) {
					if(clip.IntersectsWith(_layout.CurrentValueRect))
						DrawCurrentValue(hdc);
					if(clip.IntersectsWith(_layout.ExplanationRect))
						DrawExplanation(hdc);
				}
                if(_pref.ShowAccumulativeVolume && clip.IntersectsWith(_layout.AccumulativeVolumeRect))
                    DrawAccumulativeVolume(hdc);
                foreach(FreeLine fl in _freeLines) {
					if(fl.GetInclusion(_owner.ClientRectangle).IntersectsWith(clip))
						fl.Draw(_layout.ChartBodyRect, hdc);
				}
				if(clip.IntersectsWith(_layout.BrandInformationRect))
					DrawBrandInformation(hdc);
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				Util.Warning(Env.Frame, ex.Message + "\n�g���L�b�g��ǂݍ��݂Ȃ����܂Ń`���[�g�̕`����~���܂��B");
				Env.CurrentIndicators.Valid = false;
			}
			finally {
				g.ReleaseHdc(hdc);
			}
		}

		//�e�p�[�c�̕`�� ����������ς���
		private void DrawChartBody(IntPtr hdc, Rectangle clip) {
			Indicator[] indicators = Env.CurrentIndicators.GetIndicatorsForChart();
			DataFarm farm = _brand.ReserveFarm();

			if(farm.IsEmpty) {
				RecalcValueWindowFormat();
				DrawBrandInformation(hdc);
				return;
			}

			if(_scaleIsInvalidated) {
				RecalcFormat();
				RecalcValueWindowFormat();
                _scaleIsInvalidated = false;
            }
			DrawScaleLines(hdc);
			DrawMouseTrackingLine(hdc); //����̓��E�\�N�̑O�ɕ`�����ق����q�Q�Ȃǂ������₷��

			Win32.SelectObject(hdc, _pref.DefaultHFont);
			Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.FushiColor));
			int index = _firstDateIndex;
			int end = Math.Min(farm.TotalLength, _firstDateIndex+_layout.DisplayColumnCount);
			_firstDate = this.FirstDate; //farm.GetByIndex(index).Date;
			_lastDate = this.LastDate; //farm.GetByIndex(Math.Min(farm.FilledLength, _firstDateIndex+_layout.DisplayColumnCount)-1).Date;
            if(_firstDate==0) return; //�P���~���̈ꕔ�̃f�[�^�����Ă��錏�̎b��Ή�

			//���ꏈ���F��ڂ̉_
			if(IchimokuKumoIsAvailable())
				DrawIchimokuKumo(hdc, farm, index, end, clip);

			int prev_date_part = 0;
			string caption = "";
			while(index < end) {
				int x = (index - _firstDateIndex) * _layout.DatePitch;
				TradeData td = farm.GetByIndex(index);
				//if(!td.IsFuture) {
					int mid = x + _layout.CandleMiddleOffset;
					if(clip.Left<=x+_layout.DatePitch && x-_layout.DatePitch<clip.Right) { 
#if DOJIMA
						ChartFormat fmt = Env.CurrentIndicators.Format;
                        if(DojimaChart.IsMusousenAvailable(fmt))
                            DojimaChart.DrawMusousen(hdc, _pref.CandlePen, mid, -_priceScaleStep*3, td, clip, _priceTrans);
						if(!td.IsFuture) {
							if(fmt==ChartFormat.HalfDaily)
								DrawHalfDailyChart(hdc, farm as DailyDataFarm, x, td, clip);
							else
								DrawBasicChart(hdc, mid, td, clip);
#else
						if(!td.IsFuture) {
							DrawBasicChart(hdc, mid, td, clip);
#endif

							foreach(Indicator ind in indicators)
								DrawIndicator(hdc, mid, td, null, ind, clip);
							for(int i=0; i<OscillatorPreference.LENGTH; i++)  {
								OscillatorPreference op = _pref.OscillatorPreferences[i];
								if(op.Config!=HeightConfig.None) {
									foreach(Indicator ind in op.OscillatorGroup)
										DrawIndicator(hdc, mid, td, op, ind, clip);
								}
							}
						}
					}

					//��/�S�����̐؂�ւ��
					if(!td.IsFuture) {
						int date_part = td.Date % 100;
						if(CheckMonthDiv(prev_date_part, date_part, td, ref caption)) {
							Win32.SelectObject(hdc, _pref.MonthDivPen.Handle);
							Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));
							Win32.MoveToEx(hdc, x, _layout.HeaderHeight, out _dummyPoint);
							Win32.LineTo(hdc, x, _layout.ChartAreaBottom);
							if(ChartUtil.HasIntersectionY(clip, _layout.ChartAreaBottom, _layout.ChartAreaBottom+_layout.FooterHeight))
								ChartUtil.DrawText(hdc, x+2, _layout.ChartAreaBottom, caption);
						}
						prev_date_part = date_part;

						DrawFushiOrSplit(hdc, mid, td, clip); //�߂͍Ō�̕`��łȂ��ƉB��Ă��܂����Ƃ�����
					}
				//}
				index++;
			}
		}
		
		private bool CheckMonthDiv(int prev_date_part, int date_part, TradeData td, ref string caption) {
			if(Util.IsDailyBased(Env.CurrentIndicators.Format)) {
				if(prev_date_part > date_part) { //���̒l������������
					caption = String.Format("{0}�N{1}��", td.Date / 10000, (td.Date % 10000) / 100);
					return true;
				}
			}
			else if(Env.CurrentIndicators.Format==ChartFormat.Weekly) {
				int t = td.Date;
				DateTime d = new DateTime(t / 10000, (t % 10000) / 100, (t % 100));
				d = d.AddDays(5); //���j���̈ʒu�Ō��߂�̂����R
				int m = d.Month;
				if(d.Day<=7 && (m==1 || m==4 || m==7 || m==10)) {
					caption = String.Format("{0}�N{1}�p", d.Year, (m-1)/3+1);
					return true;
				}
			}
			else { // Monthly
				int t = td.Date;
				int y = t/10000;
				int m = (t % 10000) / 100;
				if(m==1) {
					caption = String.Format("{0}�N", y);
					return true;
				}
			}
			return false;
		}

		private void DrawBasicChart(IntPtr hdc, int mid, TradeData td, Rectangle clip) {
			DrawCandle(hdc, mid, td, clip);
			//volume
			if(_pref.ShowVolume!=HeightConfig.None && _brand.IsVolumeAvailable)
				DrawVolume(hdc, mid, td, clip);
		}

		private void DrawCurrentValue(IntPtr hdc) {
			DataFarm farm = _brand.ReserveFarm();
			int ni = _dateLine._nextToBeDrawn;
			if(farm.IsEmpty || farm.FilledLength<=ni) ni = -1;
			TradeData td = ni==-1? null : farm.GetByIndex(ni);
			DrawValueWindow(hdc, td);
		}

		private void DrawScaleLines(IntPtr hdc) {
			int right = _layout.ChartBodyRect.Right;
			//price
			Win32.SelectObject(hdc, _pref.PriceScalePen.Handle);
			double y;
			double top = _highPrice;
			int price = (int)(Math.Floor(top / _priceScaleStep) * _priceScaleStep);

			while(price >= _lowPrice) {
                y = _priceTrans.TransValue((double)price);

                Win32.MoveToEx(hdc, 0, (int)y, out _dummyPoint);
				Win32.LineTo(hdc, right, (int)y);
				ChartUtil.DrawText(hdc, right, (int)y-5, price.ToString());

				price -= _priceScaleStep;
			}

			//volume
			if(_pref.ShowVolume!=HeightConfig.None && _brand.IsVolumeAvailable) {
				Win32.SelectObject(hdc, _pref.VolumeScalePen.Handle);
				top = _volumeTrans.Inverse(GetPricePaneBottom());
				double volume = (Math.Floor(top / _volumeScaleStep) * _volumeScaleStep);
				y = _volumeTrans.TransValue(volume);
				while(volume >= 0) {
					Win32.MoveToEx(hdc, 0, (int)y, out _dummyPoint);
					Win32.LineTo(hdc, right, (int)y);
					ChartUtil.DrawText(hdc, right, (int)y-6, volume.ToString());

					volume -= _volumeScaleStep;
					y = _volumeTrans.TransValue((double)volume);
				}
			}

			//oscillator
			Win32.SelectObject(hdc, _pref.OscillatorScalePen.Handle);
			for(int i=0; i<OscillatorPreference.LENGTH; i++) {
				OscillatorPreference op = _pref.OscillatorPreferences[i];
				if(op.Config!=HeightConfig.None) {
					foreach(double osc in op.ScaleValues) {
						y = op.Trans.TransValue(osc);
						Win32.MoveToEx(hdc, 0, (int)y, out _dummyPoint);
						Win32.LineTo(hdc, right, (int)y);
						ChartUtil.DrawText(hdc, right, (int)y-6, FormatOscillatorValue(op, osc));
					}
				}
			}
		}
		private string FormatOscillatorValue(OscillatorPreference op, double v) {
			switch(op.OscillatorGroup.Type) {
				case ValueRange.Percent0_1:
				case ValueRange.Percent1_1:
					return (v*100).ToString();
				case ValueRange.Origin0:
					return (v*100).ToString("F2");
				default:
					return v.ToString();
			}
		}

		private void DrawCandle(IntPtr hdc, int x, TradeData td, Rectangle clip) {
			DrawCandle(hdc, x, td.Open, td.High, td.Low, td.Close, clip);
		}
		private void DrawCandle(IntPtr hdc, int x, double open, double high, double low, double close, Rectangle clip) {
            if(!ChartUtil.HasIntersectionY(clip, (int)_priceTrans.TransValue(high), (int)_priceTrans.TransValue(low))) return;

            Win32.SelectObject(hdc, _pref.CandlePen.Handle);
			int halfcw = _pref.HalfCandleWidth;
			//���K�̘g��[x-HALF_CW, x+HALF_CW+1)
			if(open<=close) { //�z��
				int top = (int)_priceTrans.TransValue(close);
				int bottom = (int)_priceTrans.TransValue(open);
                if(_pref.InverseChart) Util.Swap(ref top, ref bottom);
                Win32.SelectObject(hdc, _pref.BackBrush.Handle);
				if(_pref.UseCandleEffect) {
					Win32.Rectangle(hdc, x-halfcw, top, x+halfcw+1, bottom+1);
					//��ƍ�
					Win32.SelectObject(hdc, _pref.CandlePen.DarkDarkPen);
					Win32.MoveToEx(hdc, x+halfcw, top-1, out _dummyPoint);
					Win32.LineTo(hdc, x-halfcw-1, top-1);
					Win32.LineTo(hdc, x-halfcw-1, bottom+1);
					//���ƉE
					Win32.SelectObject(hdc, _pref.CandlePen.DarkPen);
					Win32.MoveToEx(hdc, x+halfcw-1, top+1, out _dummyPoint);
					Win32.LineTo(hdc, x+halfcw-1, bottom-1);
					Win32.LineTo(hdc, x-halfcw  , bottom-1);
				}
				else
					Win32.Rectangle(hdc, x-halfcw, top, x+halfcw+1, bottom+1);
			}
			else { //�A��
				int top = (int)_priceTrans.TransValue(open);
				int bottom = (int)_priceTrans.TransValue(close);
                if(_pref.InverseChart) Util.Swap(ref top, ref bottom);
                Win32.SelectObject(hdc, _pref.InsenBrush.Handle);
                if(_pref.UseCandleEffect) {
					Win32.Rectangle(hdc, x-halfcw+1, top+1, x+halfcw, bottom);
					//��ƍ�
					Win32.SelectObject(hdc, _pref.CandlePen.LightPen);
					Win32.MoveToEx(hdc, x+halfcw, top, out _dummyPoint);
					Win32.LineTo(hdc, x-halfcw, top);
					Win32.LineTo(hdc, x-halfcw, bottom+1);
					//���ƉE
					Win32.SelectObject(hdc, _pref.CandlePen.DarkPen);
					Win32.MoveToEx(hdc, x+halfcw, top+1, out _dummyPoint);
					Win32.LineTo(hdc, x+halfcw, bottom);
					Win32.LineTo(hdc, x-halfcw, bottom);
				}
				else
					Win32.Rectangle(hdc, x-halfcw, top, x+halfcw+1, bottom+1);
			}

			Win32.SelectObject(hdc, _pref.CandlePen.Handle);
			//�Ђ�
			if(high > Math.Max(open, close)) {
				Win32.MoveToEx(hdc, x, (int)_priceTrans.TransValue(high), out _dummyPoint);
				Win32.LineTo(hdc, x, (int)_priceTrans.TransValue(Math.Max(open, close)));
			}
			if(low < Math.Min(open, close)) {
				Win32.MoveToEx(hdc, x, (int)_priceTrans.TransValue(Math.Min(open, close)), out _dummyPoint);
				Win32.LineTo(hdc, x, (int)_priceTrans.TransValue(low));
			}

		}
		private void DrawFushiOrSplit(IntPtr hdc, int x, TradeData td, Rectangle clip) {
			bool low = false;
			//��
			int upmargin = _pref.InverseChart ? 5 : -5-_layout.DefaultTextHeight;
			int downmargin = _pref.InverseChart ? -5-_layout.DefaultTextHeight : 5;
			if(td.Fushi==Fushi.High) 
			{
				int y = (int)_priceTrans.TransValue(td.High)+upmargin;
                if(ChartUtil.HasIntersectionY(clip, y, y + _layout.DefaultTextHeight)) {
                    Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.FushiColor));
					string t = td.High.ToString(_brand.PriceFormatString);
					ChartUtil.DrawText(hdc, x-t.Length*_layout.DefaultTextWidth/2, y, t);
				}
			}
			else if(td.Fushi==Fushi.Low) {
				int y = (int)_priceTrans.TransValue(td.Low)+downmargin;
                if(ChartUtil.HasIntersectionY(clip, y, y + _layout.DefaultTextHeight)) {
                    Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.FushiColor));
					string t = td.Low.ToString(_brand.PriceFormatString);
					ChartUtil.DrawText(hdc, x-t.Length*_layout.DefaultTextWidth/2, y, t);
				}
				low = true;
			}

			//����
			if(_brand.SplitInfo!=null) {
				foreach(SplitInfo si in _brand.SplitInfo) {
					if(td.CoversDate(si.Date)) {
						int y = (int)_priceTrans.TransValue(td.Low)+5+(low? _layout.DefaultTextHeight : 0);
                        if(ChartUtil.HasIntersectionY(clip, y, y + _layout.DefaultTextHeight)) {
                            Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.FushiColor));
							string t = "����"+si.Ratio.ToString();
							ChartUtil.DrawText(hdc, x-_layout.DefaultTextWidth/2-2, y, t);
						}
						break;
					}
				}
			}
		}

		private void DrawVolume(IntPtr hdc, int x, TradeData td, Rectangle clip) {
			DrawVolume(hdc, x, td.Volume, clip);
		}
		private void DrawVolume(IntPtr hdc, int x, double volume, Rectangle clip) {
			Win32.RECT r = new Win32.RECT();
			r.left = x-_pref.HalfCandleWidth;
			r.top = (int)_volumeTrans.TransValue(volume);
			r.right = x+_pref.HalfCandleWidth;
			r.bottom = (int)_volumeTrans.TransValue(0)+1;
            if(ChartUtil.HasIntersectionY(clip, r.top, r.bottom)) {
                Win32.FillRect(hdc, ref r, _pref.VolumeBrush.Handle);

				Win32.POINT pt;
				Win32.SelectObject(hdc, _pref.VolumeBrush.LightPen);
				Win32.MoveToEx(hdc, r.left, r.top, out pt);
				Win32.LineTo(hdc, r.right+1, r.top);
				Win32.MoveToEx(hdc, r.left, r.top, out pt);
				Win32.LineTo(hdc, r.left, r.bottom);
				Win32.SelectObject(hdc, _pref.VolumeBrush.DarkPen);
				Win32.MoveToEx(hdc, r.right, r.top+1, out pt);
				Win32.LineTo(hdc, r.right, r.bottom);
			}
		}
		
		//indicator�̕`��Fop�̓I�V���[�^�ȊO�ł�null
		private void DrawIndicator(IntPtr hdc, int x, TradeData td, OscillatorPreference op, Indicator ind, Rectangle clip) {
			if(ind==null) return; //!!_currentOscillatorGroup�����������̂��Anull�������Ă���P�[�X���������B��������������
			if(ind.Target==IndicatorTarget.Volume && _pref.ShowVolume==HeightConfig.None) return;

			TradeData pr = td.Prev;
			if(pr==null) return;
			if(ind.TargetBrand!=null && !ind.TargetBrand.Applicable(_brand.Code)) return; //�K�p�s��

			double v1 = pr.GetValue(ind);
			double v2 = td.GetValue(ind);
			if(!Double.IsNaN(v1) && !Double.IsNaN(v2)) {
				if(ind.Target==IndicatorTarget.Volume && (v1==0 || v2==0)) return; //���݂��Ȃ��M�p�c�����

				//���Ή��\���p�����[�^�����Ă���Β���
				if(ind.RelativiseParam!=null) {
					double m = _relativisedMultipliers[ind.LaneID];
					if(m==0) {
						m = ind.RelativiseParam.CalcMultiplier(ind, td.Farm);
						_relativisedMultipliers[ind.LaneID] = m;
					}
					v1 *= m;
					v2 *= m;

					//����������炳��Ɂ�������
					if(td.Date==ind.RelativiseParam.Date) {
						Win32.RECT rect = new Win32.RECT();
						rect.left = x-2; rect.right = x+2;
						rect.top = ConvertToY(ind.Target, null, v2)-2; rect.bottom = rect.top+4;
						Win32.FillRect(hdc, ref rect, new ZBrush(ind.Appearance.Pen.Color).Handle);
					}
				}
				int y1 = ConvertToY(ind.Target, op, v1);
				int y2 = ConvertToY(ind.Target, op, v2);
                if(ChartUtil.HasIntersectionY(clip, y1, y2)) {
                    Win32.SelectObject(hdc, ind.Appearance.Pen.Handle);
					Win32.MoveToEx(hdc, x-_layout.DatePitch, y1, out _dummyPoint);
					Win32.LineTo(hdc, x, y2);
				}
			}
		}

        private void DrawMouseTrackingLine(IntPtr hdc)
        {

            //hacked by rti.
            //���������̊J�n�ƏI���Ƀ��C����`��
            //�֐��ɂ������C�����邪�Ƃ肠�����A����(w
            {
                //�J�n
                if (_AutoTradeStartDateLine._nextToBeDrawn != -1)
                {
                    int x = (_AutoTradeStartDateLine._nextToBeDrawn - _firstDateIndex) * _layout.DatePitch + _layout.DatePitch / 2;
                    Win32.SelectObject(hdc, _pref.AutoTradingStartLinePen.Handle);
                    Win32.MoveToEx(hdc, x, _layout.HeaderHeight, out _dummyPoint);
                    Win32.LineTo(hdc, x, _owner.Height - _layout.FooterHeight);
                    _AutoTradeStartDateLine._lastDrawn = _dateLine._nextToBeDrawn;
                }
                //�I��
                if (_AutoTradeEndDateLine._nextToBeDrawn != -1)
                {
                    int x = (_AutoTradeEndDateLine._nextToBeDrawn - _firstDateIndex) * _layout.DatePitch + _layout.DatePitch / 2;
                    Win32.SelectObject(hdc, _pref.AutoTradingEndLinePen.Handle);
                    Win32.MoveToEx(hdc, x, _layout.HeaderHeight, out _dummyPoint);
                    Win32.LineTo(hdc, x, _owner.Height - _layout.FooterHeight);
                    _AutoTradeEndDateLine._lastDrawn = _dateLine._nextToBeDrawn;
                }
            }
            //--------------------

            if (Env.Preference.MouseTrackingLineMode != MouseTrackingLineMode.None)
            {
                if (_dateLine._nextToBeDrawn != -1)
                {
                    int x = (_dateLine._nextToBeDrawn - _firstDateIndex) * _layout.DatePitch + _layout.DatePitch / 2;
                    Win32.SelectObject(hdc, _pref.MouseTrackingLinePen.Handle);
                    Win32.MoveToEx(hdc, x, _layout.HeaderHeight, out _dummyPoint);
                    Win32.LineTo(hdc, x, _owner.Height - _layout.FooterHeight);
                    _dateLine._lastDrawn = _dateLine._nextToBeDrawn;
                }
            }
            if (Env.Preference.MouseTrackingLineMode == MouseTrackingLineMode.Full)
            { //!!Flag�ɂ����ق���������
                int y = _priceLine._nextToBeDrawn;
                if (y >= _layout.HeaderHeight && y < GetPricePaneBottom())
                {
                    Win32.SelectObject(hdc, _pref.MouseTrackingLinePen.Handle);
                    Win32.MoveToEx(hdc, 0, y, out _dummyPoint);
                    Win32.LineTo(hdc, _layout.ChartAreaWidth, y);
                    double price = _priceTrans.Inverse((double)y);
                    double yobine = Util.Yobine(_brand.Market, price);
                    price = Math.Floor(price / yobine + 0.5) * yobine; //�Ēl�̐����{�ɏC��
                    Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.MouseTrackingLinePen.Color));
                    ChartUtil.DrawText(hdc, _layout.ChartAreaWidth, y - _layout.DefaultTextHeight / 2, price.ToString("F0"));
                    _priceLine._lastDrawn = y;
                }
            }

        }


		private void DrawBrandInformation(IntPtr hdc) {
			Win32.SelectObject(hdc, _pref.HeaderHFont);
			Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));

			AbstractBrand br = _brand;
			StringBuilder bld = new StringBuilder();
			bld.Append(EnumDescAttribute.For(typeof(MarketType)).GetDescription(br.Market));
			bld.Append(" �R�[�h:");
			bld.Append(br.CodeAsString);
			BasicBrand bb = br as BasicBrand;
			if(bb!=null) {
				if(bb.Nikkei225) bld.Append(" ���o225�̗p");
				if(bb.Unit!=0) {
					bld.Append(" �P������:");
					bld.Append(bb.Unit);
				}
			}

			if(_brand.ReserveFarm().IsEmpty) {
				bld.Append("  (�f�[�^���擾�ł��܂���)");
			}
			else {
				ChartFormat format = Env.CurrentIndicators.Format;
				int fd = Env.Frame.ChartCanvas.DrawingEngine.FirstDate;
				int ld = Env.Frame.ChartCanvas.DrawingEngine.LastDate;
				string h = String.Format("  �\������:{0} - {1}", Util.FormatFullDate(fd, format), Util.FormatFullDate(ld, format));
				bld.Append(h);
			}

			int offset = 7; //�����E�֕`��
			ChartUtil.DrawTextLimitedWidth(hdc, bld.ToString(), offset, _layout.HeaderHeight+1, _layout.ChartAreaWidth-offset, _layout.DefaultTextHeight);
			Win32.SelectObject(hdc, _pref.DefaultHFont);
		}

		private void DrawValueWindow(IntPtr hdc, TradeData td) {
			Win32.SelectObject(hdc, _pref.DefaultHFont);
			Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));

			int ix = _layout.CurrentValueRect.Left + 5;
			int vx = ix + _layout.DefaultTextWidth*11;
			Indicator[] inds = Env.CurrentIndicators.GetIndicatorsForValueWindow();
			int y = _layout.HeaderHeight;

			if(td!=null) ChartUtil.DrawText(hdc, ix, y, Util.FormatFullDate(td.Date, Env.CurrentIndicators.Format));
			y += _layout.DefaultTextHeight;
			foreach(Indicator ind in inds) {
				if(ind.Target==IndicatorTarget.Oscillator && !IsVisibleOscillator(ind)) continue;
				if(ind.TargetBrand!=null && !ind.TargetBrand.Applicable(_brand.Code)) continue; //�K�p�s��

				if(ind.Appearance!=null)
					Win32.SetTextColor(hdc, Util.ToCOLORREF(ind.Appearance.Color));
				else
					Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));

				ChartUtil.DrawText(hdc, ix, y, ind.Name);
				if(_tipBuilding) {
					int w = MeasureString(ind.Name);
					if(w > vx-ix)
						_tipEntries.Add(new ComplementaryTextEntry(new Rectangle(ix, y, w, _layout.DefaultTextHeight), ind.Name));
				}
				double val = Double.NaN;
				if(td!=null) {
					val = td.GetValue(ind);
				}
				ChartUtil.DrawText(hdc, vx, y, Util.FormatFixedLenValue(val, 9, ind.GetFormatString(_brand), ind.FormatModifier));
				y += _layout.DefaultTextHeight;
			}
		}

		private void DrawExplanation(IntPtr hdc) {
			Win32.SelectObject(hdc, _pref.DefaultHFont);
			Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));

			int ix = _layout.CurrentValueRect.Left + 5;
			int lx = ix + _layout.DefaultTextWidth*14;
			Indicator[] inds = Env.CurrentIndicators.GetIndicatorsForExplanationWindow();
			int y = _layout.HeaderHeight + _layout.DefaultTextHeight * _maximumValueWindowItemCount + 30;

			Point pt = _owner.PointToClient(Control.MousePosition);
			foreach(Indicator ind in inds) {
				if(ind.Target==IndicatorTarget.Oscillator && !IsVisibleOscillator(ind)) continue;

				IndicatorAppearance ia = ind.Appearance;
				ZPen pen = ia==null? _pref.DefaultPen : ia.Pen;
				ChartUtil.DrawText(hdc, ix, y, ind.Name);
				if(_tipBuilding) {
					int w = MeasureString(ind.Name);
					if(w > lx-ix)
						_tipEntries.Add(new ComplementaryTextEntry(new Rectangle(ix, y, w, _layout.DefaultTextHeight), ind.Name));
				}
				Win32.MoveToEx(hdc, lx, y+_layout.DefaultTextHeight/2-1, out _dummyPoint);
				Win32.SelectObject(hdc, pen.Handle);
				Win32.LineTo(hdc, lx+30, y+_layout.DefaultTextHeight/2-1);
				y += _layout.DefaultTextHeight;
			}
		}

        private void DrawAccumulativeVolume(IntPtr hdc) {
            if(!_brand.IsVolumeAvailable || !_accumulativeVolume.Available || _priceTrans==null) return;

            double price = _accumulativeVolume.StartPrice;
            Rectangle acc_rect = _layout.AccumulativeVolumeRect;
            Win32.SelectObject(hdc, _pref.DefaultHFont);
            Win32.SetTextColor(hdc, Util.ToCOLORREF(_pref.TextColor));
            int char_height = (int)_pref.DefaultCharPitch.Height;
            //�_�̔����̕��BEndPrice�t�߂��g���̂͑ΐ��O���t�ł�����Ȃ��悤�ɂ��邽��
            int half_height = (int)Math.Abs(_priceTrans.TransValue(_accumulativeVolume.EndPrice) - _priceTrans.TransValue(_accumulativeVolume.EndPrice - _accumulativeVolume.Pitch)) / 2 - 2;
            if(half_height < 2) half_height = 2;

            ChartUtil.DrawText(hdc, acc_rect.Left, acc_rect.Top, String.Format("���i�ѕʏo����"));
            ChartUtil.DrawText(hdc, acc_rect.Left, acc_rect.Top + char_height, String.Format("{0}�`", Util.FormatShortDate(_accumulativeVolume.BeginDate)));
            ChartUtil.DrawText(hdc, acc_rect.Left, acc_rect.Top + char_height*2, String.Format("�@�@�@{0}", Util.FormatShortDate(_accumulativeVolume.EndDate)));
            double scale = ChartUtil.SelectGoodValue(_accumulativeVolume.MaxVolume);
            if(scale < _accumulativeVolume.MaxVolume) scale = ChartUtil.SelectGoodValue(_accumulativeVolume.MaxVolume*2); //�͂ݏo�邱�Ƃ��Ȃ��悤��
            Trans volume_trans = Trans.Solve(0, acc_rect.Left, scale, acc_rect.Right, false, false);

            for(int index = 0; index < _accumulativeVolume.DataLength; index++) {
                int y = (int)_priceTrans.TransValue(price + _accumulativeVolume.Pitch/2);
                double value = _accumulativeVolume[index];
                if(value != 0) {
                    Win32.RECT r = new Win32.RECT();
                    r.left = acc_rect.Left;
                    r.top = y - half_height;
                    r.right = (int)volume_trans.TransValue(value);
                    r.bottom = y + half_height;
                    Win32.FillRect(hdc, ref r, _pref.VolumeBrush.Handle);

                    Win32.POINT pt;
                    Win32.SelectObject(hdc, _pref.VolumeBrush.LightPen);
                    Win32.MoveToEx(hdc, r.left, r.top, out pt);
                    Win32.LineTo(hdc, r.right + 1, r.top);
                    Win32.MoveToEx(hdc, r.right, r.top, out pt);
                    Win32.LineTo(hdc, r.right, r.bottom);
                    Win32.SelectObject(hdc, _pref.VolumeBrush.DarkPen);
                    Win32.MoveToEx(hdc, r.left, r.bottom + 1, out pt);
                    Win32.LineTo(hdc, r.right, r.bottom + 1);
                }
                price += _accumulativeVolume.Pitch;
            }

            //�c��
            Win32.SelectObject(hdc, _pref.VolumeScalePen.Handle);
            const int SCALE_COUNT = 4;
            for(int i = 0; i < SCALE_COUNT; i++) {
                int x = acc_rect.Left + acc_rect.Width / SCALE_COUNT * i;
                Win32.MoveToEx(hdc, x, acc_rect.Top + char_height * 3, out _dummyPoint);
                int y = acc_rect.Bottom - char_height * (i % 2 == 0 ? 1 : 2); //�������傫���Ȃ肪���Ȃ̂Ō݂��Ⴂ�ɂ���
                Win32.LineTo(hdc, x, y);
                string scale_string = (scale/SCALE_COUNT*i).ToString();
                ChartUtil.DrawText(hdc, x - (int)_pref.DefaultCharPitch.Width * scale_string.Length / 2, y, scale_string);
            }
        }


        //Conversions
		private int ConvertToY(IndicatorTarget target, OscillatorPreference op, double value) {
			switch(target) {
				case IndicatorTarget.Price:
					return (int)_priceTrans.TransValue(value);
				case IndicatorTarget.Volume:
					return (int)_volumeTrans.TransValue(value);
				case IndicatorTarget.Oscillator:
					return (int)op.Trans.TransValue(value);
                case IndicatorTarget.None:
                    return 0;
            }
			return 0;
		}
		private int DateOffsetToX(int offset) {
			return offset*_layout.DatePitch + _layout.CandleMiddleOffset;
		}

		//Formats
		private void RecalcFormat() {
			int width = _owner.Width;
			_pref = Env.Preference;
			DataFarm farm = _brand.ReserveFarm();

            int half_height = _layout.DefaultTextHeight / 2;
            int end = Math.Min(farm.FilledLength, _firstDateIndex + _layout.DisplayColumnCount);

            double chart_bottom = (double)GetPricePaneBottom();
            if(chart_bottom < 0) chart_bottom = 0;

            //�l�i�]�[��
            if(!_pref.ScaleLock) { //Brand�����Z�b�g����ƃ��b�N���O���̂ł���ł悢
                double range_min = new IndicatorTimeSeries(farm, Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Low), _firstDateIndex, end).Min;
                double range_max = new IndicatorTimeSeries(farm, Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.High), _firstDateIndex, end).Max;
               	//�{�����W���[�o���h�ȂǁA�㉺�ɍL����`���[�g�̂��Ƃ��l���Ă�����ƍL�߂ɂƂ�
                double t = (range_max - range_min) * 0.1;
                _lowPrice = range_min - t;
                _highPrice = range_max + t;

#if DOJIMA
                _priceScaleStep = ChartUtil.CalcScaleStep((_highPrice - _lowPrice)*1.2, chart_bottom - (double)_layout.HeaderHeight, 60);
                if(Dojima.DojimaChart.IsMusousenAvailable(Env.CurrentIndicators.Format))
					_lowPrice -= _priceScaleStep * 3; //���̒l�i�ɂR�i���]�T�����
#else
                _priceScaleStep = (int)ChartUtil.CalcScaleStep(_highPrice - _lowPrice, chart_bottom - (double)_layout.HeaderHeight, 60);
#endif
                _priceTrans = Trans.Solve(_highPrice, (double)_layout.HeaderHeight, _lowPrice, chart_bottom, _pref.LogScale, _pref.InverseChart);
				if(_pref.ShowAccumulativeVolume) {
					int first_index = _dateLine._nextToBeDrawn == -1 ? farm.FilledLength - 1 : Math.Min(_dateLine._nextToBeDrawn, farm.FilledLength - 1);
					int price_pitch = Math.Max(1, _priceScaleStep / AccumulativeVolume.DATA_PER_SCALELINE);
					_accumulativeVolume.Fill(farm, first_index, range_min, price_pitch, range_max);
				}
            }

            //�o�����]�[��
			if(_pref.ShowVolume!=HeightConfig.None) {
				double max_volume = new IndicatorTimeSeries(farm, Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Volume), _firstDateIndex, end).Max;
				max_volume = Math.Max(max_volume, new IndicatorTimeSeries(farm, Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.CreditShort), _firstDateIndex, end).Max);
				max_volume = Math.Max(max_volume, new IndicatorTimeSeries(farm, Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.CreditLong), _firstDateIndex, end).Max);
				double volume_bottom = (double)GetVolumePaneBottom() - half_height;
				if(volume_bottom<0) volume_bottom=0;
				_volumeTrans = LinearTrans.Solve(max_volume, chart_bottom, 0, volume_bottom);
				_volumeScaleStep = ChartUtil.CalcScaleStep(max_volume, volume_bottom-chart_bottom, 30);
			}

			//�I�V���[�^�]�[��
			int top = this.GetVolumePaneBottom();
			for(int i=0; i<OscillatorPreference.LENGTH; i++) {
				OscillatorPreference op = _pref.OscillatorPreferences[i];
				if(op.Config!=HeightConfig.None) {
					if(op.OscillatorGroup.Type==ValueRange.Percent0_1) {
						op.SetScaleValues(1, 0.5, 0);
					}
					else if(op.OscillatorGroup.Type==ValueRange.Percent1_1) {
						op.SetScaleValues(1, 0, -1);
					}
					else if(op.OscillatorGroup.Type==ValueRange.Origin0) {
						double m = 0;
						foreach(Indicator ind in op.OscillatorGroup) {
							TimeSeries ts = new IndicatorTimeSeries(farm, ind, _firstDateIndex, end);
							m = Math.Max(m, Math.Max(Math.Abs(ts.Max), Math.Abs(ts.Min)));
						}
						m = ChartUtil.SelectGoodValue(m);
						op.SetScaleValues(m, 0, -m);
					}
					else {
						double min = Double.MaxValue;
						double max = Double.MinValue;
						foreach(Indicator ind in op.OscillatorGroup) {
							TimeSeries ts = new IndicatorTimeSeries(farm, ind, _firstDateIndex, end);
							max = Math.Max(max, ts.Max);
							min = Math.Min(min, ts.Min);
						}
						double mid = (max+min)/2;
						double pitch = ChartUtil.SelectGoodValue((max-min)/2);
						mid = pitch * (int)(mid / pitch); //�l�̌ܓ����Ă̐����{
						op.SetScaleValues(mid + pitch, mid, mid - pitch);
					}

					op.Trans = LinearTrans.Solve(op.ScaleValues[0], top+half_height, op.ScaleValues[2], top + _layout.OscillatorPaneHeights[i] -half_height);
				}
				top += _layout.OscillatorPaneHeights[i];
			}

			//FreeLine
			SolidFreeLine[] sls = Env.FreeLines.Find(_brand, Env.CurrentIndicators.Format, _pref.LogScale);
			_freeLines.Clear();
			foreach(SolidFreeLine sl in sls)
				_freeLines.Add(new FreeLine(farm, _firstDateIndex, sl, _priceTrans));
		}
		private void RecalcValueWindowFormat() {

			Indicator[] inds = Env.CurrentIndicators.GetIndicatorsForValueWindow();
			_maximumValueWindowItemCount = 0;
			foreach(Indicator ind in inds) 
				if(ind.Target!=IndicatorTarget.Oscillator) _maximumValueWindowItemCount++;
			//�����OscillatorGroup���l��
			int im = 0;
			foreach(OscillatorPreference op in _pref.OscillatorPreferences) {
				if(op.OscillatorGroup!=null)
					im += op.OscillatorGroup.Count;
			}
			_maximumValueWindowItemCount += im;
		}
		private int GetPricePaneBottom() {
			return _layout.ChartAreaBottom-_layout.OscillatorPaneHeightTotal-_layout.VolumePaneHeight;
		}
		private int GetVolumePaneBottom() {
			return _layout.ChartAreaBottom-_layout.OscillatorPaneHeightTotal;
		}

		private bool IsVisibleOscillator(Indicator ind) {
			for(int i=0; i<OscillatorPreference.LENGTH; i++) {
				OscillatorPreference op = _pref.OscillatorPreferences[i];
				if(op.Config!=HeightConfig.None) {
					OscillatorGroup g = op.OscillatorGroup;
					if(g.Contains(ind)) return true;
				}
			}
			return false;
		}


		//FreeLine�̒ǉ�
		public void FixFreeLine(FreeLine fl) {
			Env.FreeLines.Add(_brand, Env.CurrentIndicators.Format,_pref.LogScale, fl.ToSolid(_brand.ReserveFarm(), _firstDateIndex, _priceTrans));
			_freeLines.Add(fl);
		}
		public bool RemoveHighlitedFreeLines() {
			ArrayList remain = new ArrayList();
			bool f = false;
			foreach(FreeLine line in _freeLines) {
				if(line.DrawMode==FreeLine.LineDrawMode.Normal) {
					remain.Add(line);
				}
				else {
					if(line.ID!=-1) Env.FreeLines.Remove(line.ID);
					f = true;
				}
			}
			_freeLines = remain;
			return f; //�P�ł�����������true��Ԃ�
		}

		//����Ȃ�̕`��
		//��ڋύt�\�̉_
		private bool IchimokuKumoIsAvailable() {
			IList l = Env.CurrentIndicators.IchimokuKumo;
			if(l.Count<2) return false;
			Indicator indA = (Indicator)l[0];
			Indicator indB = (Indicator)l[1];
			//�������`�悳���Ƃ��̂݉_��`��
			return ((indA.Display & IndicatorDisplay.Chart)!=IndicatorDisplay.None && indA.Appearance.Style!=IndicatorStyle.None) &&
			       ((indB.Display & IndicatorDisplay.Chart)!=IndicatorDisplay.None && indB.Appearance.Style!=IndicatorStyle.None);
		}
		private void DrawIchimokuKumo(IntPtr hdc, DataFarm farm, int index, int end, Rectangle clip) {
			while(index < end) {
				int x = (index - _firstDateIndex) * _layout.DatePitch;
				int mid = x + _layout.CandleMiddleOffset;
				TradeData td = farm.GetByIndex(index);
				if(clip.Left<=x+_pref.DatePitch && x-_layout.DatePitch<clip.Right) { 
					DrawIchimokuKumoParts(hdc, mid, td);
				}

				index++;
			}
		}
		private void DrawIchimokuKumoParts(IntPtr hdc, int x, TradeData td) {
			IList l = Env.CurrentIndicators.IchimokuKumo;
			Indicator indA = (Indicator)l[0];
			Indicator indB = (Indicator)l[1];

			double vA = td.GetValue(indA);
			double vB = td.GetValue(indB);
			if(Double.IsNaN(vA) || Double.IsNaN(vB)) return;
			int y1A = (int)_priceTrans.TransValue(vA);
			int y1B = (int)_priceTrans.TransValue(vB);

			TradeData pr = td.Prev;
			if(pr==null) return;
			double pA = pr.GetValue(indA);
			double pB = pr.GetValue(indB);
			if(Double.IsNaN(pA) || Double.IsNaN(pB)) return;
			int y0A = (int)_priceTrans.TransValue(pA);
			int y0B = (int)_priceTrans.TransValue(pB);

			//�����܂łŒl���o������
			int x0 = x-_layout.DatePitch;
			if(pA>=pB && vA>=vB)
				PaintKumo(hdc, indA.Appearance.Color, x0, y0A, x, y1A, x, y1B, x0, y0B);
			else if(pA<=pB && vA<=vB)
				PaintKumo(hdc, indB.Appearance.Color, x0, y0B, x, y1B, x, y1A, x0, y0A);
			else { //�����B�߂�ǂ��������Ȃ�
				double r = (double)(Math.Abs(y0A-y0B)) / (double)(Math.Abs(y0A-y0B)+Math.Abs(y1A-y1B));
				int cx = x - (int)((double)_layout.DatePitch*(1-r));
				int cy = (int)(y1A*r + y0A*(1-r));
				if(pA>=pB) {
					PaintKumo(hdc, indA.Appearance.Color, cx, cy, x0, y0A, x0, y0B);
					PaintKumo(hdc, indB.Appearance.Color, cx, cy, x , y1A, x , y1B);
				}
				else {
					PaintKumo(hdc, indB.Appearance.Color, cx, cy, x0, y0A, x0, y0B);
					PaintKumo(hdc, indA.Appearance.Color, cx, cy, x , y1A, x , y1B);
				}
			}
		}
		private void PaintKumo(IntPtr hdc, Color col, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
			Win32.POINT[] points = new Win32.POINT[4];
			points[0].x = x0;
			points[0].y = y0;
			points[1].x = x1;
			points[1].y = y1;
			points[2].x = x2;
			points[2].y = y2;
			points[3].x = x3;
			points[3].y = y3;

			IntPtr rgn = Win32.CreatePolygonRgn(points, 4, 2); //2��WINDING
			IntPtr brush = Win32.CreateSolidBrush(Util.ToCOLORREF(col));
			Win32.FillRgn(hdc, rgn, brush);
			Win32.DeleteObject(brush);
			Win32.DeleteObject(rgn);
		}
		private void PaintKumo(IntPtr hdc, Color col, int x0, int y0, int x1, int y1, int x2, int y2) {
			Win32.POINT[] points = new Win32.POINT[3];
			points[0].x = x0;
			points[0].y = y0;
			points[1].x = x1;
			points[1].y = y1;
			points[2].x = x2;
			points[2].y = y2;

			IntPtr rgn = Win32.CreatePolygonRgn(points, 3, 2); //2��WINDING
			IntPtr brush = Win32.CreateSolidBrush(Util.ToCOLORREF(col));
			Win32.FillRgn(hdc, rgn, brush);
			Win32.DeleteObject(brush);
			Win32.DeleteObject(rgn);
		}

		//y���W����A�����ɍł��߂��Ēl�ɑΉ�����y���W�ɕϊ�����
		public int NormalizeByYobine(int y) {
			if(_priceTrans==null || _brand==null) return y;
			double price = _priceTrans.Inverse(y);
			double yobine = Util.Yobine(_brand.Market, price);

			return (int)_priceTrans.TransValue(yobine * Math.Round(price / yobine));
		}

		//utils
		private int MeasureString(string text) {
			int n = 0;
			for(int i=0; i<text.Length; i++) {
				char ch = text[i];
				if(ch<=0xFF)
					n++;
				else
					n += 2;
			}
			return n * _layout.DefaultTextWidth;
		}

#if DOJIMA
		private HalfDailyDataFarm _halfDailyDataFarm;
		private void DrawHalfDailyChart(IntPtr hdc, DailyDataFarm farm, int x, TradeData td, Rectangle clip) {
			if(_halfDailyDataFarm==null)
				_halfDailyDataFarm = Dojima.DojimaUtil.HalfDailyDataFarmCache.Get(farm);

			int width = _layout.CandleMiddleOffset / 2;
			bool vol = _pref.ShowVolume!=HeightConfig.None && _brand.IsVolumeAvailable;
			HalfDailyDataUnit unit = _halfDailyDataFarm.GetData(td.Index, HalfDay.Zenba);
			DrawCandle(hdc, x + width, unit.open, unit.high, unit.low, unit.close, clip);
			if(vol) DrawVolume(hdc, x + width, unit.volume, clip);
			unit = _halfDailyDataFarm.GetData(td.Index, HalfDay.Goba);
			DrawCandle(hdc, x + width*3, unit.open, unit.high, unit.low, unit.close, clip);
			if(vol) DrawVolume(hdc, x + width*3, unit.volume, clip);

			//����ɏT���̌��Ƀ}�[�N
			bool flag = true;
			DateTime mark = Util.IntToDate(td.Date);
			mark = mark.AddDays(1);
			while(mark.DayOfWeek!=DayOfWeek.Saturday) {
				if(Util.IsMarketOpenDate(mark)) {
					flag = false; //�����ȍ~�y�j���ȑO�Ɏs��̊J���Ă��������������`�悵�Ȃ�
					break;
				}
				mark = mark.AddDays(1);
			}
			if(flag) {
				double p = td.Low + _priceScaleStep / 5 * (_pref.InverseChart? 1 : -1);
				ZBrush cb = new ZBrush(Color.Red);
				ZPen cp = new ZPen(Color.Red, ZPen.PenStyle.Normal);
				IntPtr ob = Win32.SelectObject(hdc, cb.Handle);
				IntPtr op = Win32.SelectObject(hdc, cp.Handle);
				int t = _layout.CandleMiddleOffset / 3; //���a
				int y = (int)_priceTrans.TransValue(p);
				Win32.Ellipse(hdc, x+width*3-t, y-t, x+width*3+t, y+t);
				Win32.SelectObject(hdc, ob);
				Win32.SelectObject(hdc, op);
				cb.Dispose();
				cp.Dispose();
			}
		}
#endif

	}

	internal class ChartUtil {
		public static double CalcScaleStep(double range, double pane_height, double standard_pitch_in_pixel) {
			if(pane_height<=0) return 1;
			double t = range / (pane_height / standard_pitch_in_pixel);
			return SelectGoodValue(t);

		}

		//src�ɍł��߂��A�؂�̗ǂ�����Ԃ��B�؂肪�悢�Ƃ́A(1 or 2.5 or 5) * 10^n
		public static double SelectGoodValue(double src) {
			if(src<0)
				return -SelectGoodValue(-src);

			double log = Math.Log10(src);
			int n = (int)Math.Floor(log);
			double a = log - n;

			double b;
			if(a < 0.16)
				b = 1;
			else if(a < 0.5)
				b = 2.5;
			else if(a < 0.83)
				b = 5;
			else {
				b = 1;
				n++;
			}

			if(n>0)
				for(int i=0; i<n; i++) b*=10;
			else
				for(int i=0; i<-n; i++) b/=10;
			return b;
		}

		public static void DrawText(IntPtr hdc, int x, int y, string text) {
			unsafe {
				int len = text.Length;
				fixed(char* p = text) {
					Win32.TextOut(hdc, x, y, p, len);
				}
			}
		}
		private static Win32.RECT _tempRect = new Win32.RECT();
		public static void DrawTextLimitedWidth(IntPtr hdc, string text, int x, int y, int width, int height) {
			_tempRect.left = x;
			_tempRect.top = y;
			_tempRect.right = x + width;
			_tempRect.bottom = y + height;
			Win32.DrawText(hdc, text, text.Length, ref _tempRect, 0);
		}
        public static bool HasIntersectionY(Rectangle clip, int y1, int y2) {
            if(y1 > y2) {
                int t = y1;
                y1 = y2;
                y2 = t;
            }
            return !(y1 > clip.Bottom || y2 < clip.Top);
        }
    }

    internal class ComplementaryTextEntry {
		public Rectangle rect;
		public string text;
		public ComplementaryTextEntry(Rectangle r, string t) {
			rect = r;
			text = t;
		}


	}
}
