//hacked by rti �閧
#if _RTI_ONLY
using System;
using System.Text;
using System.Collections;
using System.Diagnostics;
using System.IO ;

using Travis.Util;
using Zanetti.Arithmetic;
using Zanetti.Indicators;
using Zanetti.SystemTrading;
using Zanetti.SystemTrading.AutoTrading;
using Zanetti.SystemTrading.MoneyPlanning;
using Zanetti.Config;	// for BookmarkFolder


using Zanetti.Data;
using Zanetti.SystemTrading.Screening;


//�R�}���h���C����������낤�Ƃ������ǁA�v�ɂ��āA�ʂ̗p�r�Ŏg���Ă��鏊�B
//rti�ȊO�ɂ͕s�v�Ȃ��̂�����A #if �ŊO����

namespace Zanetti
{
    class CommandLine
    {
        //�o�b�N�e�X�g
        public bool Run2()
        {
            Hashtable command = new Hashtable();
            //command["startdate"] = "19960101";
            command["startdate"] = "20000101";
            command["enddate"] = "20070101";
            command["strategy"] = "autotest";
            command["moneyplanning"] = "��ɏ������Y�̈�芄���𔃂��Â���";
            command["filter"] = "2";
            //command["filter"] = "0";
            command["out"] = "c:\\aaa\\omega.log";
            return AutoTrading2(command , true);
        }
        //�t�H���[�h�e�X�g
        public bool Run3()
        {
            DateTime enddate = Util.GuessLatestTradeDate();
            if (enddate == DateTime.Today && DateTime.Now.Hour < 17)
            {
                enddate = enddate.AddDays(-1);
                while (!Util.IsMarketOpenDate(enddate)) enddate = enddate.AddDays(-1); //�s�ꂪ�J���Ă������17���ȑO�̓f�[�^���Ȃ�
            }

            Hashtable command = new Hashtable();
//            command["startdate"] = "20070101";
//            command["enddate"] = Util.DateToInt(enddate).ToString();
            command["startdate"] = "20060101";
            command["enddate"] = "20070101";
            command["strategy"] = "autotest";
            command["moneyplanning"] = "��ɏ������Y�̈�芄���𔃂��Â���";
            command["filter"] = "2";
            //command["filter"] = "0";
            command["out"] = "c:\\aaa\\omega2.log";
            return AutoTrading2(command , false);
        }
        //�X�N���[�j���O
        public bool Run4()
        {
            Hashtable command = new Hashtable();
            command["date"] = "20070621";
            command["strategy"] = "autoscreening";
            command["out"] = "c:\\aaa\\omega3.log";
            return Screening(command, false);
        }
        //�߂��킹�[
        //�\�[�X�̌��Ղ�? �Ȃɂ���H�����?
        private bool AutoTrading2(Hashtable inCommand , bool islist)
        {
            int startdate = int.Parse((string)inCommand["startdate"]);
            int enddate = int.Parse((string)inCommand["enddate"]);

            int today = Util.DateToInt(DateTime.Today);
            if (today < startdate)
            {
                //                Console.WriteLine("�G���[�E�J�n�����{����薢���ł�");
                return false;
            }
            if (today < enddate)
            {
                //                Console.WriteLine("�G���[�E�I�������{����薢���ł�");
                return false;
            }
            if (startdate > enddate)
            {
                //                Console.WriteLine("�G���[�E�J�n�� > �I�����ł��B");
                return false;
            }
            //�g�p����헪
            AutoTradingItem[] items = Env.CurrentIndicators.AutoTradingItems;
            AutoTradingItem usestrategy = null;
            foreach (AutoTradingItem item in items)
            {
                if (item.Title == (string)inCommand["strategy"])
                {
                    usestrategy = item;
                    break;
                }
            }
            if (usestrategy == null)
            {
                //                Console.WriteLine("�G���[�E�X�g���e�W {0} ��������܂���" , (string)inCommand["strategy"] );
                return false;
            }
            TradingType tradingType = usestrategy.TradingType;

            //�g�p���鎑���Ǘ�
            MoneyPlanningItem[] itemsMoneyPlanning = Env.CurrentIndicators.MoneyPlanningItems;
            MoneyPlanningItem usesMoneyPlanning = null;
            foreach (MoneyPlanningItem itemMoneyPlanning in itemsMoneyPlanning)
            {
                if (itemMoneyPlanning.Title == (string)inCommand["moneyplanning"])
                {
                    usesMoneyPlanning = itemMoneyPlanning;
                    break;
                }
            }
            if (usesMoneyPlanning == null)
            {
                //                Console.WriteLine("�G���[�E�����Ǘ� {0} ��������܂���" , (string)inCommand["moneyplanning"] );
                return false;
            }

            //��������
            AutoTradingExecutor ex = new AutoTradingExecutor(usestrategy, usesMoneyPlanning, startdate, enddate, Int32.MaxValue);

            //�t�@���^�[
            FilterType filter = (FilterType)int.Parse((string)inCommand["filter"]);
            if (filter >= FilterType.Favorite)
            {
                //                Console.WriteLine("�G���[�E�t�B���^�[{0}�͋�����Ă��܂���B" , (string)inCommand["filter"] );
                return false;
            }
            ex.BrandEnumerator = new FilteredBrandEnumerator(filter);

            //�����������s
            ex.Execute();

            //���ʂ��o�͂���X�g���[�����J��
            StreamWriter sw = new StreamWriter((string)inCommand["out"], false);


            if (islist)
            {//���X�g�`��
                //�A�i���X�g�̐�
                int analystCount = usestrategy.GetAnalystCount();
                //���J������
                int columnCount = (int)8 + usestrategy.GetAnalystCount();
                //���ʂ̎擾�ƏW�v
                AutoTradingResult atr = ex.Result as AutoTradingResult;
                for (int n = 0; n < atr.ResultCount; n++)
                {
                    SignalResult sr = atr.GetAtS(n);

                    string[] values = new string[columnCount];
                    values[0] = String.Format("{0:0000}", sr.Brand.Code);
                    values[1] = String.Format("{0:00000000}", sr.StartDate);
                    if (sr.Result == SignalResultType.Skip)
                    {
                        values[2] = "";
                        for (int i = 3; i <= (int)8; i++) values[i] = "";
                    }
                    else
                    {
                        values[2] = sr.StartPrice.ToString("F0");
                        if (sr.Result == SignalResultType.Exit || sr.Result == SignalResultType.Losscut)
                        {
                            values[3] = "";
                            values[4] = String.Format("{0:00000000}", sr.EndDate);
                            values[5] = sr.EndPrice.ToString("F0");
                            double p = sr.Performance(tradingType);
                            values[6] = String.Format("{0:F2}%", p * 100);
                            values[7] = "";
                        }
                        else if (sr.Result == SignalResultType.Draw)
                        {
                            //�h���[�Ɋ֌W�Ȃ��A����������\������
                            values[3] = "";
                            values[4] = String.Format("{0:00000000}", sr.EndDate);
                            values[5] = sr.EndPrice.ToString("F0");
                            double p = sr.Performance(tradingType);
                            values[6] = String.Format("{0:F2}%", p * 100);
                            values[7] = "";													//�����������Ƃ͔��l�ɏ���
                        }
                        else
                        {
                            values[3] = "�H";
                            values[4] = "";
                            values[5] = "";
                            double p = sr.Performance(tradingType);
                            values[6] = "";
                            values[7] = "";																//�����������Ƃ͔��l�ɏ���
                        }
                    }
                    //�A�i���X�g�̈ӌ���ǉ�
                    for (int ana = 0; ana < analystCount; ana++)
                    {
                        values[8 + ana] = sr.GetAnalystComment(ana);
                    }

                    //----
                    int l = 0;
                    for (; l < columnCount; l++)
                    {
                        sw.Write(values[l] + ",");
                    }
                    sw.Write("\r\n");
                }
            }
            else
            {//�W�v����
                AutoTradingResult atr = ex.Result as AutoTradingResult;
                AutoTradingResultSummary sum = atr.CreateSummary();

                sw.Write("signal:{0}", atr.ResultCount);
                sw.Write(",performance:{0:F2}", sum._avgPerformance * 100);
                sw.Write(",win:{0:F2}", (double)sum._exitCount / sum._totalCount * 100);
                sw.Write(",lose:{0:F2}", (double)sum._losscutCount / sum._totalCount * 100);

                try
                {
                    MoneyPlanningSimulator realsim = new MoneyPlanningSimulator(atr,
                        Env.Preference.InitCach,  atr.MoneyPlanningItem);

                    sw.Write(",last:{0}", realsim.GetCash());
                    sw.Write(",Up:{0}",  ((((double)realsim.GetCash() / realsim.GetInitCash()) - 1) * 100) );
                    sw.Write(",MaxDD:{0}", realsim.GetSaiteiRieki());
                    sw.Write(",PF:{0:F2}", realsim.ProfitFactor());
                    sw.Write(",PO:{0:F2}", realsim.PayoffRatio());
                    sw.Write(",RTI:{0:F2}", 0);
                    sw.Write(",BO:{0:F2}", realsim.HasanKakuritu(realsim.PayoffRatio(), realsim.WinPercentage()));
                    sw.Write(",SK:{0:F2}", realsim.SikinKykusenBunseki());
                }
                catch (Exception e)
                {
                }
            }
            sw.Close();

            return true;
        }

        //�߂��킹�[
        //�\�[�X�̌��Ղ�? �Ȃɂ���H�����?
        private bool Screening(Hashtable inCommand , bool islist)
        {
            ScreeningItem useScreening = null;
            foreach (ScreeningItem itemScreening in Env.CurrentIndicators.GetScreeningItems())
            {
                if (itemScreening.Header == (string)inCommand["strategy"])
                {
                    useScreening = itemScreening;
                    break;
                }
            }
            if (useScreening == null)
            {
                return false;
            }
	
            ScreeningOrder so = new ScreeningOrder(useScreening.Header, useScreening);
			FilterType filter = FilterType.None;

            int date = int.Parse((string)inCommand["date"]);
            so.OriginDate = date;
            so.BrandEnumerator = new FilteredBrandEnumerator(filter);

            so.Execute();

            //���ʂ��o�͂���X�g���[�����J��
            StreamWriter sw = new StreamWriter((string)inCommand["out"], false);

            int columnCount = 3;
            ScreeningResult atr = so.Result as ScreeningResult;
            for (int n = 0; n < atr.ResultCount; n++)
            {
                ScreeningResultEntry ent = atr.GetAtS(n);

                string[] values = new string[columnCount];
                AbstractBrand br = ent.Brand;
                values[0] = br.CodeAsString;
                values[1] = br.Name;
                values[2] = Util.FormatValue(ent.PrimaryResult.DoubleVal, so.PrimaryItem.FormatString, so.PrimaryItem.FormatModifier);

                //----
                int l = 0;
                for (; l < columnCount; l++)
                {
                    sw.Write(values[l] + ",");
                }
                sw.Write("\r\n");
            }
            sw.Close();
            
            return true;
        }
    }
}
#endif
//------------------