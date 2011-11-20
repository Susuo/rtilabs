/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Text;
using System.Reflection;

using Travis.Util;

using Zanetti.Arithmetic;
using Zanetti.Parser;
 
namespace Zanetti.Data
{
	[EnumDesc(typeof(MarketType))]
	internal enum MarketType {
		[EnumValue(Description="�w��")] B, //�w���A�בւȂ�
		[EnumValue(Description="���؂P��")] T1,
		[EnumValue(Description="���؂Q��")] T2,
		[EnumValue(Description="��؂P��")] O1,
		[EnumValue(Description="��؂Q��")] O2,
		[EnumValue(Description="JASDAQ")] J,
		[EnumValue(Description="�}�U�[�Y")] M,
		[EnumValue(Description="�w���N���X")] H,
        [EnumValue(Description="�h������")] Custom,
        [EnumValue(Description="���i")] C //���ʓ����o�[�W�����ł̂ݕK�v
    }

	[EnumDesc(typeof(BuiltInIndex))]
	internal enum BuiltInIndex {
		//�����w��
		[EnumValue(Description="���o����")] Nikkei225 = 101,
		[EnumValue(Description="TOPIX")]    TOPIX = 102,
		[EnumValue(Description="��������")] LONGTERM_INTEREST = 0, //�����DreamVisor���炵���擾�ł��Ȃ��̂Ń����B�l�������W�O�ɂ��Ă���
		[EnumValue(Description="JASDAQ")]       JASDAQ = 106,
		//�����w���敨
		[EnumValue(Description="���o���ϐ敨")] Nikkei225_F = 151,
		[EnumValue(Description="TOPIX�敨")]    TOPIX_F = 152,
		//�ב�
		[EnumValue(Description="�~�^�h��")] JPYUSD = 201,
		[EnumValue(Description="�~�^���[��")] JPYEUR = 202,
		//�O�������w��
		[EnumValue(Description="NY�_�E")] Dow = 301,
		[EnumValue(Description="NASDAQ")] Nasdaq = 302,
		[EnumValue(Description="SP500")] SP500 = 303,
		//[EnumValue(Description="FT100")] FT100 = 304, //�P���~�������̂��߂��̂Q�͂�����߂�
		//[EnumValue(Description="DAX30")] DAX30 = 305,
		//�Ǝ��
		[EnumValue(Description="���Y")]      SI_FISHING = 501, //�h��������400�ԑ�ɂ��Ă��܂����̂�
		[EnumValue(Description="�z��")]      SI_MINING, 
		[EnumValue(Description="����")]      SI_CONSTRUCTION,
		[EnumValue(Description="�H�i")]      SI_FOOD,

		[EnumValue(Description="�@��")]      SI_FABRIC,
		[EnumValue(Description="�p���v")]    SI_PULP ,
		[EnumValue(Description="���w")]      SI_CHEMICAL,
		[EnumValue(Description="���")]      SI_MEDICAL,

		[EnumValue(Description="�Ζ�")]      SI_OIL,
		[EnumValue(Description="�S��")]      SI_RUBBER,
		[EnumValue(Description="�K���X")]    SI_CERAMIC,
		[EnumValue(Description="�S�|")]      SI_IRON,
		
		[EnumValue(Description="��S")]      SI_NONEMETAL,
		[EnumValue(Description="����")]      SI_METAL,
		[EnumValue(Description="�@�B")]      SI_MACHINE ,
		[EnumValue(Description="�d�C")]      SI_ELECTRIC,
		
		[EnumValue(Description="�A��")]      SI_FREIGHTER_MACHINE,
		[EnumValue(Description="����")]      SI_PRECISIONS,
		[EnumValue(Description="����")]      SI_MANIFUCTURING,
		[EnumValue(Description="�K�X")]      SI_GAS,
		
		[EnumValue(Description="���^")]      SI_LANDTRANSPORT,
		[EnumValue(Description="�C�^")]      SI_MARINETRANSPORT,
		[EnumValue(Description="��^")]      SI_AIRTRANSPORT,
		[EnumValue(Description="�q��")]      SI_STORAGE,

		[EnumValue(Description="���")]      SI_COMMUNICATION,
		[EnumValue(Description="����")]      SI_TRADEFARM,
		[EnumValue(Description="����")]      SI_RETAIL,
		[EnumValue(Description="��s")]      SI_BANK,

		[EnumValue(Description="�،�")]      SI_SECURITY,
		[EnumValue(Description="�ی�")]      SI_INSUARANCE,
		[EnumValue(Description="���Z")]      SI_FINANCE,
		[EnumValue(Description="�s���Y")]    SI_REALESTATES,

		[EnumValue(Description="�T�[�r�X")]  SI_SERVICE,

        //600��͏��i
        //COMMOITY_START = 600
	}
	internal enum BuiltInIndexGroup {
		DomesticIndexStart = BuiltInIndex.Nikkei225,
		DomesticIndexEnd   = BuiltInIndex.TOPIX_F,
		ForeignIndexStart  = BuiltInIndex.JPYUSD, //�����Ƃ��Ă͂قڈꏏ
		ForeignIndexEnd    = BuiltInIndex.SP500,
		SectorIndexStart   = BuiltInIndex.SI_FISHING,
		SectorIndexEnd     = BuiltInIndex.SI_SERVICE
	}




	//�������
	internal class SplitInfo {
		private int _date;
		private double _ratio;

		public int Date {
			get {
				return _date;
			}
		}
		public double Ratio {
			get {
				return _ratio;
			}
		}
		public SplitInfo(int date, double ratio) {
			_date = date;
			_ratio = ratio;
		}
		public SplitInfo(string txt) {
			//S:ddd:value����̍\�z
			string[] t = txt.Split(':');
			_date = Int32.Parse(t[1]);
			_ratio = Double.Parse(t[2]);
		}
	}

	//�����̊�{���
	internal abstract class AbstractBrand {
		protected string _name;
		protected int _code;
		protected MarketType _market;
		protected int _referenceValue;
		protected SplitInfo[] _splitInfo;


		public AbstractBrand(string name, int code, MarketType m) {
			_name = name;
			_code = code;
			_market = m;
		}
		public string Name {
			get {
				return _name;
			}
		}
		public int Code {
			get {
				return _code;
			}
		}
		public string CodeAsString {
			get {
				string t = _code.ToString();
				Debug.Assert(t.Length<=4);
				if(t.Length<4) {
					StringBuilder bld = new StringBuilder(4);
					for(int i=0; i<4-t.Length; i++) bld.Append('0');
					bld.Append(t);
					return bld.ToString();
				}
				else
					return t;
			}
		}
		public SplitInfo[] SplitInfo {
			get {
				return _splitInfo;
			}
			set {
				_splitInfo = value;
			}
		}
		public MarketType Market {
			get {
				return _market;
			}
		}

		public bool IsBuiltIn { 
			get {
				return _code<1000;
			}
		}
		public bool IsSectorIndex {
			get {
				return (int)BuiltInIndexGroup.SectorIndexStart<=_code && _code<=(int)BuiltInIndexGroup.SectorIndexEnd;
			}
		}
		public bool IsDomesticIndex {
			get {
				return (int)BuiltInIndexGroup.DomesticIndexStart<=_code && _code<=(int)BuiltInIndexGroup.DomesticIndexEnd;
			}
		}
		public bool IsForeignIndex {
			get {
				return (int)BuiltInIndexGroup.ForeignIndexStart<=_code && _code<=(int)BuiltInIndexGroup.ForeignIndexEnd;
			}
		}
		public bool IsCommodity {
			get {
				//���Ƃ̓��ꐫ�ł�_code�ŕ��ނ��ׂ������A�萔��DOJIMA�f�B���N�g���ɂ����Ėʓ|������
				return _market==MarketType.C;
			}
		}
		public bool IsIndexFuture {
			get {
				return (int)BuiltInIndex.Nikkei225_F==_code || (int)BuiltInIndex.TOPIX_F==_code;
			}
		}

		public virtual bool IsVolumeAvailable {
			get {
				if(!IsBuiltIn) //�����͂��ꂾ
					return true;
				else if(IsDomesticIndex)
					return _code==(int)BuiltInIndex.Nikkei225 || _code==(int)BuiltInIndex.TOPIX || _code==(int)BuiltInIndex.JASDAQ ||
						_code==(int)BuiltInIndex.Nikkei225_F || _code==(int)BuiltInIndex.TOPIX_F;
				else if(this.IsForeignIndex)
					return _code!=(int)BuiltInIndex.JPYUSD && _code!=(int)BuiltInIndex.JPYEUR;
				else if(this.IsSectorIndex)
					return false;
				else
					return true;

			}
		}


		//�ۑ�����Ă���f�[�^��������ւ̕ϊ��ɉ���������΂悢��
		public virtual double PriceScale {
			get {
				if(!IsBuiltIn) //�����͂��ꂾ
					return 1;
				else if(IsDomesticIndex) {
					if(_code==(int)BuiltInIndex.Nikkei225_F)
						return 1;
					else if(_code==(int)BuiltInIndex.LONGTERM_INTEREST)
						return 0.001;
					else if(_code==(int)BuiltInIndex.TOPIX_F)
						return 0.1;
					else
						return 0.01;
				}
				else if(this.IsForeignIndex || this.IsSectorIndex) {
					return 0.01;
				}
				else if(IsCommodity)
					return 1;
				else
					return 1;
			}
		}
		public virtual string PriceFormatString {
			get {
				if(!IsBuiltIn) //�����͂��ꂾ
					return "F0";
				else if(IsDomesticIndex) {
					if(_code==(int)BuiltInIndex.Nikkei225_F)
						return "F0";
					else if(_code==(int)BuiltInIndex.LONGTERM_INTEREST)
						return "F3";
					else
						return "F2";
				}
				else if(this.IsForeignIndex || this.IsSectorIndex)
					return "F2";
				else if(IsCommodity)
					return "F0";
				else
					return "F0";
			}
		}
		public int ReferenceValue {
			get {
				return _referenceValue;
			}
			set {
				_referenceValue = value;
			}
		}
		public bool HasFarm {
			get {
				return Env.BrandCollection.HasFarm(this);
			}
		}

		//date�̓��t�̂P����basis�̓��ɉ��{�ɂȂ��Ă��邩�𕪊��f�[�^����v�Z
		public double CalcSplitRatio(int date, int basis) {
			if(_splitInfo==null) return 1;
			double r = 1;
			for(int i=_splitInfo.Length-1; i>=0; i--) {
				SplitInfo si = _splitInfo[i];
				if(si.Date>basis) continue; 
				if(date < si.Date) r *= si.Ratio;
				else break;
			}
			return r;
		}

		public abstract DataFarm ReserveFarm();
        //hacked by rti
        public abstract DataFarm ReserveFarm(ChartFormat format);
        //--------
		public abstract DataFarm CreateDailyFarm(int extra_dates);
        //!!�ȉ��Q���ė����K�v�H
        //hacked by rti
		//protected abstract DataFarm CreateFarm(ChartFormat format);
        //		public abstract DataFarm CreateFarm(ChartFormat format);

        public abstract DataFarm CloneFarm(ChartFormat format);
        //-----

	}

	//�ʂ̂P����
	internal class BasicBrand : AbstractBrand {
		private int  _unit; //�P�ʊ��� ���ꂪ���Ă͂܂�Ȃ������ł͂O
		private bool _nikkei225;
		private bool _active500;
		private bool _obsolete; //���p�~

		public BasicBrand(string name, int code, MarketType m) : base(name, code, m) {
		}

		public bool Nikkei225 {
			get {
				return _nikkei225;
			}
			set {
				_nikkei225 = value;
			}
		}
		public bool Active500 {
			get {
				return _active500;
			}
			set {
				_active500 = value;
			}
		}
		public bool Obsolete {
			get {
				return _obsolete;
			}
			set {
				_obsolete = value;
			}
		}
		public int Unit {
			get {
				return _unit;
			}
			set {
				_unit = value;
			}
		}

		public override DataFarm ReserveFarm() {
			return Env.BrandCollection.ReserveFarm(this);
		}
        //hacked by rti
        public override DataFarm ReserveFarm(ChartFormat format)
        {
            return Env.BrandCollection.ReserveFarm(this , format);
        }
        //--------

		public override DataFarm CreateDailyFarm(int extra_dates) {
			return Env.BrandCollection.CreateDailyFarm(this, extra_dates);
		}
        //hacked by rti
//        protected override DataFarm CreateFarm(ChartFormat format) {
//        	return Env.BrandCollection.CreateFarm(this, format);
//        }
        //public override DataFarm CreateFarm(ChartFormat format) {
        //	return Env.BrandCollection.CreateFarm(this, format);
        //}
        public override DataFarm CloneFarm(ChartFormat format)
        {
            return Env.BrandCollection.CloneFarm(this, format);
        }
        //

	}

	internal class DerivedBrand : AbstractBrand {
		protected Expression _expression;
		protected string _rawExpression;
		protected AbstractBrand[] _dependencies;
		protected double _priceScale;
		protected string _formatString;

		public DerivedBrand(AbstractBrand[] dep, string expr, string name, int code, MarketType m, double pricescale, string formatstring) : base(name, code, m) {
			_rawExpression = expr;
			_dependencies = dep;
			_priceScale = pricescale;
			_formatString = formatstring;
		}
		public override double PriceScale {
			get {
				return _priceScale;
			}
		}
		public override string PriceFormatString {
			get {
				return _formatString;
			}
		}
		public Expression Expression {
			get {
				if(_expression==null) {
					ExpressionConstructor ee = new ExpressionConstructor();
					new ZPredicationParser(new StringReader(_rawExpression), ee).Parse();
					_expression = ee.Result;
				}
				return _expression;
			}
		}
		public AbstractBrand[] Dependencies {
			get {
				return _dependencies;
			}
		}

		public override DataFarm ReserveFarm() {
			return Env.BrandCollection.ReserveFarm(this);
		}
        //hacked by rti
        public override DataFarm ReserveFarm(ChartFormat format)
        {
            return Env.BrandCollection.ReserveFarm(this, format);
        }
        //------
        public override DataFarm CreateDailyFarm(int extra_dates)
        {
			return Env.BrandCollection.CreateDailyFarm(this, extra_dates);
		}
        //hacked by rti
//        protected override DataFarm CreateFarm(ChartFormat format) {
//        	return Env.BrandCollection.CreateFarm(this, format);
//        }
        //public override DataFarm CreateFarm(ChartFormat format) {
		//	return Env.BrandCollection.CreateFarm(this, format);
		//}
        public override DataFarm CloneFarm(ChartFormat format)
        {
            return Env.BrandCollection.CloneFarm(this, format);
        }

        //-------
		public override bool IsVolumeAvailable {
			get {
				return false;
			}
		}

	}
//hacked by rti
//�L���b�V������������Ă��Ȃ��̂ŁA�X�N���b�v�E�A���h�E<S>�X�N���b�v</S>�r���h����
    internal class BrandCollection
    {
        private Hashtable _data; //code�̒l����AbstractBrand��
        private Hashtable _farms; //AbstractBrand����Farm�ց@Farm�̌��𐧌�����@�\�͂܂�

        private int _lastUpdatedDate;

        private int _stockBrandCount;
        private ArrayList _derivedBrands;

        //�����Ă���DataFarm�̐���farmCapacity�𒴂��Ȃ��悤�ɂ���
        private int _farmCapacity;
        private int _nextRefCount;

        public BrandCollection()
        {
            _data = new Hashtable();
            _farms = new Hashtable();
            _derivedBrands = new ArrayList();

            _farmCapacity = Env.Options.CacheCapacity;
            _nextRefCount = 1;
        }
        public int TotalCount
        {
            get
            {
                return _data.Count;
            }
        }
        //�ʖ����̐�
        public int StockBrandCount
        {
            get
            {
                return _stockBrandCount;
            }
        }
        public int DerivedBrandCount
        {
            get
            {
                return _derivedBrands.Count;
            }
        }
        public int LastUpdatedDate
        {
            get
            {
                return _lastUpdatedDate;
            }
        }

        public void Load(string filename)
        {
            //index�t�@�C������ǂ�ō\�z
            //!!�o�[�W�����`�F�b�N�K�v
            StreamReader sr = new StreamReader(filename, Encoding.Default);
            string line = sr.ReadLine();
            ReadHeader(line);
            line = sr.ReadLine();

            int n = 1;
            ArrayList splits = new ArrayList();
            _stockBrandCount = 0;

            _data.Clear();
            _farms.Clear();

            LoadBuiltIns();

            while (line != null)
            {
                string[] d = line.Split(',');
                if (d.Length < 4) throw new IOException("index file format error: line" + n);
                try
                {
                    int code = Int32.Parse(d[0]);
                    BasicBrand br = new BasicBrand(d[1], code, (MarketType)Enum.Parse(typeof(MarketType), d[2], true));
                    br.Unit = Int32.Parse(d[3]);
                    _data[code] = br;
                    _stockBrandCount++;

                    for (int i = 4; i < d.Length; i++)
                    {
                        string col = d[i];
                        if (col == "N225") br.Nikkei225 = true;
                        if (col == "A500") br.Active500 = true;
                        if (col == "OBS") br.Obsolete = true;
                        Debug.Assert(col.Length > 0);
                        if (col.Length > 0 && col[0] == 'S') splits.Add(new SplitInfo(d[i]));
                    }
                    if (splits.Count > 0)
                    {
                        br.SplitInfo = (SplitInfo[])splits.ToArray(typeof(SplitInfo));
                        splits.Clear();
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(String.Format("{0} �s{1} {2}", ex.Message, n, line));
                }
                line = sr.ReadLine();
                n++;
                //Debug.WriteLine(n);
            }
            sr.Close();
        }

        public AbstractBrand FindBrand(int code)
        {
            return (AbstractBrand)_data[code];
        }
        public AbstractBrand FindNextBrand(int code)
        {
            // TODO:10000�͒萔�ɏo��
            for (int i = code + 1; i < 10000; i++)
            {
                if (_data.ContainsKey(i))
                {
                    return (AbstractBrand)_data[i];
                }
            }
            return FindNextBrand(0);
        }
        public AbstractBrand FindPrevBrand(int code)
        {
            for (int i = code - 1; i > 0; i--)
            {
                if (_data.ContainsKey(i))
                {
                    return (AbstractBrand)_data[i];
                }
            }
            return FindPrevBrand(10000);
        }

        public void Append(AbstractBrand br)
        {
            _data[br.Code] = br;
            if (br is DerivedBrand) _derivedBrands.Add(br);
        }
        public ArrayList DerivedBrands
        {
            get
            {
                return _derivedBrands;
            }
        }

        public IDictionaryEnumerator GetEnumerator()
        {
            return _data.GetEnumerator();
        }
        public DataFarm ReserveFarm(AbstractBrand br)
        {
            return ReserveFarm(br, Env.CurrentIndicators.Format);
        }

        public DataFarm ReserveFarm(AbstractBrand br, ChartFormat fmt)
        {
            DataFarm f = (DataFarm)_farms[br.Code];
            bool ok = false;
            if (f == null)
            {
                //�L���b�V���ɂȂ������
            }
            else if (Util.IsDailyBased(fmt) && f is DailyDataFarm)
            {
                ok = true;
            }
            else if (fmt == ChartFormat.Weekly &&  f is WeeklyDataFarm)
            {
                ok = true;
            }
            else if (fmt == ChartFormat.Monthly && f is MonthlyDataFarm)
            {
                ok = true;
            }
            if (ok)
            {//�L���b�V���Ƀq�b�g
                if (br != _data[br.Code])
                {
                    Debug.Assert(false);
                }


                if (f.IsEmpty) f.LoadFor(br);
                if (br.ReferenceValue < _nextRefCount - 1)
                    br.ReferenceValue = _nextRefCount++;
                return f;
            }

            if (_farms.Count >= _farmCapacity) CleanFarms();

            if (br is BasicBrand)
                f = CreateFarm((BasicBrand)br, fmt);
            else
                f = CreateFarm((DerivedBrand)br, fmt);

            _farms[br.Code] = f;
            br.ReferenceValue = _nextRefCount++;
            return f;
        }
        public bool HasFarm(AbstractBrand br)
        {
            return _farms.Contains(br);
        }

        private void CleanFarms()
        {
            ArrayList t = new ArrayList(_farms.Keys);
            t.Sort(new BrandComparer(_data));

            Hashtable newfarms = new Hashtable();
            for (int n = t.Count / 2; n < t.Count; n++)
            {
                int code = (int)t[n];
                AbstractBrand br = (AbstractBrand)_data[code];
                br.ReferenceValue = n;
                newfarms[code] = _farms[code];
            }
            _nextRefCount = t.Count;
            _farms = newfarms;
        }

        //�L���b�V���Ƃ͖��֌W��DataFarm���쐬
        public DataFarm CreateDailyFarm(AbstractBrand br, int extra_dates)
        {
            DailyDataFarm f = new DailyDataFarm();
            f.LoadFor(br, extra_dates);
            return f;
        }
        private DataFarm CreateFarm(BasicBrand br, ChartFormat format)
        {
            DataFarm f;
            if (Util.IsDailyBased(format))
                f = new DailyDataFarm();
            else if (format == ChartFormat.Weekly)
                f = new WeeklyDataFarm();
            else	// Monthly
                f = new MonthlyDataFarm();
            f.LoadFor(br);
            return f;
        }
        private DataFarm CreateFarm(DerivedBrand br, ChartFormat format)
        {
            DerivedDataFarm f = new DerivedDataFarm(br, format);
            f.LoadFor(br);
            return f;
        }

        //�L���b�V������N���[��������ēn��.
        //�{���̓L���b�V���̃f�[�^�����̂܂ܓn�������������񂾂��A
        //���� return �悪���̃f�[�^�����������邩������Ȃ��B
        //C++ ����������A const DataFarm ���Ă�邾���Ŕ��ʂł���̂� C#�̓N�\�����炻�ꂪ�o���Ȃ��B
        //���N�A����ȑ�K�͊J���Ɍ����Ȃ�����͌��̂Ă邵���Ȃ�!!
        public DataFarm CloneFarm(AbstractBrand br, ChartFormat format)
        {
            DataFarm p = ReserveFarm(br, format);
            return (DataFarm)p.Clone();
        }


        private class BrandComparer : IComparer
        {
            Hashtable Data;

            public BrandComparer(Hashtable inData)
            {
                this.Data = inData;
            }
            public int Compare(object x, object y)
            {
                return ((AbstractBrand)this.Data[x]).ReferenceValue - ((AbstractBrand)this.Data[y]).ReferenceValue;
            }

        }

        public void ClearAllFarms()
        {
            _farms.Clear();

            _nextRefCount = 1;
#if DOJIMA
			Dojima.DojimaUtil.HalfDailyDataFarmCache.ClearAll();
#endif
        }

        public ICollection Values
        {
            get
            {
                return _data.Values;
            }
        }

        private void LoadBuiltIns()
        {
            AddBrandFromEnumType(typeof(BuiltInIndex).GetMembers(), MarketType.B);
#if DOJIMA
			AddBrandFromEnumType( typeof(CommodityIndex).GetMembers(), MarketType.C );
#endif
        }
        private void AddBrandFromEnumType(MemberInfo[] ms, MarketType mt)
        {
            foreach (MemberInfo mi in ms)
            {
                FieldInfo fi = mi as FieldInfo;
                //!!EnumDescAttribute�͂O���珇�ɕ��񂾏ꍇ�����Ή����Ă��Ȃ������̂ŋ}篁B��ł܂Ƃ߂邱��
                if (fi != null && fi.IsStatic && fi.IsPublic)
                {
                    EnumValueAttribute a = (EnumValueAttribute)(fi.GetCustomAttributes(typeof(EnumValueAttribute), false)[0]);
                    int code = (int)fi.GetValue(null);
                    BasicBrand br = new BasicBrand(a.Description, code, mt);
                    br.Unit = 0;
                    _data[code] = br;
                }
            }
        }

        private void ReadHeader(string line)
        {
            _lastUpdatedDate = ReadDateFromHeader(line);
        }
        private static int ReadDateFromHeader(string line)
        {
            foreach (string e in line.Split(','))
            {
                int eq = e.IndexOf('=');
                if (eq == -1) throw new IOException("index.txt�̃w�b�_�t�H�[�}�b�g�G���[");
                string name = e.Substring(0, eq);
                string value = e.Substring(eq + 1);
                if (name == "date")
                    return Int32.Parse(value); //���܂̂Ƃ��낱�ꂵ���f�[�^�Ȃ�
            }
            return -1;
        }

        public static int GuessDate(MemoryStream strm)
        {
            strm.Position = 0;
            TextReader s = new StreamReader(strm, Encoding.Default);
            string line = s.ReadLine();
            int dt = ReadDateFromHeader(line);
            return dt;
        }
    }
/*
        internal class BrandCollection {
            private Hashtable _data; //code�̒l����AbstractBrand��
            private Hashtable _farms; //AbstractBrand����Farm�ց@Farm�̌��𐧌�����@�\�͂܂�
	
            private int _lastUpdatedDate;

            private int _stockBrandCount;
            private ArrayList _derivedBrands;

            //�����Ă���DataFarm�̐���farmCapacity�𒴂��Ȃ��悤�ɂ���
            private int _farmCapacity;
            private int _nextRefCount;

            public BrandCollection() {
                _data = new Hashtable();
                _farms = new Hashtable();
                _derivedBrands = new ArrayList();

                _farmCapacity = 100;
                _nextRefCount = 1;
            }
            public int TotalCount {
                get {
                    return _data.Count;
                }
            }
            //�ʖ����̐�
            public int StockBrandCount {
                get {
                    return _stockBrandCount;
                }
            }
            public int DerivedBrandCount {
                get {
                    return _derivedBrands.Count;
                }
            }
            public int LastUpdatedDate {
                get {
                    return _lastUpdatedDate;
                }
            }

            public void Load(string filename) {
                //index�t�@�C������ǂ�ō\�z
                //!!�o�[�W�����`�F�b�N�K�v
                StreamReader sr = new StreamReader(filename, Encoding.Default);
                string line = sr.ReadLine();
                ReadHeader(line);
                line = sr.ReadLine();

                int n = 1;
                ArrayList splits = new ArrayList();
                _stockBrandCount = 0;

                _data.Clear();
                _farms.Clear();

                LoadBuiltIns();

                while(line!=null) {
                    string[] d = line.Split(',');
                    if(d.Length<4) throw new IOException("index file format error: line"+ n);
                    try {
                        int code = Int32.Parse(d[0]);
                        BasicBrand br = new BasicBrand(d[1], code, (MarketType)Enum.Parse(typeof(MarketType), d[2], true));
                        br.Unit = Int32.Parse(d[3]);
                        _data[code] = br;
                        _stockBrandCount++;

                        for(int i=4; i<d.Length; i++) {
                            string col = d[i];
                            if(col=="N225") br.Nikkei225 = true;
                            if(col=="A500") br.Active500 = true;
                            if(col=="OBS") br.Obsolete = true;
                            Debug.Assert(col.Length>0);
                            if(col.Length>0 && col[0]=='S') splits.Add(new SplitInfo(d[i]));
                        }
                        if(splits.Count>0) {
                            br.SplitInfo = (SplitInfo[])splits.ToArray(typeof(SplitInfo));
                            splits.Clear();
                        }
                    }
                    catch(Exception ex) {
                        Debug.WriteLine(String.Format("{0} �s{1} {2}", ex.Message, n, line));
                    }
                    line = sr.ReadLine();
                    n++;
                    //Debug.WriteLine(n);
                }
                sr.Close();
            }

            public AbstractBrand FindBrand(int code) {
                return (AbstractBrand)_data[code];
            }
            public AbstractBrand FindNextBrand(int code){
                // TODO:10000�͒萔�ɏo��
                for(int i=code+1;i<10000;i++){
                    if(_data.ContainsKey(i)){
                        return (AbstractBrand)_data[i];
                    }
                }
                return FindNextBrand(0);
            }
            public AbstractBrand FindPrevBrand(int code){
                for(int i=code-1;i>0;i--){
                    if(_data.ContainsKey(i)){
                        return (AbstractBrand)_data[i];
                    }	
                }
                return FindPrevBrand(10000);
            }
		
            public void Append(AbstractBrand br) {
                _data[br.Code] = br;
                if(br is DerivedBrand) _derivedBrands.Add(br);
            }
            public ArrayList DerivedBrands {
                get {
                    return _derivedBrands;
                }
            }

            public IDictionaryEnumerator GetEnumerator() {
                return _data.GetEnumerator();
            }
            public DataFarm ReserveFarm(AbstractBrand br) {
                return ReserveFarm(br, Env.CurrentIndicators.Format);
            }

            public DataFarm ReserveFarm(AbstractBrand br, ChartFormat fmt) {
                DataFarm f = (DataFarm)_farms[br];
                if(f!=null) { //�L���b�V���Ƀq�b�g
                    if(f.IsEmpty) f.LoadFor(br);
                    if(br.ReferenceValue<_nextRefCount-1)
                        br.ReferenceValue = _nextRefCount++;
                    return f;
                }

                if(_farms.Count>=_farmCapacity) CleanFarms();

                if(br is BasicBrand)
                    f = CreateFarm((BasicBrand)br, fmt);
                else
                    f = CreateFarm((DerivedBrand)br, fmt);

                _farms[br] = f;
                br.ReferenceValue = _nextRefCount++;
                return f;
            }
            public bool HasFarm(AbstractBrand br) {
                return _farms.Contains(br);
            }

            private void CleanFarms() {
                ArrayList t = new ArrayList(_farms.Keys);
                t.Sort(new BrandComparer());

                Hashtable newfarms = new Hashtable();
                for(int n = t.Count/2; n<t.Count; n++) {
                    AbstractBrand br = (AbstractBrand)t[n];
                    br.ReferenceValue = n;
                    newfarms[br] = _farms[br];
                }
                _nextRefCount = t.Count;
                _farms = newfarms;
            }

            //�L���b�V���Ƃ͖��֌W��DataFarm���쐬
            public DataFarm CreateDailyFarm(AbstractBrand br, int extra_dates) {
                DailyDataFarm f = new DailyDataFarm();
                f.LoadFor(br, extra_dates);
                return f;
            }
            public DataFarm CreateFarm(BasicBrand br, ChartFormat format) {
                DataFarm f;
                if(Util.IsDailyBased(format))
                    f = new DailyDataFarm();
                else if(format==ChartFormat.Weekly)
                    f = new WeeklyDataFarm();
                else	// Monthly
                    f = new MonthlyDataFarm();
                f.LoadFor(br);
                return f;
            }
            public DataFarm CreateFarm(DerivedBrand br, ChartFormat format) {
                DerivedDataFarm f = new DerivedDataFarm(br, format);
                f.LoadFor(br);
                return f;
            }

            private class BrandComparer : IComparer {
                public int Compare(object x, object y) {
                    return ((AbstractBrand)x).ReferenceValue - ((AbstractBrand)y).ReferenceValue;
                }

            }

            public void ClearAllFarms() {
                _farms.Clear();
                _nextRefCount = 1;
    #if DOJIMA
                Dojima.DojimaUtil.HalfDailyDataFarmCache.ClearAll();
    #endif
            }

            public ICollection Values {
                get {
                    return _data.Values;
                }
            }

            private void LoadBuiltIns() {
                AddBrandFromEnumType( typeof(BuiltInIndex).GetMembers(), MarketType.B );
    #if DOJIMA
                AddBrandFromEnumType( typeof(CommodityIndex).GetMembers(), MarketType.C );
    #endif
            }
            private void AddBrandFromEnumType(MemberInfo[] ms, MarketType mt) {
                foreach(MemberInfo mi in ms) {
                    FieldInfo fi = mi as FieldInfo;
                    //!!EnumDescAttribute�͂O���珇�ɕ��񂾏ꍇ�����Ή����Ă��Ȃ������̂ŋ}篁B��ł܂Ƃ߂邱��
                    if(fi!=null && fi.IsStatic && fi.IsPublic) {
                        EnumValueAttribute a = (EnumValueAttribute)(fi.GetCustomAttributes(typeof(EnumValueAttribute), false)[0]);
                        int code = (int)fi.GetValue(null);
                        BasicBrand br = new BasicBrand(a.Description, code, mt);
                        br.Unit = 0;
                        _data[code] = br;
                    }
                }
            }

            private void ReadHeader(string line) {
                _lastUpdatedDate = ReadDateFromHeader(line);
            }
            private static int ReadDateFromHeader(string line) {
                foreach(string e in line.Split(',')) {
                    int eq = e.IndexOf('=');
                    if(eq==-1) throw new IOException("index.txt�̃w�b�_�t�H�[�}�b�g�G���[");
                    string name = e.Substring(0, eq);
                    string value = e.Substring(eq+1);
                    if(name=="date")
                        return Int32.Parse(value); //���܂̂Ƃ��낱�ꂵ���f�[�^�Ȃ�
                }
                return -1;
            }

            public static int GuessDate(MemoryStream strm) {
                strm.Position = 0;
                TextReader s = new StreamReader(strm, Encoding.Default);
                string line = s.ReadLine();
                int dt = ReadDateFromHeader(line);
                return dt;
            }
        }
    */
}
