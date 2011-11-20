using System;
using System.Text;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Text.RegularExpressions;

using Zanetti.Data;
using Travis.Archive;
using Travis.Http;
using Zanetti.CodeExtendData;

namespace Zanetti.DataSource.Specialized
{
    internal class YahooDataSource : DailyDataSource {

        //���p�~�����̃_�E�����[�h���s��Ȃ�
        public bool IsIgnoreObsolete = true;

        public YahooDataSource(int[] dates)  : base(dates)
        {
		}

        public override void Run() {

            //�_�E�����[�h�J�n���ƏI����
            int startDate = GetStartDate(this._dates);
            int endDate = GetEndDate(this._dates);

            Hashtable newdata = new Hashtable();

            //�f�[�^���_�E�����[�h
            IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
            while (ie.MoveNext())
            {
                AbstractBrand br = (AbstractBrand)ie.Value;
                if ( br.Market == MarketType.Custom) continue;
                if (this.IsIgnoreObsolete)
                {//���p�~�͖�������ݒ�̏ꍇ
                    if (br is BasicBrand && ((BasicBrand)br).Obsolete == true)
                    {
                        continue;
                    }
                }

                if (endDate - startDate > 20)
                {//����܂�Ԃ��󂭂Ƃ������ŏc�f�������Ȃ��Ƃ����Ȃ�...
                    newdata[br.Code] = DownloadOldDate(br, startDate, endDate);
                }
                else
                {//����ȂɊԂ��󂢂Ă��Ȃ��Ȃ�AYahoo!�ɕ��ׂ�������Ȃ��Ǝv������@��
                    newdata[br.Code] = DownloadCurrentDate(br);
                }

                SendMessage(AsyncConst.WM_ASYNCPROCESS, (startDate & DataSourceBase.DATE_MASK), AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
            }

            //�f�[�^���C���|�[�g
            
            ie = Env.BrandCollection.GetEnumerator();
            while (ie.MoveNext())
            {
                AbstractBrand br = (AbstractBrand)ie.Value;
                if (br.Market == MarketType.Custom) continue;

                Hashtable tr = (Hashtable)newdata[br.Code];
                if (tr == null)
                {
                    continue;
                }
                ArrayList dates = new ArrayList(tr.Keys);
                dates.Sort();

                bool trace_flag = false;
                using (DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(dates.Count ))
                {
                    foreach(int day in dates)
                    {
                        NewDailyData td = (NewDailyData)tr[day];
                        if (td == null)
                        {
                            if (!trace_flag)
                            {
                                trace_flag = true;
                                Debug.WriteLine("Data not found(yahoo) : code=" + br.Code + " market=" + br.Market.ToString());
                            }
                        }
                        else
                            f.UpdateDataFarm(day, td);
                    }
                    f.Save(Util.GetDailyDataFileName(br.Code));
                }
                SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
            }

        }

        //�J�n������
        int GetStartDate(int[] inDateArray)
        {
            Debug.Assert(inDateArray.Length >= 1);

            int r = inDateArray[0];

            for (int i = 1; i < inDateArray.Length; i++)
            {
                if (r > inDateArray[i])
                {
                    r = inDateArray[i];
                }
            }
            return r;
        }
        //�I��������
        int GetEndDate(int[] inDateArray)
        {
            Debug.Assert(inDateArray.Length >= 1);

            int r = inDateArray[0];

            for (int i = 1; i < inDateArray.Length; i++)
            {
                if (r < inDateArray[i])
                {
                    r = inDateArray[i];
                }
            }
            return r;
        }
        //�R�[�h��yahoo�`���Ŏ擾����  5401 -> 5401.t
        //���Ή��̏ꍇ �󔒂ɂȂ�
        string GetYahooCode(int inCode, MarketType inType)
        {
            string codeString = inCode + ".";
            switch (inType)
            {
                case MarketType.T1:
                case MarketType.T2:
                    codeString += "t";
                    break;
                case MarketType.O1:
                case MarketType.O2:
                    codeString += "o";
                    break;
                case MarketType.J:
                    codeString += "q";
                    break;
                case MarketType.M:
                    codeString += "t";
                    break;
                case MarketType.H:
                    codeString += "j";
                    break;
                case MarketType.B:
                    switch (inCode)
                    {
                        case (int)BuiltInIndex.Nikkei225:   //���o����
                            codeString = "998407.O";
                            break;
                        case (int)BuiltInIndex.TOPIX:   //TOPIX
                            codeString = "998405.T";
                            break;
                        case (int)BuiltInIndex.JASDAQ:   //JASDAQ
                            codeString = "23337.Q";
                            break;
                        case (int)BuiltInIndex.Nikkei225_F:   //���o���ϐ敨
                            codeString = "5040469.O";
                            break;
                        default:
                            //���Ή�
                            codeString = "";
                            break;
                    }
                    break;
                default:
                    //���Ή�
                    codeString = "";
                    break;
            }

            return codeString;
        }

        //�Â��f�[�^���_�E�����[�h
        //Hashtable[����] = NewDailyData �`��
        Hashtable DownloadOldDate(AbstractBrand inBR, int inStartDate, int inEndDate)
        {
            DateTime d2 = Util.IntToDate(inStartDate);
            DateTime e2 = Util.IntToDate(inEndDate);
            string codeString = GetYahooCode(inBR.Code , inBR.Market);
            int count = 0;

            Hashtable ret = new Hashtable();
            if (codeString == "")
            {
                return ret; //���Ή�
            }

            while (true)
            {
                string url = String.Format("http://table.yahoo.co.jp/t?c={0}&a={1}&b={2}&f={3}&d={4}&e={5}&g=d&s={6}&y={7}&z={6}", d2.Year, d2.Month, d2.Day, e2.Year, e2.Month, e2.Day, codeString, count);
                string html = Download(url);

                //�_�E�����[�h���� html�̉��
                if (!ParseHTML(ret, inBR.Code, html))
                {
                    break;
                }

                //���̃f�[�^������?
                if (html.IndexOf("����") < 0)
                {
                    break;  //�Ȃ��Ȃ�I���
                }
                count += 50;
            }
            return ret;
        }

        //�����̐V�����f�[�^���_�E�����[�h
        //Hashtable[����] = NewDailyData �`��
        Hashtable DownloadCurrentDate(AbstractBrand inBR)
        {
            string codeString = GetYahooCode(inBR.Code, inBR.Market);

            Hashtable ret = new Hashtable();
            if (codeString == "")
            {
                return ret; //���Ή�
            }

            string url = String.Format("http://table.yahoo.co.jp/t?s={0}&g=d", codeString);
            string html = Download(url);

            //�_�E�����[�h���� html�̉��
            ParseHTML(ret, inBR.Code, html);

            return ret;
        }

        string Download(string inUrl)
        {
            try
            {
                MemoryStream ms = Util.HttpDownload(inUrl);
                ms.Close();
                return Encoding.GetEncoding("euc-jp").GetString(ms.ToArray());
            }
            catch(Exception e)
            {
                throw new Exception("URL(" + inUrl + ")���_�E�����[�h���ɃG���[���������܂����B\r\n��O���b�Z�[�W:" + e.Message, e);
            }
        }

        bool ParseHTML(Hashtable ioReslut ,int inCode ,string inHTML)
        {
            int kaishine = inHTML.IndexOf("�n�l");
            if (kaishine < 0)
            {
                return false;
            }
            int tableStart = inHTML.LastIndexOf("<table ", kaishine);
            if (tableStart < 0)
            {
                return false;
            }

            double vv = 1, pv = 1;
            //�{������
            if (IsDomesticIndex(inCode))
            {
                vv = 0.001; //DreamVisor�̂��̂ɂ��킹��i�D�ŁB1000���P�ʂ���
                pv = 100;
            }
            if (inCode == (int)BuiltInIndex.TOPIX_F)
            { //TOPIX�敨�͐����P�ʂŋL�^����Ă���
                pv = 10;
            }

            //�e�[�u���̎�荞��
            ArrayList y = TableToCsv(inHTML.Substring(tableStart));
            for (int yCount = 1 ; yCount < y.Count ; yCount ++ )
            {
                NewDailyData td = new NewDailyData();
                ArrayList x = (ArrayList)y[yCount];
                if (((string)x[1]).IndexOf("����") >= 0)
                {
                    //����?
                    continue;
                }
                int date = Util.StringDateToInt((string)x[0]);
                td.open =  (int) (double.Parse((string)x[1]) * pv);
                td.high = (int) (double.Parse((string)x[2]) * pv);
                td.low = (int) (double.Parse((string)x[3]) * pv);
                td.close = (int) (double.Parse((string)x[4]) * pv);
                if (x.Count > 5)
                {
                    td.volume = (int) (double.Parse((string)x[5]) * vv);
                }
                else
                {
                    td.volume = 0;
                }
                ioReslut[date] = td;
            }
            return true;
        }
        private static bool IsDomesticIndex(int code)
        {
            return code == (int)BuiltInIndex.Nikkei225 || code == (int)BuiltInIndex.TOPIX || code == (int)BuiltInIndex.JASDAQ;
        }

        //table �^�O�� csv �ɕϊ�
        //���G�ȃe�[�u���ɂ͖��Ή��ł��B
        //ArrayList[y] = {ArrayList[x]} �`���ł�.
        ArrayList TableToCsv(string inHTML)
        {
            ArrayList y = new ArrayList();

            //�}����
            inHTML = inHTML.Replace("<TABLE", "<table");
            inHTML = inHTML.Replace("</TABLE", "</table");
            inHTML = inHTML.Replace("<TR", "<tr");
            inHTML = inHTML.Replace("</TR", "</tr");
            inHTML = inHTML.Replace("<TD", "<td");
            inHTML = inHTML.Replace("</TD", "</td");
            inHTML = inHTML.Replace("<TH", "<td");   //TD�����ɂ���
            inHTML = inHTML.Replace("</TH", "</td"); //TD�����ɂ���
            inHTML = inHTML.Replace("<th", "<td");   //TD�����ɂ���
            inHTML = inHTML.Replace("</th", "</td"); //TD�����ɂ���

            int tableStart = inHTML.IndexOf("<table");
            if (tableStart < 0)
            {
                return y;
            }
            int tableEnd = inHTML.IndexOf("</table>", tableStart);
            if (tableEnd < 0)
            {
                return y;
            }
            int trLoop = tableStart;
            while(true)
            {
                //TR�^�O �s�̎擾
                int trStart = inHTML.IndexOf("<tr", trLoop, tableEnd - trLoop);
                if (trStart < 0)
                {
                    break;
                }
                int trEnd = inHTML.IndexOf("</tr>", trStart, tableEnd - trStart);
                if (trEnd < 0)
                {
                    break;
                }
                trLoop = trEnd + 1;

                //TD�^�O ��̎擾
                ArrayList x = new ArrayList();
                int tdLoop = trStart;
                while (true)
                {
                    int tdStart = inHTML.IndexOf("<td", tdLoop, trEnd - tdLoop);
                    if (tdStart < 0)
                    {
                        break;
                    }
                    int tdEnd = inHTML.IndexOf("</td>", tdStart, trEnd - tdStart);
                    if (tdEnd < 0)
                    {
                        break;
                    }
                    tdLoop = tdEnd + 1;

                    //���g��ۑ�.
                    string node = inHTML.Substring(tdStart , tdEnd - tdStart);
                    x.Add(KillTag(node));
                }
                y.Add(x);
            }
            return y;
        }
        string KillTag(string inHTML)
        {
            string retString = "";
            int tagLoop = 0;

            while (true)
            {
                int tagStart = inHTML.IndexOf('<', tagLoop);
                if (tagStart < 0)
                {
                    break;
                }
                //�^�O���n�܂�܂ł̃e�L�X�g�̎�荞��
                if (tagStart - tagLoop > 0)
                {
                    retString += inHTML.Substring(tagLoop, tagStart - tagLoop);
                }
                int tagEnd = inHTML.IndexOf('>', tagStart);
                if (tagEnd < 0)
                {
                    break;  //�^�O����Ă��Ȃ�??
                }
                tagLoop = tagEnd + 1;
            }

            return retString;
        }
    }
}
