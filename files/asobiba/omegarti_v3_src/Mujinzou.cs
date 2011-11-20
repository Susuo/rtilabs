/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Collections;
using System.Diagnostics;


using Zanetti.Data;
using Travis.Archive;
using Travis.Http;
using Zanetti.CodeExtendData;

namespace Zanetti.DataSource.Specialized {
	internal class MujinzouDataSource : DailyDataSource {

		private bool _includesDomesticIndices;
        private bool _IsFundamentalDownload = false;

		public MujinzouDataSource(int[] dates) : base(dates) {
		}

		public bool IncludesDomesticIndices {
			get {
				return _includesDomesticIndices;
			}
			set {
				_includesDomesticIndices = value;
			}
		}
        public bool IsFundamentalDownload
        {
            get
            {
                return _IsFundamentalDownload;
            }
            set
            {
                _IsFundamentalDownload = value;
            }
        }


		public override void Run() {

            //hacked by rti
            //�Ɛя��
            ArrayList newCodeExtendsDataGK = new ArrayList();   //��Ќ��Z���\
            ArrayList newCodeExtendsDataG = new ArrayList();   //�ƐяC��
            //------------------------------------

			Hashtable[] newdata = new Hashtable[_dates.Length];
			//�f�[�^���܂��_�E�����[�h
			for(int i=0; i<_dates.Length; i++) {
				newdata[i] = FillData(_dates[i]);
                //hacked by rti
                //�Ɛя����_�E�����[�h
                if (this._IsFundamentalDownload)
                {
                    if (IsDownloadExtendsData(_dates[i]))
                    {
                        newCodeExtendsDataGK.Add(DownloadExtendsDataGK(_dates[i]));
                        newCodeExtendsDataG.Add(DownloadExtendsDataG(_dates[i]));
                    }
                }
                //------------------------
                SendMessage(AsyncConst.WM_ASYNCPROCESS, (_dates[i] & DataSourceBase.DATE_MASK), AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}

			//�e�f�[�^�̒ǉ��ƕۑ�
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = (AbstractBrand)ie.Value;
				if((br.Market==MarketType.B && !IsMujinzouSupportedIndices(br.Code)) || br.Market==MarketType.Custom) continue;
			
				bool trace_flag = false;
				using(DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(_dates.Length)) {
					for(int i=0; i<_dates.Length; i++) {
						NewDailyData td = (NewDailyData)newdata[i][br.Code];
						if(td==null) {
							if(!trace_flag) {
								trace_flag = true;
								Debug.WriteLine("Data not found(mujinzou) : code="+br.Code+" market="+br.Market.ToString());
							}
						}
						else
							f.UpdateDataFarm(_dates[i], td);
					}
                    //hacked by rti
                    //���Z���(GK) 
                    foreach (ArrayList dates in newCodeExtendsDataGK)
                    {//GK��
                        foreach (CodeExtendData.DataAccounts da in dates)
                        {
                            if (da.Code == br.Code)
                            {
                                f.CodeExtend.Update(da);
                            }
                        }
                    }
                    //�ƐяC��(G) 
                    foreach (ArrayList dates in newCodeExtendsDataG)
                    {
                        foreach (CodeExtendData.DataRevised dr in dates)
                        {
                            if (dr.Code == br.Code)
                            {
                                f.CodeExtend.Update(dr);
                            }
                        }
                    }
                    //�g�����̕ۑ�
                    f.CodeExtend.Write(br.Code);
                    //----------------------------------
					f.Save(Util.GetDailyDataFileName(br.Code));
				}
				SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}
		}

        //������t�ɑ΂��āA�R�[�h��int�l����PanRollingTradeData�ւ̃n�b�V���e�[�u�����\�z���ĕԂ�
        private Hashtable FillData(int date)
        {
			TextReader r = null;
			try {
				Hashtable result = new Hashtable();
				DateTime d2 = Util.IntToDate(date);
                //���s���ɂ̓f�[�^���Q��ނ���A���o�敨���̗L���A���ؖ����̗L���A�o�����O�̂Ƃ��̈����A���قȂ�悤���B�敨�̃f�[�^���Ƃ��悤�ɂ�������g�p����
                //2007/3/1, eonet����̃f�[�^������s�\�ɂȂ�B�W����infoseek�ɐ؂�ւ���ƂƂ��ɁA���łɐݒ�t�@�C���Ő؂�ւ��\�ɂ����B���ꂪ��r��̎����ɖʓ|���������Ƃ��N�������̂��B
                //2007/3/31, �����infoseek���ړ�
                string url = Env.Options.MujinzouUsesKakoData ?
                    String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/T{3:D2}{4:D2}{5:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month, d2.Day) :
                    String.Format("http://souba-data.com/data_day/{0}d/{1:D2}_{2:D2}d/T{3:D2}{4:D2}{5:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month, d2.Day);
                //hacked by rti
                //r = new StreamReader(ExtractData(url), Encoding.Default);
                Stream st = ExtractData(url);
                if (st == null)
                {
                    //�t�@�C�����Ȃ�����!!
                    return new Hashtable();
                }
                r = new StreamReader(st, Encoding.Default);
                //------

				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split(',');
					//��      0    1  2             3     4     5     6     7,         8,       9
					//2005/5/20,1001,11,1001 ���o����,11104,11110,11034,11037,1257840000,���؂P��
					if(t.Length==10 && t[1].Length==4) {
						int code = ParseCode(t[1]);

						BasicBrand br = Env.BrandCollection.FindBrand(code) as BasicBrand;
						if(br!=null && CheckMarket(br, Util.ParseInt(t[2], 0))) {

							double vv = 1, pv = 1;
							//�{������
							if(IsDomesticIndex(code)) {
								vv = 0.001; //DreamVisor�̂��̂ɂ��킹��i�D�ŁB1000���P�ʂ���
								pv = 100;
							}
							if(code==(int)BuiltInIndex.TOPIX_F) { //TOPIX�敨�͐����P�ʂŋL�^����Ă���
								pv = 10;
							}

							NewDailyData td = new NewDailyData();
							td.volume = (int)(Double.Parse(t[8]) * vv);
							NewDailyData existing = (NewDailyData)result[code];
							//Debug.WriteLine(line);
							td.open = (int)(Double.Parse(t[4]) * pv);
							td.high = (int)(Double.Parse(t[5]) * pv);
							td.low  = (int)(Double.Parse(t[6]) * pv);
							td.close= (int)(Double.Parse(t[7]) * pv);
							result[code] = td;
						}
					}
					line = r.ReadLine();
				}
				return result;
			}
            finally
            {
				if(r!=null) r.Close();
			}
		}

        //hackedby rti
//        private static Stream ExtractData(string url)
//        {
//            MemoryStream ms = Util.HttpDownload(url);
//            ms.Close();
//            byte[] input = ms.ToArray();
//            ArrayList list = new ExtractLha().extract(input);
//            return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
//        }
        private static Stream ExtractData(string url)
        {
            MemoryStream ms = null;
            try
            {
                ms = Util.HttpDownload(url);
                ms.Close();
            }
            catch (System.Net.WebException webException)
            {
                //404Not Found?
                //�[���A��O���΂��Ȃ� 404�̒ʒm������Ă���� >> �r��
                //�f�o�b�K�ŗ�O���Ď�����Ɩʓ|�ɂȂ����Ȃ����B
                //���������A��O�Ƃ����̂́A�{���ɗ�O�̎��ӊO�ɂ͔�΂�����_�������ă}�}���狳���Ȃ���������
                if (webException.Status == System.Net.WebExceptionStatus.ProtocolError &&
                    ((System.Net.HttpWebResponse)webException.Response).StatusCode == System.Net.HttpStatusCode.NotFound)
                {
                    return null;
                }
            }
            //���s���̈ړ]��� 404�̎��� 404�̃y�[�W�� 200 OK �ŕԂ��Ă����f�G�d�l�Ȃ̂őΉ�����.
            //C# ���ă�������r�Ƃ��ǁ[���̂��A�悭�킩���̂œK���ɂ���Ă݂�
            byte[] input = ms.ToArray();
            string a = Encoding.ASCII.GetString(input);
            if (Encoding.ASCII.GetString(input).IndexOf("File Not Found") != -1 )
            {
                return null;
            }
            
            ArrayList list = new ExtractLha().extract(input);
            return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
        }
        //--------------

        private static bool CheckMarket(BasicBrand br, int muj)
        {
            MarketType mt = br.Market;
            switch (mt)
            {
                //2006/12/26�̃f�[�^���A���؂͂��ׂăR�[�h11, ��؂P�E�Q����21, �w���N���X��23�ɕύX�ɂȂ����B���f�[�^��ǂ񂾂Ƃ��ł����v�Ȃ悤�ɌÂ�������c���Ă���
                case MarketType.B:
                    return muj == 11; //�w���͓��؈ꕔ�Ƃ��ċL�^����Ă���
                case MarketType.T1:
                    return muj == 11;
                case MarketType.T2:
                    return muj == 11 || muj == 12;
                case MarketType.O1:
                    return muj == 21;
                case MarketType.O2:
                    return muj == 21 || muj == 22;
                case MarketType.M:
                    return muj == 21 || muj == 13;
                case MarketType.J:
                    return muj == 91;
                case MarketType.H:
                    return muj == 23 || muj == 24;
                default:
                    return false;
            }
        }

        private static bool IsMujinzouSupportedIndices(int code)
        {
			return code==(int)BuiltInIndex.Nikkei225 ||
				code==(int)BuiltInIndex.TOPIX ||
				code==(int)BuiltInIndex.JASDAQ ||
				code==(int)BuiltInIndex.Nikkei225_F ||
				code==(int)BuiltInIndex.TOPIX_F;
		}
		private static int ParseCode(string code) {
			int t = Util.ParseInt(code);
			//�ȉ��ɊY��������̂łȂ���΃f�[�^�̎�荞�݂͍s���Ȃ�
			if(t<1300) {
				switch(t) {
					case 1001: return (int)BuiltInIndex.Nikkei225;
					case 1002: return (int)BuiltInIndex.TOPIX;
					case 1004: return (int)BuiltInIndex.Nikkei225_F;
					case 1005: return (int)BuiltInIndex.JASDAQ;
					case 1006: return (int)BuiltInIndex.TOPIX_F;
					default: return 0;
				}
			}
			else
				return t; //�ӂ��̖���
		}
		private static bool IsDomesticIndex(int code) {
			return code==(int)BuiltInIndex.Nikkei225 || code==(int)BuiltInIndex.TOPIX || code==(int)BuiltInIndex.JASDAQ;
		}

        //hacked by rti
        //�g���f�[�^���_�E�����[�h����ׂ���?
        private bool IsDownloadExtendsData(int inDate)
        {
            //���̍Ōゾ������擾����.
            DateTime d = Util.IntToDate(inDate);
            if (d.AddDays(1).Day == 1)
            {
                return true;
            }

            //�擾������Ԃ̍Ō�̓���������A�擾����.
            if (_dates[_dates.Length - 1] == inDate)
            {
                return true;
            }

            //�܂��擾���Ȃ��Ă�����
            return false;
        }

        //���Z���
        private ArrayList DownloadExtendsDataGK(int inDate) //GK��
        {
            Debug.Assert(IsDownloadExtendsData(inDate) == true);

            ArrayList result = new ArrayList();
            DateTime d2 = Util.IntToDate(inDate);

            string url = String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/GK{3:D2}{4:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month);
            //�_�E�����[�h xls���g��W�J.
            ArrayList al = ExtractXLSData(url);
            if (al == null)
            {
                return result;
            }

            //xls�̓��e���N���X�Ɋ���U��
            for (int i = 1; i < al.Count; i++)
            {
                ArrayList line = (ArrayList)al[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }

                //�ȉ��̏���
                //0���t	1�R�[�h	2�s��敪	3������	4�O�����Z	5���Z�敪	6�O������i�S���j	7�o�험�v�i�S���j	8���v�S���j	9�ꊔ�v�i�K�j	10�z�����i�K�j	�������Z	���Z�敪	�O������i�S���j	�o�험�v�i�S���j	���v�@�@�i�S���j	�ꊔ�v�i�K�j	�z�����i�K�j	�������Z	���Z�敪	�O������i�S���j	�o�험�v�i�S���j	���v�@�@�i�S���j	�ꊔ�v�i�K�j	�z�����i�K�j	�ꊔ�����Y�i�K�j
                CodeExtendData.DataAccounts ext = new CodeExtendData.DataAccounts();
                ext.Code = Util.atoi((string)line[1]);
                ext.Date = Util.StringDateToInt((string)line[0]);
                ext.Kubun = ((string)line[5] == "1") ? 1 : 3;    //���s���� 1 �������璆��   2 ��������{���Z
                ext.ZenkiKettusan = Util.StringDateToInt((string)line[4]);       //�O�����Z��
//                    ext.ZenkiUriage = Util.atoi((string)line[6]);         //�O������(�S���~)
//                    ext.ZenkiKeijyourieki = Util.atoi((string)line[7]);   //�O���o�험�v(�S���~)
//                    ext.Zenkirieki = Util.atoi((string)line[8]);          //�O�����v(�S���~)
//                    ext.ZenkiHitoikabueki = Util.atoi((string)line[9]);   //�O���ꊔ�v(�K)
//                    ext.ZenkiHaitoukin = Util.atoi((string)line[10]);      //�O���z����(�K)
                ext.KonkiKettusan = Util.StringDateToInt((string)line[11]);       //�������Z
                ext.KonkiUriage = Util.atoi((string)line[13]);         //��������(�S���~)
                ext.KonkiKeijyourieki = Util.atoi((string)line[14]);   //�����o�험�v(�S���~)
                ext.Konkirieki = Util.atoi((string)line[15]);          //�������v(�S���~)
                ext.KonkiHitoikabueki = Util.atoi((string)line[16]);   //�����ꊔ�v(�K)
                ext.KonkiHaitoukin = Util.atoi((string)line[17]);      //�����z����(�K)
                ext.JikiKettusan = Util.StringDateToInt((string)line[18]);        //�������Z
                ext.JikiUriage = Util.atoi((string)line[20]);          //��������(�S���~)
                ext.JikiKeijyourieki = Util.atoi((string)line[21]);    //�����o�험�v(�S���~)
                ext.Jikirieki = Util.atoi((string)line[22]);           //�������v(�S���~)
                ext.JikiHitoikabueki = Util.atoi((string)line[23]);    //�����ꊔ�v(�K)
                ext.JikiHaitoukin = Util.atoi((string)line[24]);       //�����z����(�K)
                ext.Hitokabujyunsisan = Util.atoi((string)line[25]);   //�ꊔ�����Y

                result.Add(ext);
            }
            al = null;
            return result;
        }
        //�ƐяC��
        private ArrayList DownloadExtendsDataG(int inDate)
        {
            Debug.Assert(IsDownloadExtendsData(inDate) == true);

            ArrayList result = new ArrayList();
            DateTime d2 = Util.IntToDate(inDate);

            string url = String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/G{3:D2}{4:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month);
            //�_�E�����[�h xls���g��W�J.
            ArrayList al = ExtractXLSData(url);
            if (al == null)
            {
                return result;
            }

            //xls�̓��e���N���X�Ɋ���U��
            for (int i = 2; i < al.Count; i++)
            {
                ArrayList line = (ArrayList)al[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }

                //0���\��	1�R�[�h	2������	3���Z�敪	4�ϑ�	5���Z��	6�C���O���㍂	7���㍂�i�S���~�j8�ω����i���j9�c�Ɖv�i�S���~�j	10�C���O�o��v	11�o��v�i�S���~�j12�ω����i���j	13�C���O�ň��v	14�ň��v�@�i�S���~�j	15�ω����i���j	16�ꊔ 17�C���O�z��	18�z���@�@�i�~�j	�ω����i���j	�����Y	�E�v	�s��																																																																																																																																																																																																																																									
                CodeExtendData.DataRevised ext = new CodeExtendData.DataRevised();
                ext.Code = Util.atoi((string)line[1]);
                ext.Date = Util.StringDateToInt((string)line[0]);
                ext.SyuseimaeUriage = Util.atoi((string)line[6]);           //�C���O����
                ext.Uriage = Util.atoi((string)line[7]);                   //����
                ext.SyuseimaeKeijyourieki = Util.atoi((string)line[10]);           //�C���O�o�험�v
                ext.Keijyourieki = Util.atoi((string)line[11]);                    //�o�험�v
                ext.SyuseimaeZeibikieki = Util.atoi((string)line[13]);           //�C���O�ň��v
                ext.Zeibikieki = Util.atoi((string)line[14]);                    //�ň��v
                ext.SyuseimaeHaitou = Util.atoi((string)line[17]);           //�C���O�z��
                ext.Haitou = Util.atoi((string)line[18]);                    //�z��

                //�P�̌��Z�Ȃ�A�����Z���Ȃ����T���A�A����ΒP�͖̂�������A���傠����
                if (ExtendsDataGIsTantai((string)line[3]))
                {
                    //�ŁA�A���͂���킯?
                    if (ExtendsDataGFindRenketu(al, i + 1, ext.Code))
                    {//�A���A���a�����A���ᖳ����
                        continue;
                    }
                }

                result.Add(ext);
            }
            al = null;
            return result;
        }
        //�P��
        private bool ExtendsDataGIsTantai(string inLine)
        {
            return inLine.IndexOf("�P") != -1;
        }
         //�ƐяC��
        private bool ExtendsDataGFindRenketu(ArrayList inCsv , int inSearchLine , int inCode)
        {
            for (int i = inSearchLine ; i < inCsv.Count; i++)
            {
                ArrayList line = (ArrayList)inCsv[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }
                if (inCode == Util.atoi((string)line[1]))
                {
                    if (! ExtendsDataGIsTantai( (string) line[3] ) )
                    {
                        //�A��������!!
                        return true;
                    }
                }
            }
            return false;
        }
        
        //lzh�ň��k����Ă���f�[�^���_�E�����[�h���āA xls�t�@�C����ǂݍ��݂܂�
        private static ArrayList ExtractXLSData(string url)
        {
            MemoryStream ms = null;
            try
            {
                ms = Util.HttpDownload(url);
                ms.Close();
            }
            catch (System.Net.WebException webException)
            {
                //404Not Found?
                //�[���A��O���΂��Ȃ� 404�̒ʒm������Ă���� >> �r��
                //�f�o�b�K�ŗ�O���Ď�����Ɩʓ|�ɂȂ����Ȃ����B
                //���������A��O�Ƃ����̂́A�{���ɗ�O�̎��ӊO�ɂ͔�΂�����_�������ă}�}���狳���Ȃ���������
                if (webException.Status == System.Net.WebExceptionStatus.ProtocolError &&
                    ((System.Net.HttpWebResponse)webException.Response).StatusCode == System.Net.HttpStatusCode.NotFound)
                {
                    return null;
                }
            }
            //���s���̈ړ]��� 404�̎��� 404�̃y�[�W�� 200 OK �ŕԂ��Ă����f�G�d�l�Ȃ̂őΉ�����.
            //C# ���ă�������r�Ƃ��ǁ[���̂��A�悭�킩���̂œK���ɂ���Ă݂�
            byte[] input = ms.ToArray();
            if (Encoding.ASCII.GetString(input).IndexOf("File Not Found") != -1)
            {
                return null;
            }

            ArrayList list = new ExtractLha().extract(input);
            
            //�W�J�����o�b�t�@���e���|�����ɏ�������.
            //ADO���ă�������̃G�N�Z���f�[�^�ɑ΂��Ă��L���Ȃ̂�?? �悭�킩���̂ň�x�t�@�C�����o�R����.
   			string tmpfn = Path.GetTempFileName();

			FileStream fst = File.OpenWrite(tmpfn);
            BinaryWriter bw = new BinaryWriter(fst);
            bw.Write( ((Extract.FileBuffer)list[0]).buffer );
			bw.Close();
			fst.Close();

            try
            {
                Hashtable xls = CodeExtendData.UtilX.XLSReader(tmpfn);

                //�ŏ��̃y�[�W��Ԃ�.
                foreach (string key in xls.Keys)
                {
                    return (ArrayList)xls[key];
                }
            }
            finally
            {
			    File.Delete(tmpfn);
            }
            return null;
        }
        //-------------------------
	}
}
