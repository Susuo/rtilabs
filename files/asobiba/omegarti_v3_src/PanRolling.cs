/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Threading;
using System.Collections;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.DataSource.Specialized
{
	internal class PanRollingTradeData {
		public int open;
		public int high;
		public int low;
		public int close;
		public string volume;
		public double volumeratio;
	}

	internal class PanRollingDataSource : DailyDataSource {

		public PanRollingDataSource(params int[] dates) : base(dates) {
		}

		private Hashtable _codes_thousandth;

		public override void Run() {
			Hashtable[] newdata = new Hashtable[_dates.Length];
			InitThousandthCodes();
			//�f�[�^���܂��_�E�����[�h
			for(int i=0; i<_dates.Length; i++)
				newdata[i] = FillData(_dates[i]);

			//�e�f�[�^�̒ǉ��ƕۑ�
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = (AbstractBrand)ie.Value;
				using(DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(_dates.Length)) {
					if(!f.IsEmpty) { 
						for(int i=0; i<_dates.Length; i++) {
							if(f.LastDate<_dates[i]) {
								PanRollingTradeData td = (PanRollingTradeData)newdata[i][br.Code];
								if(td==null) {
									if(br.Market!=MarketType.J)
										Debug.WriteLine("Data not found : code="+br.Code+" market="+br.Market.ToString());
								}
								else
									ExtendDataFarm(f, _dates[i], td);
							}
						}
						f.Save(Util.GetDailyDataFileName(br.Code));
					}
				}
				SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}
		}

		private void ExtendDataFarm(DailyDataFarm f, int date, PanRollingTradeData td) {
			if(td.volumeratio==0)
				td.volumeratio = GuessVolumeRatio(f, (double)f.GetByIndex(f.FilledLength-1).Close, td.volume);

			unsafe {
				f.WriteExtraData(0 , date);
				f.WriteExtraData(4 , td.open);
				f.WriteExtraData(8 , td.high);
				f.WriteExtraData(12, td.low);
				f.WriteExtraData(16, td.close);
				f.WriteExtraData(20, (int)(Double.Parse(td.volume)*td.volumeratio));
				f.ProgressExtraDataAddress();
			}
		}

		//������t�ɑ΂��āA�R�[�h��int�l����PanRollingTradeData�ւ̃n�b�V���e�[�u�����\�z���ĕԂ�
		private Hashtable FillData(int date) {
			TextReader r = null;
			try {
				Hashtable result = new Hashtable();
				string url = String.Format("http://www.panrolling.com/data/daily/s{0}.txt", date);
				r = new StreamReader(Util.HttpDownload(url), Encoding.Default);
				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split('\t');
					if(t.Length==7) { //�P�s�ڂɂ͓��t������悤��
						int code = Int32.Parse(t[0]);
						double v = 1;
						bool skip = false;
						//�R�[�h�̓���
						if(code==1001) { //���o����
							v = 100;
							code = (int)BuiltInIndex.Nikkei225;
						}
						else if(code==1002) { //TOPIX
							v = 100;
							code = (int)BuiltInIndex.TOPIX;
						}
						else if(code==1003) { //�h��
							code = (int)BuiltInIndex.JPYUSD;
						}
						else if(code==1008) //���[���H
							skip = true;
						else if(_codes_thousandth.Contains(code))
							v = 1000;
						else if(code==9861 || code==7615 || code==9409 || code==9479)
							v = 100;

						if(!skip) {
							PanRollingTradeData td = new PanRollingTradeData();
							td.volumeratio = 0;
							td.volume = t[6];
							//Debug.WriteLine(line);
							td.open = (int)(Double.Parse(t[2]) * v);
							td.high = (int)(Double.Parse(t[3]) * v);
							td.low  = (int)(Double.Parse(t[4]) * v);
							td.close= (int)(Double.Parse(t[5]) * v);
							result[code] = td;
						}
					}
					line = r.ReadLine();
				}
				return result;
			}
			finally {
				if(r!=null) r.Close();
			}
		}

		//���i�����ۂ�1/1000�ɏ�����Ă���������Z�b�g
		private void InitThousandthCodes() {
			_codes_thousandth = new Hashtable();
			int[] data = new int[] {
				4740, 4741, 4753, 4795, 6829, 7717, 9442, 8421, 4788, 4759,
				6786, 2656, 4763, 4771, 4777, 2665, 9444, 4765, 4697, 4764, 
				7589, 4792, 4797, 4800, 4744, 6790, 4776, 4751, 2652, 4815, 
				4824, 4747, 2667, 4787, 2681, 4784, 4814, 4742, 9445, 4794, 
				4779, 4821, 4818, 7641
									};
			foreach(int t in data)
				_codes_thousandth.Add(t, null);
		}
				

		//PanRolling�̃f�[�^�͏o�����\�L���܂��܂��B1/10/100/1000�{�̂����ꂩ������̂����A������������肷��
		private double GuessVolumeRatio(DataFarm farm, double lastprice, string expr) {
			TradeData last = farm.GetByIndex(farm.FilledLength-1);
			double min = 1;
			int period = expr.IndexOf('.');
			if(period>0) {
				if(period==expr.Length-4) return 1000; //�����ȉ��R�ʂ܂ł���Ζ��炩
				
				if(period==expr.Length-3) min = 100;
				else if(period==expr.Length-2) min = 10;
			}

			double l = Math.Log10(Double.Parse(expr) / last.Volume);
			double cand;
			if(l<-2.5)
				cand = 1000;
			else if(l<-1.5)
				cand = 100;
			else if(l<-0.5)
				cand = 10;
			else
				cand = 1;

			cand = Math.Max(cand, min);

			if(0.9<=(lastprice/last.Close) && (lastprice/last.Close)<=1.1) {
				//�l�����������ʂ͈̔͂ɓ����Ă���Ȃ�A���̒l��M�p����
				return cand;
			}
			else {
				//�傫�������ďo�������}�������Ƃ��́A�{�������������肷�邩������Ȃ��B
				int t1 = Math.Min(GuessTanniKabu(last.Volume), GuessTanniKabu(last.Prev.Volume));
				do {
					int vol = (int)(Double.Parse(expr) * cand);
					int t2 = GuessTanniKabu(vol);
					if(t1==t2)
						return cand;
					else {
						Debug.WriteLine("GuessRatio rare case! code=" + farm.Brand.Code.ToString());
						cand *= 10;
					}
				} while(true);
			}

		}

		//�o��������P�ʊ��𐄑�
		private static int GuessTanniKabu(double vol_) {
			int vol = (int)vol_;
			if(vol % 10 != 0) return 1;
			else if(vol % 100 != 0) return 10;
			else if(vol % 1000 != 0) return 100;
			else return 1000;
		}

		//���i�E�o�����␳�̂��߂̃e�X�g�v���O����
		public static void Test() {
			StreamReader pan = new StreamReader("C:\\zanetti\\doc\\pan.csv");
			StreamReader iticker = new StreamReader("C:\\zanetti\\doc\\iticker.csv");
			string pl = pan.ReadLine();
			while(pl!=null) {
				string[] rv = pl.Split(',');
				int code = Int32.Parse(rv[0]);
				double value1 = Double.Parse(rv[1]);
				double volume1 = Double.Parse(rv[2])*1000;

				string il;
				string[] sv;
				do {
					il = iticker.ReadLine();
					sv = il.Split(',');
				} while(code!=Int32.Parse(sv[0]));

				double value2 = Double.Parse(sv[1]);
				double volume2 = Double.Parse(sv[2]);
				if(value1!=value2 || volume1!=volume2) {
					if(value2==0 || volume2==0)
						Debug.WriteLine(String.Format("{0} ZERO", code));
					else
						Debug.WriteLine(String.Format("{0},{1},{2}", code, value1/value2, volume1/volume2));
				}

				pl = pan.ReadLine();
			}
		}

	}
}
