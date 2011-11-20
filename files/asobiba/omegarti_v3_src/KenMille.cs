/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 * 
 */
using System;
using System.Text;
using System.IO;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.DataSource.Specialized
{
	internal class KenMilleDataSource : FullDataSource {

		public KenMilleDataSource(CodeEnumerator ce) : base(ce) {
		}
		public override byte[] DownloadFullData(BasicBrand br) {
			MemoryStream s = null;
			try {
				int code = br.Code;
				s = Util.HttpDownload(FormatDailyTimeSeriesURL(code, br.Market));
				s.Close();
				return s.ToArray();
			}
			finally {
				if(s!=null) s.Close();
			}
		}

		public override void ImportFullData(BasicBrand br, byte[] buf) {
			//Debug.WriteLine("DL START");
			FileStream d = null;
			int code = br.Code;
			string filename = Util.GetDailyDataFileName(code);
			bool success = false;
			try {
				d = new FileStream(filename, FileMode.Create);
				int records = buf[0] + (buf[1]*256); //�ŏ��̂Q�o�C�g�Ń��R�[�h���������Ă���
                if(records > 10000) throw new FormatException("���R�[�h�����ُ�ł�");

                int offset = 4;
				bool body_found = false;
				if(!VolumeIsAvailable(code)) body_found = true; //�o�����f�[�^���Ȃ��Ƃ킩���Ă���z�͍ŏ�����true
				if(code==(int)BuiltInIndex.Nikkei225 || code==(int)BuiltInIndex.TOPIX) body_found = true; //���o���ρATOPIX�͉ߋ��f�[�^�ɏo�����O�̃]�[��������
				for(int i=0; i<records; i++) {
					//2792�ȂǁA�擪�ɋ�f�[�^�̓����Ă��邢��炵�����������݂���
					if(!body_found) {
						body_found = buf[offset+24]!=0; //�o�������݂�
                        //�ŏ��Ɍ��������ʒu�ŕs���ȓ��t�f�[�^�Ȃ�x��
                        TestDateFormat(buf, offset);
                    }

                    if(body_found) {
						Debug.Assert(offset + 36 <= buf.Length);
						d.Write(buf, offset+0,  20);
						d.Write(buf, offset+24, 12);
					}
					offset += 36;
				}
				success = true;
			}
			finally {
				if(d!=null) d.Close();
				//Debug.WriteLine("DL END");
				if(!success && File.Exists(filename)) File.Delete(filename);
			}
		}

		private static string FormatDailyTimeSeriesURL(int code, MarketType mt) {
			if(mt==MarketType.B) code = MarketTypeToFileName((BuiltInIndex)code);
			string t = code<1000? "0"+(code.ToString()) : code.ToString();
			
			string post = "";
			if(mt==MarketType.O1 || mt==MarketType.O2) post = "o";
			else if(mt==MarketType.H) post = "n";
			else if(mt==MarketType.M) post = "z";

			/*
			 * �Â��d�l
			return String.Format("http://www.miller.co.jp/member/chart/D{0}/D{1}{2}", t[0], t, post);
			*/

			//�K�[�h���{���ꂽ���Ƃ̎d�l�@�K�[�h�̈Ӑ}����������Ƃ��������ŁA���ۂ�Java�A�v���b�g�̒���������Ɣ`����OK
			return String.Format("http://www.miller.co.jp/kmp00/visitor/apps/cgi-bin/cv0cgt10c.cgi?type=1&q={0}{1}&dw=1&password={2}", t, post, FormatKey(code));
		}
		private static string FormatKey(int code) {
			StringBuilder b = new StringBuilder();

			if(code < 1000) code += 1000;
			//string s2 = ((code + 3352) * 23 + 0x11ae5).ToString(); 
			string s2 = ((code + Env.Options.KenmilleKey) * 23 + 0x100d9).ToString();
			b.Append(s2.Substring(2, 1));
			b.Append("Xe");
			b.Append(s2.Substring(2, 1));
			b.Append("e+");
			b.Append(s2.Substring(1, 2));
			b.Append("vw");
			b.Append(s2.Substring(3, 2));
			b.Append("f");
			b.Append(s2.Substring(2, 1));
			b.Append("3a");
			return b.ToString();
		}

		private static int MarketTypeToFileName(BuiltInIndex m) {
			switch(m) {
				case BuiltInIndex.Nikkei225:
					return 100;
				case BuiltInIndex.TOPIX:
					return 105;
				case BuiltInIndex.JASDAQ:
					return 115;
				case BuiltInIndex.Nikkei225_F:
					return 101;
				case BuiltInIndex.TOPIX_F:
					return 106;
				case BuiltInIndex.JPYUSD:
					return 500;
				case BuiltInIndex.JPYEUR:
					return 501;
				case BuiltInIndex.Dow:
					return 200;
				case BuiltInIndex.Nasdaq:
					return 202;
				case BuiltInIndex.SP500:
					return 201;
				default:
					throw new ArgumentException("unexpected index " + m);
			}
		}
		private static bool VolumeIsAvailable(int code) {
			//���݁A�ʉ݊֌W�͏o�����f�[�^�Ȃ�
			return !(200<=code && code<400);
		}

        private static void TestDateFormat(byte[] buf, int offset) {
            unsafe {
                fixed(byte* p = &buf[offset]) {
                    int t = *(int*)p;
					if(t==0) return; //0�������Ă���̂͂��܂ɂ���B�������B
                    if(t < 19760101 || t > 21000101) throw new FormatException("���t�t�H�[�}�b�g���s���ł��B");
                }
            }
        }
    }
}
