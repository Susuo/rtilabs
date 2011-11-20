/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

using Zanetti.Indicators;
using Zanetti.Data;
using Zanetti.Forms;

namespace Zanetti.UI
{
	/// <summary>
	/// </summary>
	internal class ChartCanvas : System.Windows.Forms.UserControl
	{
		private AbstractBrand _brand;
		private ChartDrawing _drawing;
		private ChartTitle _title;
		private System.Windows.Forms.HScrollBar _scrollBar;
		private Label _complementaryLabel;
		
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ChartCanvas()
		{
			// ���̌Ăяo���́AWindows.Forms �t�H�[�� �f�U�C�i�ŕK�v�ł��B
			InitializeComponent();
			
			// TODO: InitializeComponent �Ăяo���̌�ɏ�����������ǉ����܂��B
			this.BackColor = Env.Preference.BackBrush.Color;
			this.SetStyle(ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true);
			_drawing = new ChartDrawing(this);
		}


		/// <summary>
		/// �g�p����Ă��郊�\�[�X�Ɍ㏈�������s���܂��B
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region �R���|�[�l���g �f�U�C�i�Ő������ꂽ�R�[�h 
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
		/// </summary>
		private void InitializeComponent()
		{
			this._scrollBar = new System.Windows.Forms.HScrollBar();
			_title = new ChartTitle();
			this.SuspendLayout();
			// 
			// _scrollBar
			// 
			this._scrollBar.Dock = System.Windows.Forms.DockStyle.Bottom;
			this._scrollBar.Enabled = false;
			this._scrollBar.LargeChange = 2;
			this._scrollBar.Location = new System.Drawing.Point(0, 83);
			this._scrollBar.Maximum = 4;
			this._scrollBar.Name = "_scrollBar";
			this._scrollBar.Size = new System.Drawing.Size(100, 17);
			this._scrollBar.TabIndex = 0;
			this._scrollBar.ValueChanged += new System.EventHandler(this.OnScrollBarValueChanged);

			_title.Location = new Point(0, 0);
			_title.Size = new Size(this.Width, Env.Layout.HeaderHeight);
		
			// 
			// ChartCanvas
			// 
			this.Controls.Add(this._scrollBar);
			this.Controls.Add(this._title);
			this.MouseUp += new MouseEventHandler(OnMouseUp);
			this.Name = "ChartCanvas";
			this.Size = new System.Drawing.Size(100, 100);
			this.ResumeLayout(false);

			_title.BringToFront();
		}
		#endregion

		public Label ComplementaryLabel {
			get {
				if(_complementaryLabel==null) {
					_complementaryLabel = new Label();
					_complementaryLabel.Visible = false;
					_complementaryLabel.Font = Env.Preference.DefaultFont;
					_complementaryLabel.TextAlign = ContentAlignment.MiddleLeft;
					_complementaryLabel.BorderStyle = BorderStyle.FixedSingle;
					_complementaryLabel.BackColor = Color.FromKnownColor(KnownColor.Info);
					_complementaryLabel.ForeColor = Color.FromKnownColor(KnownColor.InfoText);
					this.Controls.Add(_complementaryLabel);
				}
				return _complementaryLabel;
			}
		}
		public ChartTitle ChartTitle {
			get {
				return _title;
			}
		}

		public ChartDrawing DrawingEngine {
			get {
				return _drawing;
			}
		}

		public int BodyHeight {
			get {
				return this.Height - _scrollBar.Height;
			}
		}

		public void LoadBrand(AbstractBrand br, bool preserve_date) {
			int date = -1;
			if(preserve_date && _brand!=null) {
				DataFarm f = _brand.ReserveFarm();
				if(!f.IsEmpty) {
					Debug.Assert(_drawing.FirstDateIndex>=0 && _drawing.FirstDateIndex<f.TotalLength);
					if(_drawing.FirstDateIndex + Env.Layout.DisplayColumnCount >= f.TotalLength)
						date = -1; //�ŏI�f�[�^�������Ă���Ƃ��́u�Ōオ�����Ă���v��Ԃ��ێ�
					else
						date = f.GetByIndex(_drawing.FirstDateIndex).Date;
				}
			}

			_brand = br;
			AdjustScrollBar();
			if(_scrollBar.Enabled) {
				DataFarm f = br.ReserveFarm();
				int v = date==-1? GetTotalDataLength() - Env.Layout.DisplayColumnCount : f.DateToIndex(date);
				if(v<0) v = 0;
				if(v>_scrollBar.Maximum-_scrollBar.LargeChange+1) v = _scrollBar.Maximum-_scrollBar.LargeChange+1;
				_scrollBar.Value = v;
			}
			_drawing.SetBrand(br);

			//_title.InitUI();
		}
		public AbstractBrand GetBrand() {
			return _brand;
		}
		public int FirstDateIndex {
			get {
				return _drawing.FirstDateIndex;
			}
		}

		public void MoveToLatest() {
			AdjustScrollBar();
			int v = 0;
			if(_scrollBar.Enabled) v = GetTotalDataLength() - Env.Layout.DisplayColumnCount;
			if(v<0) v = 0;
			_scrollBar.Value = v;
			Invalidate(false);
		}
		public void SetDateIndex(int first, int cursor) {
			_scrollBar.Value = first;
			DataFarm f = _brand.ReserveFarm();
			if(f.IsEmpty)
				_drawing.UpdateDateLineIndex(-1);
			else
				_drawing.UpdateDateLineIndex(Math.Min(cursor, f.FilledLength-1));
			Invalidate(true);
		}
		public void ForceVisibleDate(int date, bool cut) {
		
			DataFarm f = _brand.ReserveFarm();
			if(f.IsEmpty) return;

			int index = f.DateToIndex(date);
			
			//�X�N���[�����Ȃ��ƌ����Ȃ�
			if(_drawing.FirstDateIndex>index || index>=_drawing.FirstDateIndex+Env.Layout.DisplayColumnCount || cut) {
				int nv = index - Env.Layout.DisplayColumnCount/2; //��̐^�񒆂ɕ\��
				if(nv<0) nv = 0;
				if(nv>=f.TotalLength) nv = f.TotalLength-1;
				_scrollBar.Value = nv;
			}
			_drawing.UpdateDateLineIndex(index);
			Invalidate(false);
		}

        //hacked by rti
        //���������I�������`�悷��
        public void DrawAutoTradingLine(int startDate, int endDate)
        {
            DataFarm f = _brand.ReserveFarm();
            if (f.IsEmpty) return;

            int startIndex = f.DateToIndex(startDate);
            int endIndex = f.DateToIndex(endDate);

            _drawing.UpdateDateAutoTradingLineIndex(startIndex, endIndex);
        }
        //---------------------------------



        public void ReloadFromPreference()
        {
			Preference pref = Env.Preference;
			Color bc = pref.BackBrush.Color;
			Color fc = pref.TextColor;
			this.BackColor = bc;
			this.ForeColor = fc;
			if(_complementaryLabel!=null) _complementaryLabel.Font = pref.DefaultFont;
			//InitOscillatorGroupUI();
			_drawing.ClearScale();
		}
		public void ResetLayout() {
			//_oscillatorGroup.Visible = Env.Options.ShowOscillator;
			AdjustScrollBar(); //����͉��i�яo�����̕\���ݒ肪�ω������Ƃ������ł��悢
			_drawing.ClearScale();
			_title.InitUI();
		}


		//���b�Z�[�W���������
		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			Preference pref = Env.Preference;
		}


		protected override void OnPaint(PaintEventArgs e) {
			base.OnPaint (e);
			if(this.DesignMode || _brand==null) return;

			_drawing.PaintMain(e.Graphics, e.ClipRectangle);
			if(_currentFreeLine!=null && _currentFreeLine.PivotHasEnoughDistanceTo(this.PointToClient(Control.MousePosition))) {
				IntPtr hdc = e.Graphics.GetHdc();
				_currentFreeLine.Draw(Env.Layout.ChartBodyRect, hdc);
				e.Graphics.ReleaseHdc(hdc);
			}
		}

		protected override void OnResize(EventArgs e) {
			base.OnResize (e);
			if(!this.DesignMode && _drawing!=null && _brand!=null && Env.Frame.WindowState!=FormWindowState.Minimized) {
				bool last_is_visible = !_scrollBar.Enabled || _scrollBar.Value+_scrollBar.LargeChange>=_scrollBar.Maximum;
				_drawing.ClearScale();
				AdjustScrollBar();
				
				this.SuspendLayout();
				Preference pref = Env.Preference;
				//_oscillatorGroup.Top = this.BodyHeight - _oscillatorGroup.Height - 8;
				//_oscillatorGroup.Left = this.Width - Env.Layout.RemarkAreaWidth+5;

				if(!_brand.ReserveFarm().IsEmpty && last_is_visible) { //�ŐV�f�[�^�����Ă���Ƃ��̃��T�C�Y�͂��̏�Ԃ��ێ�
					int v = _brand.ReserveFarm().TotalLength - Env.Layout.DisplayColumnCount;
					if(v<0) v = 0;
					_scrollBar.Value = v; //���̂܂�ValueChanged�C�x���g���U��
				}
				Invalidate(false);
				this.ResumeLayout();
			}
		}
		protected override void OnMouseMove(MouseEventArgs ev) {
			base.OnMouseMove (ev);
			if(_drawing==null) return;
			
			Preference pref = Env.Preference;
			//�B��Ă��܂�Indicator�e�L�X�g�̕\���E��\��
			Point p = new Point(ev.X, ev.Y);//this.PointToClient(Control.MousePosition);
			if(_complementaryLabel!=null && _complementaryLabel.Visible) {
				if(!new Rectangle(_complementaryLabel.Location, _complementaryLabel.Size).Contains(p))
					_complementaryLabel.Visible = false;
			}
			else {
				ArrayList ar = _drawing.TipEntries;
				if(ar!=null) {
					foreach(ComplementaryTextEntry e in ar) {
						if(e.rect.Contains(p)) {
							Label l = this.ComplementaryLabel;
							l.Location = new Point(e.rect.X-2, e.rect.Y-1);
							l.Size = new Size(e.rect.Size.Width+5, e.rect.Size.Height+2);
							l.Text = e.text;
							l.Visible = true;
							l.BringToFront();
							l.Invalidate();
							break;
						}
					}
				}
			}

			int offset = -1;
			if(p.X >= 0 && p.X < Env.Layout.ChartAreaWidth) {
				offset = (p.X-Env.Layout.CandleMiddleOffset) / Env.Layout.DatePitch;
				DataFarm farm = _brand.ReserveFarm();
				if(!farm.IsEmpty && _drawing.FirstDateIndex+offset>=farm.TotalLength) offset = -1;
			}

			//����
			if(Env.Preference.MouseTrackingLineMode==MouseTrackingLineMode.Full) {
				int my = _drawing.NormalizeByYobine(ev.Y);
				if(my!=_drawing.PriceLine._lastDrawn) {
					int h = Env.Layout.DefaultTextHeight;
					Rectangle yr = new Rectangle(0, _drawing.PriceLine._lastDrawn-h/2, this.Width, h);
					Invalidate(yr, false);
					yr = new Rectangle(0, my-h/2, this.Width, h);
					Invalidate(yr, false);
				}
				_drawing.PriceLine._nextToBeDrawn = my;
			}

			//�K�v�ȃG���A��Invalidate
			int ld = _drawing.DateLine._lastDrawn;
			if(offset==-1) {
				if(ld!=-1) {
					Rectangle r = Env.Layout.CurrentValueRect;
					Invalidate(r, false);
					int t = ld - _drawing.FirstDateIndex;
					r = new Rectangle((t-1)*Env.Layout.DatePitch, 0, Env.Layout.DatePitch*2, this.Height);
					Invalidate(r, false);
				}
				_drawing.UpdateDateLineIndex(-1);
			}
			else {
				if(_drawing.FirstDateIndex+offset!=ld) {
					Rectangle r = Env.Layout.CurrentValueRect;
					//r.X += ChartDrawing.VALUEWINDOW_HEADER_WIDTH;
					Invalidate(r, false);

                    //���i�яo����
					if(Env.Preference.ShowAccumulativeVolume)
	                    Invalidate(Env.Layout.AccumulativeVolumeRect, false);

                    //�߂̐��l�̂��Ƃ�����̂ōL�߂�Invalidate����
					if(ld!=-1) {
						int t = ld - _drawing.FirstDateIndex;
						//Debug.WriteLine(String.Format("prev offset={0}", t));
						r = new Rectangle((t-1)*Env.Layout.DatePitch, 0, Env.Layout.DatePitch*2, this.Height);
						Invalidate(r, false);
					}
					r = new Rectangle((offset-1)*Env.Layout.DatePitch, 0, Env.Layout.DatePitch*2, this.Height);
					Invalidate(r, false);
				}
				_drawing.UpdateDateLineIndex(_drawing.FirstDateIndex + offset);
			}

			//�������Ă��钼���̍ĕ`��
			if(_currentFreeLine!=null) {
				Invalidate(_currentFreeLine.GetInclusion(this.ClientRectangle), false);
				_currentFreeLine.Destination = p;
			}
			//�ڋ߂�����������΂�����N���A
			bool near_line_found = false;
			foreach(FreeLine line in _drawing.FreeLines) {
				double d = line.GetDistance(p);
				FreeLine.LineDrawMode m = d<3? FreeLine.LineDrawMode.Hilight : FreeLine.LineDrawMode.Normal;
				if(line.DrawMode!=m) {
					line.DrawMode = m;
					Invalidate(line.GetInclusion(this.ClientRectangle), false);
				}
				if(!near_line_found) near_line_found = m==FreeLine.LineDrawMode.Hilight;
			}
			this.Cursor = near_line_found? Cursors.Hand : Cursors.Default;
		}
		private void OnMouseUp(object sender, MouseEventArgs args) {
			if(args.Button==MouseButtons.Right) {
				ContextMenu m = Env.Frame.CreateContextMenu();
				m.Show(this, new Point(args.X, args.Y));
			}
			else if(args.Button==MouseButtons.Left) {
				if(_currentFreeLine!=null) {
					if(_drawing.FreeLineCount==10) {
						Util.Warning(Env.Frame, "���͂P�����ɂ�10�{�܂ł��������܂���");
						Invalidate();
					}
					else if(Env.FreeLines.Count==1000) {
						Util.Warning(Env.Frame, "���͑S����1000�{�܂ł��������܂���");
						Invalidate();
					}
					else if(_currentFreeLine.PivotHasEnoughDistanceTo(_currentFreeLine.Destination)) {
						_drawing.FixFreeLine(_currentFreeLine);
					}
					_currentFreeLine = null;
				}
				if(_drawing.RemoveHighlitedFreeLines()) Invalidate(); //�폜���ꂽ��������Invalidate
			}
		}



		protected override bool IsInputChar(char charCode) {
			return false;
		}

		private void AdjustScrollBar() {
			if(_brand==null || _brand.ReserveFarm().IsEmpty) {
				_scrollBar.Enabled = false;
			}
			else {
				Preference pref = Env.Preference;
				int total = GetTotalDataLength();
				int display = Env.Layout.DisplayColumnCount;
				if(display>=total) {
					_scrollBar.Enabled = false;
					_scrollBar.Value = 0;
					_drawing.FirstDateIndex = 0;
				}
				else {
					int v = _scrollBar.Value;
					_scrollBar.LargeChange = display;
					_scrollBar.Maximum = total-1;
					//Debug.WriteLine(String.Format("Adjusted disp={0} all={1} v={2}", display, all, v));
					if(v > _scrollBar.Maximum - display) _scrollBar.Value = _scrollBar.Maximum - display + 1;
					_scrollBar.Enabled = true;
				}
			}
		}
		private int GetTotalDataLength() {
			return _brand.ReserveFarm().TotalLength;
		}

		private void OnScrollBarValueChanged(object sender, EventArgs args) {
			if(this.DesignMode) return;
			int v = _scrollBar.Value;
			_drawing.FirstDateIndex = v;
			//Debug.WriteLine(String.Format("ValueChange max={0} lc={1} v={2}", _scrollBar.Maximum, _scrollBar.LargeChange, v));
			_drawing.ClearScale();
			Invalidate();
		}

		/*
		private void OnOscillatorGroupCheckedChanged(object sender, EventArgs args) {
			int i = _oscillatorGroupBoxes.IndexOf(sender);
			Debug.Assert(i!=-1);
			_drawing.OscillatorGroup = Env.CurrentIndicators.GetOscillatorGroupAt(i);
			Invalidate();
		}
		*/

		//FreeLine�֌W
		private FreeLine _currentFreeLine;

		protected override void OnMouseDown(MouseEventArgs e) {
			base.OnMouseDown (e);
			
			if(e.Button==MouseButtons.Left){
				if(_brand.ReserveFarm().IsEmpty) return;

				_currentFreeLine = new FreeLine(new Point(e.X, e.Y));
			}
		}		
		
		public void ClearCurrentFreeLine() {
			if(_currentFreeLine!=null) {
				Invalidate(_currentFreeLine.GetInclusion(Env.Layout.ChartBodyRect), false);
				_currentFreeLine = null;
			}
		}

		//DOJIMA�p�������C��
#if DOJIMA
		protected override void OnDoubleClick(EventArgs e) {
			base.OnDoubleClick (e);
			if(Env.CurrentIndicators.Format!=ChartFormat.HalfDaily) return;

			int index = _drawing.DateLine._lastDrawn;
			DailyDataFarm f = _brand.ReserveFarm() as DailyDataFarm;
			if(index<0 || index>=f.FilledLength) return;

			Dojima.ModifyHalfDayData dlg = new Dojima.ModifyHalfDayData();
			dlg.InitUI(f.GetByIndex(index), Dojima.DojimaUtil.HalfDailyDataFarmCache.Get(f), index);
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				Invalidate(true); //�S��ʍĕ`��
			}
		}
#endif

	}
}
