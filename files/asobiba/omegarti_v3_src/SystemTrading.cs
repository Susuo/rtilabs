/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Threading;
using System.Collections;
using System.Diagnostics;

using Travis.Util;

using Zanetti.Data;
using Zanetti.Config;

//�X�N���[�j���O�ƃZ�I���[���؂̋��ʋ@�\���܂Ƃ߂�
namespace Zanetti.SystemTrading
{
	//�����񋓊֌W

	[EnumDesc(typeof(FilterType))]
	internal enum FilterType {
		[EnumValue(Description="���ׂĂ̖���")] None,
		[EnumValue(Description="���o���ύ̗p����")] Nikkei225,
		[EnumValue(Description="���ϔ���������500����")] Active500,
		[EnumValue(Description="���؁E��؂P��")] Div1,
		[EnumValue(Description="���؁E��؂P���ȊO")] NotDiv1,
		[EnumValue(Description="���C�ɓ���̃t�H���_")] Favorite
	}

	[EnumDesc(typeof(TradingType))]
	internal enum TradingType {
		[EnumValue(Description="����")] Long,
		[EnumValue(Description="����")] Short
	}

	[EnumDesc(typeof(EntryType))]
	internal enum EntryType {
		[EnumValue(Description="�����I�l")] TodayClose,
		[EnumValue(Description="�����n�l")] TomorrowOpen,
		[EnumValue(Description="�t�w�l")] Gyakusashine
	}

    //hacked by rti
    [EnumDesc(typeof(IntervalType))]
    internal enum IntervalType
    {
        [EnumValue(Description = "��d�����Ă��甃��")]
        ClosingTrade ,
        [EnumValue(Description = "��������")]
        EveryDay
    }

    [EnumDesc(typeof(AnalystType))]
    internal enum AnalystType
    {
        [EnumValue(Description = "�V�O�i��")]
        Signal,
        [EnumValue(Description = "��d����")]
        Exit,
        [EnumValue(Description = "����")]
        Losscut,
        [EnumValue(Description = "�t�w�l")]
        Gyakusashine
    }
    //-----


	internal abstract class BrandEnumerator {
		
		//�v���O���X�o�[�̃R���g���[��������̂ŁACount�ŕԂ����l����Next���ĂсA���ۂɏ����Ώۂɂ���̂�Check�ɒʂ�����݂̂Ƃ������[���ł���
		public abstract AbstractBrand Next();
		
		public abstract bool Check(AbstractBrand br);

		public abstract int Count {
			get;
		}
		public abstract string Description {
			get;
		}
	}

	internal class AllBrandEnumerator : BrandEnumerator {
		private IDictionaryEnumerator _e;
		public AllBrandEnumerator() {
			_e = Env.BrandCollection.GetEnumerator();
		}
		public override AbstractBrand Next() {
			return _e.MoveNext()? (AbstractBrand)_e.Value : null;
		}
		public override int Count {
			get {
				return Env.BrandCollection.TotalCount;
			}
		}
		public override bool Check(AbstractBrand br) {
			return br is BasicBrand && !br.IsBuiltIn;
		}
		public override string Description {
			get {
				return "�S����";
			}
		}

	}

	internal class FilteredBrandEnumerator : AllBrandEnumerator {
		private FilterType _filter;
		public FilteredBrandEnumerator(FilterType t) {
			_filter = t;
		}
		public FilterType FilterType {
			get {
				return _filter;
			}
		}
		public override bool Check(AbstractBrand br0) {
			if(!base.Check(br0)) return false;
			BasicBrand br = br0 as BasicBrand;

			switch(_filter) {
				case FilterType.Div1:
					return br.Market==MarketType.T1 || br.Market==MarketType.O1;
				case FilterType.NotDiv1:
					return br.Market!=MarketType.T1 && br.Market!=MarketType.O1;
				case FilterType.Nikkei225:
					return br.Nikkei225;
				case FilterType.Active500:
					return br.Active500;
				default:
					return br.Market!=MarketType.B; //�w���͓��R�܂߂Ȃ�
			}
		}
		public override string Description {
			get {
				return EnumDescAttribute.For(typeof(FilterType)).GetDescription(_filter);
			}
		}

	}


	internal class SingleBrandEnumerator : BrandEnumerator {
		private AbstractBrand _br;
		private bool _finished;
		public SingleBrandEnumerator(AbstractBrand br) {
			_br = br;
		}
		public override AbstractBrand Next() {
			if(_finished)
				return null;
			else {
				_finished = true;
				return _br;
			}
		}
		public override int Count {
			get {
				return 1;
			}
		}
		public override bool Check(AbstractBrand br) {
			return true;
		}
		public override string Description {
			get {
				return _br.Name;
			}
		}

	}

	internal class BookmarkedBrandEnumerator : BrandEnumerator {
		private BookmarkFolder _bookmarkFolder;
		private ArrayList _brands;
		private int _index;
		public BookmarkedBrandEnumerator(BookmarkFolder f) {
			_bookmarkFolder = f;
			_brands = new ArrayList();
			_bookmarkFolder.GatherTo(_brands, GatherOption.Brand);
			_index = 0;
		}
		public override AbstractBrand Next() {
			AbstractBrand r = null;
			while(r==null && _index<_brands.Count) {
				object t = _brands[_index++];
				r = t as AbstractBrand;
			} 
			return r;
		}
		public override int Count {
			get {
				return _brands.Count;
			}
		}
		public override bool Check(AbstractBrand br) {
			return true;
		}
		public override string Description {
			get {
				return "�u�b�N�}�[�N " + _bookmarkFolder.Name;
			}
		}



	}

	//���f���ĉ�����������N���X
	internal abstract class SystemTradingExecutor {
		protected string _errorMessage;
		protected string _name;
		protected BrandEnumerator _brandEnumerator;
		protected IntPtr _notifyTarget;
		protected Thread _thread;

		protected SystemTradingExecutor(string name) {
			_name = name;
		}
		public BrandEnumerator BrandEnumerator {
			get {
				return _brandEnumerator;
			}
			set {
				_brandEnumerator = value;
			}
		}

		public string Name {
			get {
				return _name;
			}
		}
		public string ErrorMessage {
			get {
				return _errorMessage;
			}
		}
		public abstract SystemTradingResult Result {
			get;
		}

		public void Execute() {
			//������
			BeforeExecute();
			this.Result.BrandDescription = _brandEnumerator.Description;

			AbstractBrand br = _brandEnumerator.Next();
			while(br!=null) {
				try {
					if(_brandEnumerator.Check(br)) {
						ExecuteBrandResult t = ExecuteBrand(br);
						this.Result.CheckedBrandCount++; //!!����͂����ŃC���N�������g���邪�A�G���[�̏��͔h���N���X���Ƃ����̂��킩��Â炢�d�l��
						if(t==ExecuteBrandResult.TooManyResult) return;
					}
					if(_notifyTarget!=IntPtr.Zero)
						Win32.SendMessage(_notifyTarget, AsyncConst.WM_ASYNCPROCESS, new IntPtr(br.Code), new IntPtr(AsyncConst.LPARAM_PROGRESS_SUCCESSFUL));

					br = _brandEnumerator.Next();
				}
				catch(TradeDataOverflowException ) {
					Debug.WriteLine("Overflow in screening " + br.Code);
					this.Result.AddDataErrorBrand(br);

                    //hacked  by rti
                    //�G���[�����������ꍇ�͔�΂��Ď��̃f�[�^����������悤�ɂ���.
                    //�������Ȃ��Ɩ������[�v�ɂȂ�
                    br = _brandEnumerator.Next();
                    //----

				}
				catch(Exception ex) {
					Debug.WriteLine(ex.StackTrace);
					_errorMessage = ex.Message;
                    //hacked by rti ---
//					if(_notifyTarget!=IntPtr.Zero)
//						Win32.SendMessage(_notifyTarget, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_ERROR));
                    if (_notifyTarget != IntPtr.Zero)
                    {
                        Win32.SendMessage(_notifyTarget, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_ERROR));
                    }
                    else
                    {
                        //�G���[�X�L�b�v
                        br = _brandEnumerator.Next();
                    }
				}
			}

			AfterExecute();
		}

		//���s�̃T�|�[�g
		protected virtual void BeforeExecute() { }
		protected virtual void AfterExecute() { }
		protected abstract ExecuteBrandResult ExecuteBrand(AbstractBrand br);

		//�񓯊����s
		public void AsyncExecute(IntPtr target) {
			_notifyTarget = target;
			_thread = new Thread(new ThreadStart(Run));
			_thread.Start();
		}
		private void Run() {
			try {
				Execute();
				if(_notifyTarget!=IntPtr.Zero) Win32.SendMessage(_notifyTarget, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_FINISHED));
			}
			catch(Exception ex) {
				_errorMessage = ex.Message;
				Util.SilentReportCriticalError(ex);
				if(_notifyTarget!=IntPtr.Zero) Win32.SendMessage(_notifyTarget, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_ERROR));
			}
		}

		public void Abort() {
			if(_thread!=null)
				_thread.Abort();
		}
	}

	internal enum ExecuteBrandResult {
		Succeeded,
		Ignored,
		DataError,
		Filtered,
		TooManyResult
	}

	internal abstract class SystemTradingResult {
		protected ArrayList _data;
		protected string _brandDescription;
		protected bool _sortRequired;
		protected int _checkedCount;
		protected ArrayList _dataErrorBrands;
		protected bool _tooManyResults;
		
		public SystemTradingResult(int capacity) {
			_data = new ArrayList(capacity);
			_dataErrorBrands = new ArrayList();
		}

		public void AddDataErrorBrand(AbstractBrand br) {
			_dataErrorBrands.Add(br);
		}

		public SystemTradingResultEntry GetAt(int index) {
			if(_sortRequired) {
				_data.Sort();
				_sortRequired = false;
			}
			return (SystemTradingResultEntry)_data[index];
		}
		public string BrandDescription {
			get {
				return _brandDescription;
			}
			set {
				_brandDescription = value;
			}
		}

		public int CheckedBrandCount {
			get {
				return _checkedCount;
			}
			set {
				_checkedCount = value;
			}
		}
		public bool HasTooManuResults {
			get {
				return _tooManyResults;
			}
			set {
				_tooManyResults = value;
			}
		}

		public IList DataErrorBrands {
			get {
				return _dataErrorBrands;
			}
		}
		
		public int ResultCount {
			get {
				return _data.Count;
			}
		}
	}

	internal abstract class SystemTradingResultEntry : IComparable {
		protected AbstractBrand _brand;

		public SystemTradingResultEntry(AbstractBrand br) {
			_brand = br;
		}

		public AbstractBrand Brand {
			get {
				return _brand;
			}
		}

		public abstract int CompareTo(object obj);
	}

}
