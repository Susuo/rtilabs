//hacked by rti
using System;
using System.Collections;
using System.Diagnostics;

using Travis.Util;
using Zanetti.Arithmetic;
using Zanetti.Indicators;
using Zanetti.Data;
using Zanetti.SystemTrading.AutoTrading;
using Zanetti.Arithmetic.Series;

namespace Zanetti.SystemTrading.MoneyPlanning
{
    class MoneyPlanningItem
    {
		private string _title;
		private string _header;
		private Expression _signal;
		private EvalResult[] _args;

        public MoneyPlanningItem(string title, string header,  EvalResult[] args, Expression s)
        {
			_title = title;
			_header = header;
			_args = args;
			_signal = s;
		}


		public string Title {
			get {
				return _title;
			}
		}
		public string Header {
			get {
				return _header;
			}
		}

		public Expression Signal {
			get {
				return _signal;
			}
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
		}
    }

    //������������  
    class PiggyBank
    {
        //�������̃J��
        //[EndDate] = Int64 ����
        private Hashtable Can;

        public PiggyBank()
        {
            this.Can = new Hashtable();
        }

        //�������ɔ��������
        //�w����̒��ɂȂ�������o����
        public void Into(Int64 inUriage, int inDate)
        {
            Int64 a = 0;
            if (this.Can[inDate] != null)
            {
                a = (Int64)this.Can[inDate];
            }
            a += inUriage;
            this.Can[inDate] = a;
        }
        //���������炨���������o��
        public Int64 Open(int inDate)
        {
            if (this.Can[inDate] == null)
            {
                return 0;
            }

            Int64 ret = (Int64)this.Can[inDate];
            this.Can.Remove(inDate);
            return ret;
        }
        //�c��
        public Int64 Balance()
        {
            Int64 ret = 0;
            foreach (Int64 v in this.Can.Values)
            {
                ret += v;
            }
            return ret;
        }
        //�������̔j��
        public Int64 Break()
        {
            Int64 ret = Balance();
            this.Can = new Hashtable();

            return ret;
        }
    }





    //���������ꍇ�ׂ̖��̃V���~���[�V����
    internal class MoneyPlanningSimulator
    {
        private MoneyPlanningItem Item;
        private MoneyPlanningEvaluator Evaluator;
        private Int64 InitCash;      //��������
        private Int64 Cash;          //������
        private Int64 SouRieki;      //�����v
        private Int64 SouSonshitu;   //������
        private Int64 Rieki;         //���v(�}�C�i�X�̏ꍇ�͑���)
        private Int64 SaikouRieki;   //�ō��̗��v
        private Int64 SaiteiRieki;   //�Œ�̗��v
        private int WinCount;      //������
        private int LoseCount;     //������
        private int NoMoneyCount;  //�c���s���Ŏd�|�����Ȃ�����
        private ArrayList SousisanLog;  //���X�̑����Y�̋L�^
        private ArrayList Log;

        //���v���������钙����
        private PiggyBank RiekiPiggyBank;
        //�������������钙����
        private PiggyBank GenkaPiggyBank;

        public Int64 GetInitCash()
        {
            return this.InitCash;
        }
        public Int64 GetCash()
        {
            return this.Cash;
        }
        public Int64 GetSouRieki()
        {
            return this.SouRieki;
        }
        public Int64 GetSouSonshitu()
        {
            return this.SouSonshitu;
        }
        public Int64 GetRieki()
        {
            return this.Rieki;
        }
        public Int64 GetSaikouRieki()
        {
            return this.SaikouRieki;
        }
        public Int64 GetSaiteiRieki()
        {
            return this.SaiteiRieki;
        }
        public int GetWinCount()
        {
            return this.WinCount;
        }
        public int GetLoseCount()
        {
            return this.LoseCount;
        }
        public int GetNoMoneyCount()
        {
            return this.NoMoneyCount;
        }
        public string GetLog()
        {
            return string.Join("", this.Log.ToArray(typeof(string)) as string[]);
        }

        //�j�Y�m��
        public double HasanKakuritu(double payoffratio, double win)
        {
            //http://elastigirl.web.fc2.com/dl/downloads.html �̃G�N�Z�����
            //X�� �y�C�I�t���V�I         0.2 0.4	0.6	0.8	1.0	1.2	1.4	1.6	1.8	2.0	2.2	2.4	2.6	2.8	3.0
            //Y�� ����                   10% 20% 30% 40% 50% 60% 70% 80% 90%
            double [] hyou = {
                100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,
                100,	100,	100,	100,	100,	100,	100,	99.90,	99.70,	99.10,	97.70,	95.20,	91.50,	86.80,	81.20,
                100,	100,	100,	100,	99.90,	99.10,	96.40,	90.40,	81.10,	69.60,	57.60,	46.40,	36.60,	28.50,	22.00,
                100,	100,	99.90,	98.80,	92.60,	78.40,	59.50,	41.20,	26.80,	16.80,	10.30,	6.30,	3.90,	2.40,	1.50,
                100,	99.90,	96.10,	78.40,	50.00,	26.00,	11.90,	5.10,	2.20,	0.90,	0.40,	0.20,	0.10,	0.00,	0.00,
                100,	95.00,	64.10,	26.10,	7.40,	1.80,	0.40,	0.10,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,
                98.00,	58.70,	12.40,	1.30,	0.10,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,
                72.20,	6.50,	0.10,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,
                5.80,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,	0.00,
            };

            //��s�̐�
            int YStep = 15;

            int pos = 0;
            //X��
            if (payoffratio >= 3.0) pos = 14;
            else if (payoffratio >= 2.8) pos = 13;
            else if (payoffratio >= 2.6) pos = 12;
            else if (payoffratio >= 2.4) pos = 11;
            else if (payoffratio >= 2.2) pos = 10;
            else if (payoffratio >= 2.0) pos = 9;
            else if (payoffratio >= 1.8) pos = 8;
            else if (payoffratio >= 1.6) pos = 7;
            else if (payoffratio >= 1.4) pos = 6;
            else if (payoffratio >= 1.2) pos = 5;
            else if (payoffratio >= 1.0) pos = 4;
            else if (payoffratio >= 0.8) pos = 3;
            else if (payoffratio >= 0.6) pos = 2;
            else if (payoffratio >= 0.4) pos = 1;
            else if (payoffratio >= 0.2) pos = 0;
            else pos = 0;

            //Y��
            if (win >= 90) pos += 8 * YStep;
            else if (win >= 80) pos += 7 * YStep;
            else if (win >= 70) pos += 6 * YStep;
            else if (win >= 60) pos += 5 * YStep;
            else if (win >= 50) pos += 4 * YStep;
            else if (win >= 40) pos += 3 * YStep;
            else if (win >= 30) pos += 2 * YStep;
            else if (win >= 20) pos += 1 * YStep;
            else if (win >= 10) pos += 0 * YStep;
            else pos += 0 * YStep;

            return hyou[pos];
        }


        public int TotalCount()
        {
            //return WinCount + LoseCount + NoMoneyCount;   //�������Ȃ��Ď���ł��Ȃ���������������v�Z�̕���ɓ����̂͌��������邩�Ȃ�
            return WinCount + LoseCount;
        }

        //����
        public double WinPercentage()
        {
            return (double)GetWinCount() / TotalCount() * 100;
        }
        //����
        public double LosePercentage()
        {
            return (double)GetLoseCount() / TotalCount() * 100;
        }

        //�v���t�B�b�g�t�@�N�^�[
        public double ProfitFactor()
        {
            if (SouSonshitu == 0)
            {
                return Double.NaN;   //�v�Z�s�\
            }
            return (double)SouRieki / Math.Abs(SouSonshitu);
        }
        //�y�C�I�t���V�I
        public double PayoffRatio()
        {
            if (WinCount == 0 || LoseCount == 0 || SouSonshitu == 0)
            {
                return Double.NaN;   //�v�Z�s�\
            }
            return ((double)SouRieki / WinCount) / (Math.Abs(SouSonshitu) / LoseCount);
        }
        //�����Ȑ�����
        public double SikinKykusenBunseki()
        {
            double [] days = new double[this.SousisanLog.Count];
            for (int i = 0; i < this.SousisanLog.Count; i++)
            {
                days[i] = i + 1;    //����
            }
            return Statistics.RSQ(days, this.SousisanLog.ToArray(typeof(double)) as double[] ) * 100;
        }

        //�{�������邨���������`���܂�
        private Int64 GetPocketMoney(int inSignalCount)
        {
            //���������邨�����������߂܂�
            MoneyPlanningEvaluator e =
                new MoneyPlanningEvaluator(this.Item.Title, this.InitCash, this.Cash, this.GenkaPiggyBank.Balance() + this.Cash, inSignalCount);
            e.Args = this.Item.Args;
            EvalResult signal = (EvalResult)this.Item.Signal.Apply(e);

            if (!signal.IsDouble)
            {
                throw new ApplicationException("double�����҂����̂ɗ��؂�ꂽ");
            }
            Int64 pocketMoney = (Int64)signal.DoubleVal;
            //��������葽���������邱�Ƃ͂ł��܂���A�Alet's go�̂��X�ɂł��s���C��!
            if (pocketMoney > this.Cash)
            {
                pocketMoney = this.Cash;
            }
            //�{�������邨������
            return pocketMoney;
        }

        private void Construct(AutoTradingResult r)
        {
            Int64 cashback;
            //���Ԏ����L�[�ɕ��בւ��܂�.
            //[StartDate] = ArrayList(SignalResult,SignalResult,SignalResult,SignalResult)
            Hashtable timeTable = new Hashtable();
            for (int i = 0; i < r.ResultCount; i++)
            {
                SignalResult sr = r.GetAtS(i);
//                if (!IsValidTrade(sr.Result))
//                {
//                    continue;
//                }
                if (timeTable[sr.StartDate] == null)
                {
                    timeTable[sr.StartDate] = new ArrayList();
                }
                ((ArrayList)timeTable[sr.StartDate]).Add(sr);
            }

            int dayCount = 0;

            //��������V���~���[�g���Ă����܂��B
            for (int currentDate = r.StartDate; currentDate <= r.EndDate; currentDate = Util.NextDay(currentDate), dayCount ++)
            {
                //�{�����痘�p�ł���悤�ɂȂ邨����ǉ�
                cashback = RiekiPiggyBank.Open(currentDate) + GenkaPiggyBank.Open(currentDate);
                this.Cash += cashback;

                //�{���̑����Y
                Int64 soushisan = this.GenkaPiggyBank.Balance() + this.Cash;
                //�����Y���L�^ double ���g���̂͑�l�̎���
                this.SousisanLog.Add((double)soushisan);

                //��������f�[�^���擾 
                ArrayList list = (ArrayList)timeTable[currentDate];

                if (list != null || cashback != 0)
                {
                    this.Log.Add( "\r\n�{���� " + FormatDate(currentDate) + "�ł�\r\n");
                    this.Log.Add( "�����ϕ� " + FormatYen(cashback) + "���{����藘�p�ł��܂�\r\n" );
                    this.Log.Add("�]�� " + FormatYen(this.Cash) + "(�����Y " + FormatYen(soushisan) + ")�����p�ł��܂�\r\n");
                }

                if (list == null)
                {
                    continue;
                }

                //���������邨�����������߂܂�
                Int64 pocketMoney = GetPocketMoney(list.Count);
                //���ꂾ������Ȃ��Ⴂ���Ȃ������ЂɎd����̂͂��ꂾ��
                Int64 useLimit = pocketMoney / list.Count;

                this.Log.Add( "�V�O�i����" + (list.Count).ToString() + "�������Ă��܂�\r\n" );
                this.Log.Add( "�{�����p�ł��邨���� " + FormatYen(pocketMoney) + "�ł�\r\n" );
                this.Log.Add( "1�����ɕt�� " + FormatYen(useLimit) + "�܂ŗ��p�ł��܂�\r\n" );

                foreach (SignalResult sr in list)
                {
                    //�P���������܂�..
                    Int64 unit = (sr.Brand is BasicBrand) ? ((BasicBrand)sr.Brand).Unit : 1;
                    if (unit <= 0)
                    {
                        unit = 1;    //�w���ȂǕ��ʂ͔������Ȃ����̂𔄔����悤�Ƃ���Ƃ����Ȃ�炵���B
                    }
                    Int64 startPrice = (Int64)sr.StartPrice;
                    Int64 endPrice = (Int64)sr.EndPrice;
                    int endDate = sr.EndDate;

                    if (!IsValidTrade(sr.Result))
                    {//�������̎��
                        endPrice = startPrice;
                        endDate = r.EndDate + 1;
                    }

                    //�w���o���閇��
                    Int64 mai = (Int64)(useLimit / startPrice / unit);
                    if (mai <= 0)
                    {//�c���s���Ŏd�|�����܂���ł���
                        this.NoMoneyCount++;
                        this.Log.Add( "���� " + sr.Brand.Code.ToString() + " ���w�����悤�Ƃ��܂���������������܂���\r\n" );
                        continue;
                    }

                    //�������グ
                    Int64 genka = mai * startPrice * unit;
                    this.Cash -= genka;
                    Debug.Assert(this.Cash >= 0);
                    this.Log.Add("���� " + sr.Brand.Code.ToString() + "�� " + FormatYen(genka) + " (" + startPrice.ToString() + "@" + (mai * unit).ToString() + " ) �w�����܂���\r\n");

                    //���p���̑��v���v�Z
                    Int64 uriage = mai * endPrice * unit;
                    //�҂�
                    Int64 jyunrieki = 0;
                    if (r.Item.TradingType == TradingType.Long)
                    {
                        jyunrieki = (uriage - genka);   //�����v
                    }
                    else
                    {//�V���[�g�Ȃ̂ŋt�ɂ���
                        jyunrieki = (genka - uriage);   //�����v
                    }
                    //�L���Ȏ����������A�����v�̋L�^
                    if (IsValidTrade(sr.Result))
                    {//�����v�ɂ��ċL�^���� (�ő�̕����Ƃ��A���낢��)
                        Record(jyunrieki);
                    }
                    //�ׂ��𔄋p���܂ŕ܂��Ă����A���p���ɂȂ����玝�����ɉ��Z����.
                    //�������������痘�p�ł���(�ʓ|�Ȃ�ŋ�����)
                    //int cashbackday = Util.NextDay(endDate);
                    int cashbackday = Util.NextDay(Util.NextDay(endDate));   //�����1���ǉ�. ����������Ă���̂��x���ƒ������Ȃ�
                    RiekiPiggyBank.Into(jyunrieki, cashbackday);
                    GenkaPiggyBank.Into(genka, cashbackday);

                    this.Log.Add("���̎���́A" + FormatDate(cashbackday) + "�ɁA���v " + FormatYen(jyunrieki) + "���グ�邱�ƂɂȂ�܂�\r\n");
                }
               this.Log.Add( "�{���̎���͏I�����܂����A�c��]�� " + FormatYen(this.Cash) + "(�����Y " + FormatYen(this.GenkaPiggyBank.Balance() + this.Cash) + ")�ł�\r\n" );
            }
            this.Log.Add( "\r\n\r\n���ׂĂ̎�����I�����܂���\r\n" );

            //����ۋ����I������̂ł��ׂĐ��Z���܂��B
            cashback = RiekiPiggyBank.Break() + GenkaPiggyBank.Break();
            this.Cash += cashback;
            this.Log.Add( "�����ϕ� " + FormatYen(cashback) + "��ǉ����܂�\r\n");
            this.Log.Add( "�����Y " + FormatYen(this.Cash) + " �ɂȂ�܂���\r\n");

            //�Ō�̒[���̊��Ԃ�����ׂ����A�A�A�����ȏ�...�����ǁA�ꉞ�A�����.
            this.SousisanLog.Add((double)this.Cash);

            //�N�}�[?
            if (this.Cash < this.InitCash )
            {
                this.Log.Add( "\r\n���߂����A�N�}�[\r\n" );
            }
        }
        //�L���Ȏ��?
        private bool IsValidTrade(SignalResultType inResult)
        {
            return inResult == SignalResultType.Exit ||
                    inResult == SignalResultType.Losscut ||
                    inResult == SignalResultType.Draw;
        }

        //��������L�^
        private void Record(Int64 rizaya)
        {
            if (rizaya > 0)
            {
                this.SouRieki += rizaya;
                this.WinCount++;
            }
            else
            {
                this.SouSonshitu += rizaya;
                this.LoseCount++;
            }
            this.Rieki += rizaya;

            if (this.Rieki >= this.SaikouRieki)
            {
                this.SaikouRieki = this.Rieki;
            }
            if (this.Rieki <= this.SaiteiRieki)
            {
                this.SaiteiRieki = this.Rieki;
            }
        }

        //�~
        private static string FormatYen(Int64 inYen)
        {
            return String.Format("{0:#,##0} �~", inYen);
        }
        //���t
        private static string FormatDate(int inDate)
        {
            DateTime d = Util.IntToDate(inDate);
            return String.Format("{0}/{1}/{2}", d.Year, d.Month, d.Day);
        }

        public MoneyPlanningSimulator(AutoTradingResult tr, Int64 inStartCash,  MoneyPlanningItem inItem)
        {
            this.InitCash = inStartCash;
            this.Cash = inStartCash;
            this.SouRieki = 0;
            this.SouSonshitu = 0;
            this.Rieki = 0;
            this.SaikouRieki = Int64.MinValue;
            this.SaiteiRieki = Int64.MaxValue;
            this.WinCount = 0;
            this.LoseCount = 0;
            this.NoMoneyCount = 0;
            //���v���������钙����
            this.RiekiPiggyBank = new PiggyBank();
            //�������������钙����
            this.GenkaPiggyBank = new PiggyBank();
            this.Item = inItem;
            this.Log = new ArrayList();
            this.SousisanLog = new ArrayList();
            Construct(tr);
        }
    }


    internal class MoneyPlanningEvaluator : BasicVisitor
    {
        private EvalResult[] _args;
        Int64 InitCash;
        Int64 Cash;
        Int64 TotalAssets;
        int SignalCount;

        public MoneyPlanningEvaluator(string fn, Int64 inInitCash, Int64 inCash, Int64 inTotalAssets, int inSignalCount)
            : base(fn)
        {
            this.InitCash = inInitCash;
            this.Cash = inCash;
            this.TotalAssets = inTotalAssets;
            this.SignalCount = inSignalCount;
        }
        public EvalResult[] Args
        {
            get
            {
                return _args;
            }
            set
            {
                _args = value;
            }
        }

        //���΂炭�̓I���W�i���̃R�s�y

        public EvalResult Eval(Expression expr)
        {
            return (EvalResult)expr.Apply(this);
        }
        public EvalResult[] Eval(Expression[] exprs)
        {
            if (exprs == null) return null;
            EvalResult[] r = new EvalResult[exprs.Length];
            for (int i = 0; i < r.Length; i++) r[i] = (EvalResult)exprs[i].Apply(this);
            return r;
        }

        public void EnsureArgIndex(int index)
        {
            EvalResult[] newval = new EvalResult[Math.Max(index + 1, _args == null ? 0 : _args.Length)];
            if (_args != null)
            {
                for (int i = 0; i < _args.Length; i++)
                    newval[i] = _args[i];
            }
            _args = newval;
        }

        public override object Constant(ConstantExpression expr)
        {
            return new EvalResult(expr.Value);
        }
        public override object Parameter(ParameterExpression expr)
        {
            if (_args == null || _args.Length <= expr.Index)
                ThrowArithmeticException(String.Format("�p�����[�^�̃C���f�b�N�X {0} ���������͈͂ɂ���܂���B", expr.Index));
            return new EvalResult(_args[expr.Index]);
        }

        public override object Primitive(PrimitiveExpression expr)
        {
            Debug.Assert(false);
            throw new Exception("�Ԃ����Ⴏ���肦�Ȃ�");
        }

        public override object BinaryOp(BinaryOpExpression expr)
        {
            if (expr.Type == BinaryOpExpressionType.And || expr.Type == BinaryOpExpressionType.Or || expr.Type == BinaryOpExpressionType.Predication)
            {
                //�V���[�g�J�b�g�]���\
                return BinaryOpLogical(expr);
            }
            else
            {
                //�V���[�g�J�b�g��r�͕s��
                EvalResult r1 = (EvalResult)expr.Left.Apply(this);
                EvalResult r2 = (EvalResult)expr.Right.Apply(this);
                if (expr.Type == BinaryOpExpressionType.Plus || expr.Type == BinaryOpExpressionType.Minus)
                    return BinaryOpAddSub(expr, r1, r2);
                else if (expr.Type == BinaryOpExpressionType.Multiply || expr.Type == BinaryOpExpressionType.Divide)
                    return BinaryOpMulDiv(expr, r1, r2);
                else //����ȊO�͔�r���Z
                    return BinaryOpComp(expr, r1, r2);
            }
        }
        private EvalResult BinaryOpLogical(BinaryOpExpression expr)
        {
            EvalResult r1 = (EvalResult)expr.Left.Apply(this);
            Debug.Assert(r1.IsBool);
            bool v1 = r1.BoolVal;
            if (expr.Type == BinaryOpExpressionType.And)
            {
                if (!v1)
                {
                    r1.BoolVal = false; //�I�u�W�F�N�g�����ʍ팸�e�N
                    return r1;
                }
                else
                    return (EvalResult)expr.Right.Apply(this);
            }
            else if (expr.Type == BinaryOpExpressionType.Or)
            {
                if (v1)
                {
                    r1.BoolVal = true;
                    return r1;
                }
                else
                    return (EvalResult)expr.Right.Apply(this);
            }
            else
            {
                Debug.Assert(expr.Type == BinaryOpExpressionType.Predication);
                if (!v1)
                {
                    r1.BoolVal = true;
                    return r1;
                }
                else
                    return (EvalResult)expr.Right.Apply(this);
            }
        }
        private EvalResult BinaryOpAddSub(BinaryOpExpression expr, EvalResult r1, EvalResult r2)
        {
            //���l���m�A�T�C�Y�̓�����TimeSeries���m�ŉ��Z�\
            if (r1.IsDouble && r2.IsDouble)
            {
                if (expr.Type == BinaryOpExpressionType.Plus)
                    r1.DoubleVal += r2.DoubleVal;
                else
                    r1.DoubleVal -= r2.DoubleVal;
                return r1;
            }
            else if (r1.IsTimeSeries && r2.IsTimeSeries)
            {
                if (expr.Type == BinaryOpExpressionType.Plus)
                    r1 = new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, BinOpTimeSeries.ADD));
                else
                    r1 = new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, BinOpTimeSeries.SUB));
                return r1;
            }
            else if (r1.IsTimeSeries && r2.IsDouble)
            {
                r1 = new EvalResult(new SingleOpTimeSeries(r1.TimeSeriesVal, SingleOpTimeSeries.CreateSlided(expr.Type == BinaryOpExpressionType.Plus ? r2.DoubleVal : -r2.DoubleVal)));
                return r1;
            }
            else if (r2.IsTimeSeries && r1.IsDouble)
            {
                r2 = new EvalResult(new SingleOpTimeSeries(r2.TimeSeriesVal, SingleOpTimeSeries.CreateSlided(expr.Type == BinaryOpExpressionType.Plus ? r1.DoubleVal : -r1.DoubleVal)));
                return r2;
            }
            else
                throw new Exception("type mismatch(add/sub)");
        }
        private EvalResult BinaryOpMulDiv(BinaryOpExpression expr, EvalResult r1, EvalResult r2)
        {
            //���l���m�ATimeSeries�Ɛ��l�ŉ��Z�\
            if (r1.IsDouble && r2.IsDouble)
            {
                if (expr.Type == BinaryOpExpressionType.Multiply)
                    r1.DoubleVal *= r2.DoubleVal;
                else
                    r1.DoubleVal /= r2.DoubleVal;
                return r1;
            }
            else if (r1.IsTimeSeries && r2.IsDouble)
            {
                r1 = new EvalResult(new SingleOpTimeSeries(r1.TimeSeriesVal,
                    SingleOpTimeSeries.CreateScalar(expr.Type == BinaryOpExpressionType.Multiply ? r2.DoubleVal : 1 / r2.DoubleVal)));
                return r1;
            }
            else if (r1.IsTimeSeries && r2.IsTimeSeries)
            {
                if (r1.TimeSeriesVal.Count != r2.TimeSeriesVal.Count) throw new Exception("timeseries length mismatch");
                return new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, expr.Type == BinaryOpExpressionType.Multiply ? BinOpTimeSeries.MUL : BinOpTimeSeries.DIV));
            }
            else
                throw new Exception("type mismatch(mul/div)");
        }

        private EvalResult BinaryOpComp(BinaryOpExpression expr, EvalResult r1, EvalResult r2)
        {
            //���l���m�̂�
            if (r1.IsDouble && r2.IsDouble)
            {
                double v1 = r1.DoubleVal;
                double v2 = r2.DoubleVal;
                bool result = false;
                switch (expr.Type)
                {
                    case BinaryOpExpressionType.Eq:
                        result = (v1 == v2);
                        break;
                    case BinaryOpExpressionType.NotEq:
                        result = (v1 != v2);
                        break;
                    case BinaryOpExpressionType.Smaller:
                        result = (v1 < v2);
                        break;
                    case BinaryOpExpressionType.SmallerEq:
                        result = (v1 <= v2);
                        break;
                    case BinaryOpExpressionType.Greater:
                        result = (v1 > v2);
                        break;
                    case BinaryOpExpressionType.GreaterEq:
                        result = (v1 >= v2);
                        break;
                }
                r1.BoolVal = result;
                return r1;
            }
            else
                throw new Exception("type mismatch(bin-op)");
        }

        public override object UnaryOp(UnaryOpExpression expr)
        {
            Debug.Assert(expr.Type == UnaryOpExpressionType.Inverse);
            EvalResult e = (EvalResult)expr.Content.Apply(this);
            if (e.IsBool)
                e.BoolVal = !e.BoolVal;
            else if (e.IsDouble)
                e.DoubleVal = -e.DoubleVal;
            else
                e = new EvalResult(new SingleOpTimeSeries(e.TimeSeriesVal, SingleOpTimeSeries.NEG));

            return e;
        }

        public override object Function(FunctionExpression expr)
        {
            if (expr.Category == FunctionExpression.FECategory.Unknown)
            { //���O�̉�������B�g�ݍ��݃��C�u�����ƒ�`�ς�Indicator�̂ǂ��炩�łȂ��Ƃ����Ȃ�
                int i = FindMoneyPlanningFunctionID(expr.Name);
                if (i != -1)
                {
                    expr.Category = FunctionExpression.FECategory.Library;
                    expr.LaneID = i;
                }
                else
                {
                    Indicator ind = Env.CurrentIndicators.FindFromAscii(expr.Name);
                    if (ind != null)
                    {
                        expr.Category = FunctionExpression.FECategory.Indicator;
                        expr.LaneID = ind.LaneID;
                    }
                    else
                    {
                        ThrowArithmeticException(expr.Name + " �͒�`����Ă��܂���B");
                    }
                }

            }


            if (expr.Category == FunctionExpression.FECategory.Library && IsMoneyPlanningFunctionID(expr.LaneID))
            {
                return CalcMoneyPlanningFunction(expr);
            }

            ExpressionIndicator ei = (ExpressionIndicator)Env.CurrentIndicators.GetIndicator(expr.LaneID);
            if (ei.ArgCount == 0)
            {
                throw new ApplicationException("�p�����[�^�̈�����0�ł��A���肦�܂���");
            }
            else
            { //�����O�̏ꍇ�̓L���b�V�����L���ɂȂ邪�A�����łȂ��Ƃ��͍Čv�Z�B�ꉞ�����̒��g���`�F�b�N���邱�ƂŃL���b�V�����쐬���邱�Ƃ��ł��邪�ʓ|�Ȃ̂ł�߂�
                EvalResult[] t = new EvalResult[ei.ArgCount];
                //�����O�̂Ƃ��͎Q�Ɛ�̃p�����[�^�����̂܂܎g��
                if (expr.Args == null || expr.Args.Length == 0)
                    t = ei.Args;
                else
                {
                    if (ei.ArgCount != expr.Args.Length)
                        ThrowArithmeticException(String.Format("{0} �̈����� {1} �łȂ���΂Ȃ�܂��񂪁A{2} �ł��B", expr.Name, ei.Args.Length, expr.Args.Length));
                    for (int i = 0; i < t.Length; i++)
                        t[i] = (EvalResult)expr.Args[i].Apply(this);
                }

                //�p�����[�^��u�����čĎ��s
                EvalResult[] saved = _args;
                _args = t;
                EvalResult r = (EvalResult)ei.Expression.Apply(this);
                _args = saved;
                return r;
            }
        }

        private enum MoneyPlanningFunctionID
        {
            InitCash = 0x2000,
            Cash,
            TotalAssets,
            SignalCount,
            End
        }
        private static bool IsMoneyPlanningFunctionID(int inType)
        {
            return (inType >= (int)MoneyPlanningFunctionID.InitCash && inType < (int)MoneyPlanningFunctionID.End);
        }
        private EvalResult CalcMoneyPlanningFunction(FunctionExpression inExpr)
        {
            switch (inExpr.LaneID)
            {
                case (int)MoneyPlanningFunctionID.InitCash:
                    return new EvalResult(this.InitCash);
                case (int)MoneyPlanningFunctionID.Cash:
                    return new EvalResult(this.Cash);
                case (int)MoneyPlanningFunctionID.TotalAssets:
                    return new EvalResult(this.TotalAssets);
                case (int)MoneyPlanningFunctionID.SignalCount:
                    return new EvalResult(this.SignalCount);
                default:
                    Debug.Assert(false); //�����ɂ͗��Ȃ�
                    return null;
            }
        }

        private static int FindMoneyPlanningFunctionID(string name)
        {
            if (name == "initcash")
                return (int)MoneyPlanningFunctionID.InitCash;
            else if (name == "cash")
                return (int)MoneyPlanningFunctionID.Cash;
            else if (name == "total_assets")
                return (int)MoneyPlanningFunctionID.TotalAssets;
            else if (name == "signal_count")
                return (int)MoneyPlanningFunctionID.SignalCount;
            else
                return -1;
        }
    }

}
//-------------------------------