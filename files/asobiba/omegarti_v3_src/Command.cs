/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Xml;
using System.IO;
using System.Drawing;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Security.Cryptography;

using Zanetti.Arithmetic;
using Zanetti.Data;
using Zanetti.DataSource;
using Zanetti.Forms;
using Zanetti.Config;
using Zanetti.SystemTrading.Screening;
using Zanetti.SystemTrading.AutoTrading;
using Zanetti.Prediction;
using Zanetti.Indicators;
using Zanetti.UI;
using Zanetti.Parser;

using Travis.Storage;
using Travis.Util;
using Travis.PKI;

using Crownwood.DotNetMagic.Common;
using Crownwood.DotNetMagic.Controls;
using Crownwood.DotNetMagic.Docking;
//using Crownwood.DotNetMagic.Menus;


namespace Zanetti.Commands {
	/* �V�����R�}���h��ǉ�����Ƃ��͎��̂��Ƃ����邱��
	 * 
	 * 1) Command.CID��Enum�l�ɒǉ�
	 * 2) Command#Init, Exec���C��
	 * 3) �R�}���h�����j���[��c�[���o�[�����ꍇ�́AMenuItem��Command�̊֘A�t���Ȃǂ�ǉ�
	 */ 
	internal enum CommandResult {
		Succeeded,
		Ignored,
		Failed
	}
	//�L�[�J�X�^�}�C�Y�_�C�A���O�ł͂��̏��Ԃŕ\�������
	internal enum CID {
		None = 0 ,
		//�_�C�A���O�\��
		ShowDownloadDialog,
		ShowInitializeDialog,
		ShowPrintDialog,
		ShowCustomizeDialog,
		ShowScreeningDialog,
		ShowPredictionDialog,
		ShowAutoTradingDialog,
		ShowAddBookmarkDialog,
		ShowAboutBox,
		ShowExtensionKitDialog,
		ShowTestKitDialog,
		ShowSearchBrandDialog,
		ShowKeyConfigDialog,
        ShowEnvironmentDialog,
		RegisterUserCode,
		//�X�^�C���ύX
		SetStyleDaily,
		SetStyleWeekly,
		SetStyleMonthly,
		SetStyleHalfDaily, //�����p
		ShrinkCandleWidth,
		ExpandCandleWidth,
		ToggleLogarithmStyle,
		ToggleInverseStyle,
		ClearFreeLines,
		ToggleBookmarkPane,
        ToggleSplitAdjuster,
        ToggleScaleLock,
		TogglePriceEnabled,
		ToggleAccumulativeVolumeEnabled,
        //hacked by rti
        ToggleRealTimeUpdate,
        //-------------
		//�ړ�
		MoveToFirstDate,
		MoveToLastDate,
		MoveToPrevPage,
		MoveToNextPage,
		MoveToPrevDate, //���E�\�N�P�{���̈ړ�
		MoveToNextDate, 
		ShowPrevBrand, //���݌����Ă���y�C���ł̖����ړ�
		ShowNextBrand,  
		ShowPrevCode,  //�R�[�h���ł̈ړ�
		ShowNextCode,   
		Back,
		Front,
		//�f�[�^�֌W
		UpdateCurrentData,
		DownloadIndexFile,
		ExportBrand,
		ExportHalfDaily,
		//Web
		OpenWeb,
		OpenCompanyInfoSite_Yahoo,
		OpenCompanyInfoSite_Infoseek,
		OpenCompanyInfoSite_Nikkei,
		OpenCompanyInfoSite_Livedoor,
		//���̑�
		Quit,
		ReloadKit,

		//������O�ɂ���R�}���h�̓L�[�J�X�^�}�C�Y���̈ꗗ�ŏo�Ă���B��̃R�}���h�͏o�Ă��Ȃ��B
		VisibleCount,
		ShowInputCode,
		ShowSpecifiedBrand,

		//�T�|�[�g�p�R�}���h
#if ENABLE_SUPPORT_COMMAND
		SupRebuildIndex,
		SupCreateKey,
		SupReload,
		SupCreateCert,
		SupStatistics,
		SupShrinkData,
#endif
		//�I�[
		Count
	}

	internal class Command : ICloneable {
		private CID _id;
		private string _description;
		private Keys _shortcut;

		public CID ID {
			get {
				return _id;
			}
		}
		public string Description {
			get {
				return _description;
			}
		}
		public Keys Shortcut {
			get {
				return _shortcut;
			}
		}
		public object Clone() {
			return new Command(_id, _description, _shortcut);
		}

		/// �����CommandCollection���炵���Ă΂Ȃ��B
		public void SetChortcut(Keys key) {
			_shortcut = key;
		}

		public bool Visible {
			get {
				return _id < CID.VisibleCount;
			}
		}



		public Command(CID id, string description, Keys shortcut) {
			_id = id;
			_description = description;
			_shortcut = shortcut;
		}

		public CommandResult Exec() {
			switch(_id) {
				//�_�C�A���O�\��
				case CID.ShowDownloadDialog:
					return CommandExec.ShowDownloadDialog();
				case CID.ShowInitializeDialog:
					return CommandExec.ShowInitializeDialog();
				case CID.ShowPrintDialog:
					return CommandExec.ShowPrintDialog();
				case CID.ShowCustomizeDialog:
					return CommandExec.ShowCustomizeDialog();
				case CID.ShowScreeningDialog:
					return CommandExec.ShowScreeningDialog();
				case CID.ShowPredictionDialog:
					return CommandExec.ShowPredictionDialog();
				case CID.ShowAutoTradingDialog:
					return CommandExec.ShowAutoTradingDialog();
				case CID.ShowAddBookmarkDialog:
					return CommandExec.ShowAddBookmarkDialog();
				case CID.ShowAboutBox:
					return CommandExec.AboutBox();
				case CID.ShowExtensionKitDialog:
					return CommandExec.ShowExtensionKitDialog();
				case CID.ShowTestKitDialog:
					return CommandExec.ShowKitTestDialog();
				case CID.ShowSearchBrandDialog:
					return CommandExec.ShowSearchBrandDialog();
				case CID.ShowKeyConfigDialog:
					return CommandExec.ShowKeyConfigDialog();
                case CID.ShowEnvironmentDialog:
                    return CommandExec.ShowEnvironmentDialog();
                case CID.RegisterUserCode:
                    return CommandExec.RegisterUserCode();
				//�X�^�C���ύX
				case CID.SetStyleDaily:
					return CommandExec.SetChartFormat(ChartFormat.Daily);
				case CID.SetStyleWeekly:
					return CommandExec.SetChartFormat(ChartFormat.Weekly);
				case CID.SetStyleMonthly:
					return CommandExec.SetChartFormat(ChartFormat.Monthly);
#if DOJIMA
				case CID.SetStyleHalfDaily:
					return CommandExec.SetChartFormat(ChartFormat.HalfDaily);
				case CID.ExportHalfDaily:
					return CommandExec.ExportHalfDailyData();
#endif
				case CID.ShrinkCandleWidth:
					return CommandExec.ShrinkCandleWidth();
				case CID.ExpandCandleWidth:
					return CommandExec.ExpandCandleWidth();
				case CID.ToggleLogarithmStyle:
					return CommandExec.SetLogarithmStyle(!Env.Preference.LogScale);
				case CID.ToggleInverseStyle:
					return CommandExec.SetInverseStyle(!Env.Preference.InverseChart);
				case CID.ClearFreeLines:
					return CommandExec.ClearFreeLines();
                //hacked by rti
                case CID.ToggleRealTimeUpdate:
                    return CommandExec.RealTimeUpdater();
                //------------
				case CID.ToggleBookmarkPane:
					return CommandExec.ShowBookmarkPane(!Env.Frame.IsBookmarkPaneVisible);
                case CID.ToggleSplitAdjuster:
                    return CommandExec.SetSplitAdjuster(!Env.Preference.AdjustSplit);
                case CID.ToggleScaleLock:
                    return CommandExec.SetScaleLock(!Env.Preference.ScaleLock);
				case CID.TogglePriceEnabled:
					return CommandExec.SetPriceEnabled(!Env.Preference.ShowPrice);
				case CID.ToggleAccumulativeVolumeEnabled:
					return CommandExec.SetAccumulativeVolumeEnabled(!Env.Preference.ShowAccumulativeVolume);
				//�ړ�
                case CID.MoveToFirstDate:
					return CommandExec.JumpDate(Keys.Home);
				case CID.MoveToLastDate:
					return CommandExec.JumpDate(Keys.End);
				case CID.MoveToNextPage:
					return CommandExec.JumpDate(Keys.PageDown);
				case CID.MoveToPrevPage:
					return CommandExec.JumpDate(Keys.PageUp);
				case CID.MoveToNextDate:
					return CommandExec.JumpDate(Keys.Right);
				case CID.MoveToPrevDate:
					return CommandExec.JumpDate(Keys.Left);
				case CID.ShowNextBrand:
					return CommandExec.ShowNextBrand();
				case CID.ShowPrevBrand:
					return CommandExec.ShowPrevBrand();
				case CID.ShowNextCode:
					return CommandExec.ShowNextCode();
				case CID.ShowPrevCode:
					return CommandExec.ShowPrevCode();
				case CID.Back:
					return CommandExec.Back(1);
				case CID.Front:
					return CommandExec.Front(1);
					//�f�[�^�֌W
				case CID.DownloadIndexFile:
					return CommandExec.DownloadIndexFile();
				case CID.UpdateCurrentData:
					return CommandExec.UpdateCurrentData();
				case CID.ExportBrand:
					return CommandExec.ExportBrand();
					//Web
				case CID.OpenWeb:
					return CommandExec.OpenWeb();
				case CID.OpenCompanyInfoSite_Yahoo:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Yahoo);
				case CID.OpenCompanyInfoSite_Infoseek:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Infoseek);
				case CID.OpenCompanyInfoSite_Nikkei:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Nikkei);
				case CID.OpenCompanyInfoSite_Livedoor:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Livedoor);
					//���̑�
				case CID.Quit:
					return CommandExec.Quit();
				case CID.ReloadKit:
					return CommandExec.Reload(null);
#if ENABLE_SUPPORT_COMMAND
				case CID.SupRebuildIndex: {
					Data.StaticGrouping.Run();
					return CommandResult.Succeeded;
				}
				case CID.SupReload:
					return CommandExec.Reload(null);
				case CID.SupCreateKey:
					return CommandExec.CreateKey();
				case CID.SupCreateCert:
					return CommandExec.GenerateUserCode();
				case CID.SupStatistics:
					return CommandExec.StatisticsTest();
				case CID.SupShrinkData:
					return CommandExec.ShrinkData();
#endif
				default:
					Debug.WriteLine("Unknown command " + _id);
					return CommandResult.Ignored;
			}
		}
	}
	internal class CommandCollection : ICloneable {
		
		//Collection
		private Hashtable _keyToCommand;
		private Command[] _idArray;

		public CommandCollection(StorageNode keys) {
			Init(keys);
		}
		public void Reset() {
			Init(null);
		}

		private CommandCollection() {
			_keyToCommand = new Hashtable();
			_idArray = new Command[(int)CID.Count];
		}
		public object Clone() {
			CommandCollection r = new CommandCollection();
			foreach(Command cmd in _idArray) {
				Command nc = (Command)cmd.Clone();
				r._idArray[(int)nc.ID] = nc;
				if(nc.Shortcut!=Keys.None) r._keyToCommand[nc.Shortcut] = nc;
			}
			return r;
		}

		private void Init(StorageNode keys) {
			_keyToCommand = new Hashtable();
			_idArray = new Command[(int)CID.Count];

			//�_�C�A���O�\��
			AddCommand(CID.ShowDownloadDialog,      "�f�[�^�̃_�E�����[�h",     keys, Keys.Control|Keys.O);
			AddCommand(CID.ShowInitializeDialog,    "�f�[�^�̏�����",           keys, Keys.None);
			AddCommand(CID.ShowPrintDialog,         "���",                     keys, Keys.Control|Keys.P);
			AddCommand(CID.ShowCustomizeDialog,     "�J�X�^�}�C�Y",             keys, Keys.Control|Keys.T);
			AddCommand(CID.ShowScreeningDialog,     "�X�N���[�j���O",           keys, Keys.Control|Keys.S);
			AddCommand(CID.ShowAutoTradingDialog,   "������������",             keys, Keys.Control|Keys.V);
			AddCommand(CID.ShowPredictionDialog,    "�Z�I���[����",             keys, Keys.None);
			AddCommand(CID.ShowAddBookmarkDialog,   "�u�b�N�}�[�N�ւ̒ǉ�",     keys, Keys.None);
			AddCommand(CID.ShowAboutBox,            "�o�[�W�������̕\��",     keys, Keys.None);
			AddCommand(CID.ShowKeyConfigDialog,     "�L�[���蓖�ẴJ�X�^�}�C�Y", keys, Keys.None);
            AddCommand(CID.ShowEnvironmentDialog,   "���ݒ�",                 keys, Keys.None);
            AddCommand(CID.ShowExtensionKitDialog, "�g���L�b�g�ꗗ", keys, Keys.None);
            AddCommand(CID.ShowTestKitDialog,       "�g���L�b�g�e�X�g",         keys, Keys.None);
			AddCommand(CID.ShowSearchBrandDialog,   "�����̌���",               keys, Keys.Control|Keys.F);
			AddCommand(CID.RegisterUserCode,        "Contribution Certificate�̓o�^", keys, Keys.None);
			//�X�^�C���ύX
			AddCommand(CID.SetStyleDaily,           "�����\��",                 keys, Keys.Control|Keys.D);
			AddCommand(CID.SetStyleWeekly,          "�T���\��",                 keys, Keys.Control|Keys.W);
			AddCommand(CID.SetStyleMonthly,         "�����\��",                 keys, Keys.Control|Keys.M);
			AddCommand(CID.SetStyleHalfDaily,       "�������\��",               keys, Keys.Control|Keys.H);
			AddCommand(CID.ShrinkCandleWidth,       "���E�\�N���̏k��",         keys, Keys.Control|Keys.OemMinus);
			AddCommand(CID.ExpandCandleWidth,       "���E�\�N���̊g��",         keys, Keys.Control|Keys.Oemplus);
			AddCommand(CID.ToggleLogarithmStyle,    "�ΐ��\���ؑ�",             keys, Keys.Control|Keys.L);
			AddCommand(CID.ToggleInverseStyle,      "�㉺���]�\���ؑ�",         keys, Keys.Control|Keys.R);
			AddCommand(CID.ClearFreeLines,          "���R�����̃N���A",         keys, Keys.None);
			AddCommand(CID.ToggleBookmarkPane,      "�u�b�N�}�[�N�\���̐ؑ�",   keys, Keys.Control|Keys.B);
            AddCommand(CID.ToggleSplitAdjuster,     "�����̍l���ؑ�",           keys, Keys.None);
            AddCommand(CID.ToggleScaleLock,         "�k�ڂ̌Œ�",               keys, Keys.None);
			AddCommand(CID.TogglePriceEnabled,      "���i�Ɩ}��̕\��",         keys, Keys.None);
			AddCommand(CID.ToggleAccumulativeVolumeEnabled, "���i�яo�����̕\��",keys,Keys.None);
            //hacked by rti
            AddCommand(CID.ToggleRealTimeUpdate, "���A���^�C���X�V���s��", keys, Keys.None);
            //-------------------
			//�ړ�
            AddCommand(CID.MoveToFirstDate,         "�ŏ��̓��t�ֈړ�",         keys, Keys.Home);
			AddCommand(CID.MoveToLastDate,          "�Ō�̓��t�ֈړ�",         keys, Keys.End);
				//���̃u���b�N��KeyConfigDialog�ŃJ�X�^�}�C�Y�ł���B�P�R�}���h���̓Ɨ��ł͂Ȃ����Ƃɒ���
			AddCommand(CID.MoveToNextPage,          "�P��ʐ�̓��t�ֈړ�",     keys, Keys.None);
			AddCommand(CID.MoveToPrevPage,          "�P��ʑO�̓��t�ֈړ�",     keys, Keys.None);
			AddCommand(CID.MoveToNextDate,          "�P������",                 keys, Keys.None);
			AddCommand(CID.MoveToPrevDate,          "�P���O��",                 keys, Keys.None);
			AddCommand(CID.ShowNextBrand,           "���̕\��������",           keys, Keys.None);
			AddCommand(CID.ShowPrevBrand,           "�O�̕\��������",           keys, Keys.None);
			AddCommand(CID.ShowNextCode,            "���̃R�[�h������",         keys, Keys.None);
			AddCommand(CID.ShowPrevCode,            "�O�̃R�[�h������",         keys, Keys.None);
			AddCommand(CID.Back,                    "�߂�",                     keys, Keys.Back);
			AddCommand(CID.Front,                   "�i��",                     keys, Keys.Shift|Keys.Back);
			
			AddCommand(CID.ShowInputCode,           "�R�[�h����",               keys, Keys.None);
			//�f�[�^�֌W
			AddCommand(CID.DownloadIndexFile,       "�C���f�b�N�X�t�@�C���̃_�E�����[�h", keys, Keys.None);
			AddCommand(CID.UpdateCurrentData,       "���݂̖������X�V",         keys, Keys.F5);
			AddCommand(CID.ExportBrand,             "�f�[�^�̃G�N�X�|�[�g", keys, Keys.None);
			AddCommand(CID.ExportHalfDaily,         "�������f�[�^�̃G�N�X�|�[�g", keys, Keys.None);
			//Web
			AddCommand(CID.OpenWeb,                  "OmegaChart��Web���J��",   keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Yahoo,    "Yahoo�̊�Ə��y�[�W���J��",    keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Infoseek, "Infoseek�̊�Ə��y�[�W���J��", keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Nikkei,   "���o�̊�Ə��y�[�W���J��",     keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Livedoor, "livedoor�̊�Ə��y�[�W���J��", keys, Keys.None);
			//���̑�
			AddCommand(CID.Quit,                     "OmegaChart�̏I��", keys, Keys.Control|Keys.Shift|Keys.X);
			AddCommand(CID.ReloadKit,                "�g���L�b�g�̃����[�h", keys, Keys.None);
			//�T�|�[�g�p�R�}���h
#if ENABLE_SUPPORT_COMMAND
			AddCommand(CID.SupRebuildIndex,          "Active500�Ȃǂ�index�č쐬", keys, Keys.None);
			AddCommand(CID.SupReload,                "�����[�h",                   keys, Keys.None);
			AddCommand(CID.SupCreateKey,             "�L�[�̍č쐬",               keys, Keys.None);
			AddCommand(CID.SupCreateCert,            "Contribution Cert�쐬",      keys, Keys.None);
			AddCommand(CID.SupStatistics,            "Statistics",                 keys, Keys.None);
			AddCommand(CID.SupShrinkData,            "�f�[�^�k��",                 keys, Keys.None);
#endif
		}
		
		private void AddCommand(CID cid, string desc, StorageNode keys, Keys def) {
			Keys k = keys==null? def : Util.ParseKey(keys[cid.ToString()]);
			Command cmd = new Command(cid, desc, k);
			_idArray[(int)cid] = cmd;
			if(k!=Keys.None) _keyToCommand[k] = cmd;
		}
		public Command Get(CID cid) {
			Command r = _idArray[(int)cid];
			Debug.Assert(r!=null);
			return r;
		}
		public CommandResult Exec(CID cid) {
			Command cmd = Get(cid);
			Debug.Assert(cmd!=null);
			return cmd.Exec();
		}
		public CommandResult Exec(Keys key) {
			Command cmd = _keyToCommand[key] as Command;
			CID cid;
			if(cmd!=null)
				return cmd.Exec();
			else if((cid = Env.Options.KeyConfig.Translate(key))!=CID.None) {
				return Exec(cid);														
			}
			else if(Keys.D0 <= key && key <= Keys.D9) {
				CommandExec.PromptInputCode(false, (char)('0' + (key-Keys.D0)));
				return CommandResult.Succeeded;
			}
			else if(Keys.NumPad0 <= key && key <= Keys.NumPad9) {
				CommandExec.PromptInputCode(false, (char)('0' + (key-Keys.NumPad0)));
				return CommandResult.Succeeded;
			}
			else
				return CommandResult.Ignored;
		}
		public CommandResult Exec(ref Message m) {
			if(m.Msg==Win32.WM_KEYDOWN) {
				int k = m.WParam.ToInt32();
				return Exec((Keys)k | Control.ModifierKeys); //VK�V���[�Y�萔�Ɠ����Ɖ���
			}
			else if(m.Msg==Win32.WM_MOUSEWHEEL) {
				return ExecMouseWheel(m.WParam.ToInt32() >> 16);
			}
			else
				return CommandResult.Ignored;
		}

		public CommandResult ExecMouseWheel(int delta) {
			CID cid;
			if((cid = Env.Options.KeyConfig.TranslateWheel(delta))!=CID.None) {
				return Exec(cid);			
			}
			else
				return CommandResult.Ignored;
		}

		public IEnumerator Enum() {
			return _idArray.GetEnumerator();
		}

		public void SetKeyAssign(Command cmd, Keys newkey) {
			Debug.Assert(_keyToCommand[newkey]==null);
			_keyToCommand[cmd.Shortcut] = null;
			_keyToCommand[newkey] = cmd;
			cmd.SetChortcut(newkey);
		}

		public void SaveTo(StorageNode node) {
			foreach(Command cmd in _idArray) {
				if(cmd.Shortcut!=Keys.None)
					node[cmd.ID.ToString()] = Util.FormatShortcut(cmd.Shortcut);
			}
		}
	}

	internal class CommandExec {

		public static CommandResult ShowBrand(AbstractBrand b) {
			AbstractBrand old = Env.Frame.ChartCanvas.GetBrand();
			if(old!=null) Env.BrandHistory.Update(old);
			CommandResult r = ShowBrandInternal(b);
			return r;
		}
		private static CommandResult ShowBrandInternal(AbstractBrand b) {
			Env.Preference.ScaleLock = false;
			RefreshChart();			
			Env.Frame.ChartCanvas.LoadBrand(b, true);

            //hacked by rti
            //�\�����������؂�ւ����̂ŁA���A���^�C���ɊĎ�����������؂�ւ���
            if (Env.Frame.RealTimeUpdater.IsAlive())
            {
                Env.Frame.RealTimeUpdater.SetNextCode(b.Code, b.Market);
            }
            //-

			KitTestDialog kittest = Env.Frame.CurrentModelessDialog as KitTestDialog;
			if(kittest!=null) kittest.UpdateBrandName();
			return CommandResult.Succeeded;
		}
		public static CommandResult Back(int step) {
			if(!Env.BrandHistory.HasBack) return CommandResult.Ignored;
			AbstractBrand b = Env.BrandHistory.Back(Env.Frame.ChartCanvas.GetBrand(), step);
			return ShowBrandInternal(b);
		}
		public static CommandResult Front(int step) {
			if(!Env.BrandHistory.HasFront) return CommandResult.Ignored;
			AbstractBrand b = Env.BrandHistory.Redo(Env.Frame.ChartCanvas.GetBrand(), step);
			return ShowBrandInternal(b);
		}

		public static CommandResult ClearFreeLines() {
			Env.FreeLines.Clear(Env.Frame.ChartCanvas.GetBrand(), Env.CurrentIndicators.Format, Env.Preference.LogScale);
			Env.Frame.ChartCanvas.DrawingEngine.ClearScale();
			Env.Frame.ChartCanvas.Invalidate();
			return CommandResult.Succeeded;
		}
        //hacked by rti
        public static CommandResult RealTimeUpdater()
        {
            if (Env.Frame.RealTimeUpdater.IsAlive())
            {
                Env.Frame.RealTimeUpdater.Stop();
            }
            else
            {
                AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
                Env.Frame.RealTimeUpdater.SetNextCode(br.Code , br.Market);
                Env.Frame.RealTimeUpdater.Start();
            }
            return CommandResult.Succeeded;
        }
        //-----

		public static CommandResult ShowDownloadDialog() {
			return ShowDownloadDialog(null);
		}
		public static CommandResult ShowDownloadDialog(DownloadOrder order) {
			MainFrame f = Env.Frame;
			//!!ModelessDialog�@�\�͂܂�������

			DownloadWizard dlg = new DownloadWizard(order);

#if PRIVATE_FEATURE //������p�F�_�E�����[�h�����瑦hottest�ŃX�N���[�j���O
			if(Control.ModifierKeys==Keys.Control) {
				dlg.RunScreeningNow = true;
				dlg.Text = "������p�_�E�����[�h ���X�N���[�j���O";
			}
#endif
			dlg.Owner = f;
			Rectangle rc = f.DesktopBounds;
			dlg.Left = (rc.Left + rc.Right)/2 - dlg.Width/2;
			dlg.Top  = (rc.Top  + rc.Bottom)/2 - dlg.Height/2;
			//f.CurrentModelessDialog = dlg;
			dlg.Show();
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowInitializeDialog() {
			InitializeData dlg = new InitializeData();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				return RefreshChart();
			}
			else
				return CommandResult.Failed;
		}

        private static CodeInput _codeInput;

		public static void PromptInputCode(bool fromMenu, char ch) {
            if(_codeInput==null) _codeInput = new CodeInput(); //�Q��ڂ���ė��p
			if(!fromMenu) _codeInput.StockCode = (int)(ch - '0');
			if(_codeInput.ShowDialog(Env.Frame)==DialogResult.OK) {
                AbstractBrand br = Env.BrandCollection.FindBrand(_codeInput.StockCode);
				if(br!=null)
					ShowBrand(br);

				if(fromMenu)
					Util.Information(Env.Frame, "���̃��j���[���g�������ɁA�ʏ�̉�ʂłS���̐��l��ł����߂Ζ������w�肷�邱�Ƃ��ł��܂��B");
			}
		}

		public static CommandResult ShowPrintDialog() {
			new PrintSupport().ShowPrintDialog();
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowCustomizeDialog() {
			CustomizeDialog dlg = new CustomizeDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				RefreshChart(null);
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ExportBrand() {
            ExportDialog dlg = new ExportDialog();
            dlg.ShowDialog(Env.Frame);
            return CommandResult.Succeeded;
        }


		public static CommandResult ShowScreeningDialog() {
			if(Env.Frame.CurrentModelessDialog!=null) {
				Util.Warning(Env.Frame, "���݃_�E�����[�h�����s���̂��߁A�X�N���[�j���O�͂ł��܂���B");
				return CommandResult.Succeeded;
			}

			ScreeningDialog dlg = new ScreeningDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.ScreeningOrder.Result!=null) {
					if(dlg.ScreeningOrder.Result.ResultCount > 0) {
						ScreeningResultPane pane = new ScreeningResultPane(dlg.ScreeningOrder);
						AddDockingPane(pane, dlg.ScreeningOrder.Name, pane.RequiredWidth, IconConst.SEARCH);

					}
				}
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowPredictionDialog() {
			PredictionDialog dlg = new PredictionDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.Result!=null) {
					if(dlg.Result.HitCount==0) {
						Util.Warning(Env.Frame, "�����ɊY������f�[�^�͂���܂���ł����B");
					}
					else {
						PredictionResultPane pane = new PredictionResultPane(dlg.Result);
						AddDockingPane(pane, dlg.Result.Item.Title, pane.RequiredWidth, IconConst.COPY);
					}
				}
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowAutoTradingDialog() {
			AutoTradingDialog dlg = new AutoTradingDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.Result!=null) {
					AutoTradingResultPane pane = new AutoTradingResultPane(dlg.Result);
					AddDockingPane(pane, dlg.Result.Item.Title, pane.RequiredWidth, IconConst.COPY);
				}
			}
			return CommandResult.Succeeded;
		}
		private static Content AddDockingPane(Control content, string title, int width, int imgindex) {
			DockingManager m = Env.Frame.DockingManager;
			WindowContentTabbed wc = FindTabControl(m, State.DockLeft);
			Content c;
			if(wc==null) {
				c = m.Contents.Add(content, title);
				c.DisplaySize = new Size(width+16,500);
				c.FloatingSize = new Size(width+16,500); 
				m.AddContentWithState(c, State.DockLeft);
			}
			else {
				c = m.Contents.Add(content, title);
				//�ʏ�̃^�u������΂����ɒǉ�
				if(wc.TabControl.HideTabsMode==HideTabsModes.HideUsingLogic)
					m.AddContentToWindowContent(c, wc);
				c.BringToFront();
			}
			c.ImageList = Env.ImageList16;
			c.ImageIndex = imgindex;
			return c;

		}
		private static WindowContentTabbed FindTabControl(DockingManager m, State s) {
			foreach(Content c in m.Contents) {
				WindowContentTabbed t = c.ParentWindowContent as WindowContentTabbed;
				if(t!=null && t.State==s) return t;
			}
			return null;
		}

		public static CommandResult ShowAddBookmarkDialog() {
			SelectBookmarkFolder dlg = new SelectBookmarkFolder();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
				if(dlg.ResultFolder.ContainsCode(br.Code,false))
					Util.Warning(Env.Frame, "���ɓ����������o�^����Ă��܂��B");
				else{
					Content cnt = Env.Frame.BookmarkPaneContent;
					if(cnt!=null && !cnt.Visible)
						ShowBookmarkPane(true);				
					dlg.ResultFolder.AddChild(new BrandBookmark(dlg.ResultFolder,br.Code),null);
				}
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowBookmarkPane(bool show) {
			if(show) {
				foreach(Content content in Env.Frame.DockingManager.Contents) {
					if(content.Control is BookmarkPane) {
						content.BringToFront();
						return CommandResult.Failed;
					}
				}
				BookmarkPane pane = new BookmarkPane();
				Env.Frame.BookmarkPaneContent = AddDockingPane(pane, "���C�ɓ���", 150, IconConst.STAR);				
			}
			else {
				foreach(Content content in Env.Frame.DockingManager.Contents) {
					if(content.Control is BookmarkPane) {
						Env.Frame.DockingManager.Contents.Remove(content);
						return CommandResult.Failed;
					}
				}
				//Env.Frame.DockingManager.Contents.Remove(Env.Frame.BookmarkPaneContent);
				//Env.Frame.BookmarkPaneContent = null;
				//Env.Frame.DockingManager.HideContent(Env.Frame.BookmarkPaneContent);
			}
			return CommandResult.Succeeded;
		}
        public static CommandResult SetSplitAdjuster(bool adjust) {
            Env.BrandCollection.ClearAllFarms();
            Env.Preference.AdjustSplit = adjust;
            return ResetLayout();
        }
        public static CommandResult SetScaleLock(bool lock_) {
            Env.BrandCollection.ClearAllFarms();
            Env.Preference.ScaleLock = lock_;
			return ResetLayout();
		}
		public static CommandResult SetPriceEnabled(bool value) {
			Env.Preference.ShowPrice = value;
			return ResetLayout();
		}
		public static CommandResult SetAccumulativeVolumeEnabled(bool value) {
			Env.Preference.ShowAccumulativeVolume = value;
			return ResetLayout();
		}


        //!!Keys���g���̂͂�߂����Ƃ���
		public static CommandResult JumpDate(Keys key) {
			DataFarm farm = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
			if(farm.IsEmpty) return CommandResult.Failed;
			int limit = farm.TotalLength;

			int w = Env.Layout.DisplayColumnCount;
			int n = Env.Frame.ChartCanvas.FirstDateIndex;
			int cursor = n;
			switch(key) {
				case Keys.Home:
					n = 0;
					cursor = n;
					break;
				case Keys.End:
					n = limit - w;
					if(n<0) n = 0;
					cursor = limit-1;
					break;
				case Keys.PageUp:
					n -= w;
					if(n<0) n = 0;
					cursor = n;
					break;
				case Keys.PageDown:
					n += w;
					if(n > limit-w) n = limit-w;
					if(n < 0) n = 0;
					cursor = n + w - 1;
					break;
				case Keys.Left:
					n -= 5;
					if(n < 0) n=0;
					cursor = n;
					break;
				case Keys.Right:
					n += 5;
					if(n > limit-w) n = limit-w;
					if(n < 0) n = 0;
					cursor = n + w - 1;
					break;
			}

			Env.Frame.ChartCanvas.SetDateIndex(n, cursor);
			return CommandResult.Succeeded;
		}
		public static CommandResult OpenWeb() {
			try {
				Process.Start("http://www.omegachart.org/");
			}
			catch(Exception) {
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult AboutBox() {
			AboutBox dlg = new AboutBox();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowExtensionKitDialog() {
			ExtensionKitListDialog dlg = new ExtensionKitListDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				Env.SaveEnv();
				Reload(null);
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowKeyConfigDialog() {
			KeyConfigDialog dlg = new KeyConfigDialog();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}
        public static CommandResult ShowEnvironmentDialog() {
            EnvironmentDialog dlg = new EnvironmentDialog();
            dlg.ShowDialog(Env.Frame);
            return CommandResult.Succeeded;
        }


        public static CommandResult ShowKitTestDialog() {
			KitTestDialog dlg = Env.KitTestDialog;
			if(dlg==null) {
				dlg = new KitTestDialog();
				Rectangle rc = Env.Frame.DesktopBounds;
				dlg.Left = (rc.Left + rc.Right)/2 - dlg.Width/2;
				dlg.Top  = (rc.Top  + rc.Bottom)/2 - dlg.Height/2;
			}
			if(dlg.Visible) return CommandResult.Ignored;

			Env.Frame.CurrentModelessDialog = dlg;
			dlg.Owner = Env.Frame;
			dlg.Show();
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowSearchBrandDialog() {
			SearchBrandDialog dlg = new SearchBrandDialog();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		//IndicatorSet�̍č\�z�ƍĕ\��
		public static CommandResult RefreshChart() {
			RefreshChart(null);
			return CommandResult.Succeeded;
		}
		public static CommandResult RefreshChart(IIndicatorCustomizer ic) {
			Env.Frame.Cursor = Cursors.WaitCursor;
			ChartCanvas cnv = Env.Frame.ChartCanvas;
			AbstractBrand br = cnv.GetBrand();
			ChartFormat format= Env.Options.ChartFormat;
#if DOJIMA
			//�h���������̔������͋����Ȃ��@�����I�ɂ͂ł���͂�����
			if(br is DerivedBrand && format==ChartFormat.HalfDaily)
				format = ChartFormat.Daily;
#endif
			Env.BrandCollection.ClearAllFarms();
			Env.Preference.Refresh();

			IndicatorSetBuilder bld = new IndicatorSetBuilder();
			bld.Customizer = ic;
			bld.Construct(Env.Options.ChartFormat);
			Env.CurrentIndicators = bld.Result;

			cnv.ReloadFromPreference();
			cnv.LoadBrand(br, false);
			ResetLayout();
			Env.Frame.Cursor = Cursors.Default;
			return CommandResult.Succeeded;
		}
		public static CommandResult ResetLayout() {
			Env.Layout.Init();
			Env.Frame.ChartCanvas.ResetLayout();
			Env.Frame.Invalidate(true);
			return CommandResult.Succeeded;
		}


		public static CommandResult Reload(IIndicatorCustomizer ic) {
			Env.ReloadSchema();
			RefreshChart(ic);
			//Util.Information(Env.Frame, "�����[�h���܂���");
			return CommandResult.Succeeded;
		}

			
		public static CommandResult Quit() {
			Env.Frame.Close();
			return CommandResult.Succeeded;
		}

		//�����֌W
		public static CommandResult CreateKey() {
			RSAKeyPair kp = RSAKeyPair.GenerateNew(256, new Random());
			//PrivateKey�ۑ�
			StreamWriter wr = new StreamWriter(Env.GetAppDir()+"privatekey.txt");
			wr.WriteLine(kp.D.ToHexString());
			wr.WriteLine(kp.P.ToHexString());
			wr.WriteLine(kp.Q.ToHexString());
			wr.WriteLine(kp.U.ToHexString());
			wr.Close();
			//PublicKey�_���v
			RSAPublicKey pub = (RSAPublicKey)kp.PublicKey;
			Debug.WriteLine("Pubkey-Mod="+pub.Modulus.ToHexString());
			Debug.WriteLine("Pubkey-Exp="+pub.Exponent.ToHexString());
			return CommandResult.Succeeded;
		}

		public static CommandResult SignKit() {

			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "XML�t�H�[�}�b�g�̊g���L�b�g�I��";
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				string fn = dlg.FileName;
				StorageNode node = new DOMNodeReader(XmlUtil.LoadDOM(fn)).Read();
				Stream strm = new FileStream(fn+".bin", FileMode.Create, FileAccess.Write);
				new BinaryNodeWriter(strm).Write(node);
				strm.Close();
				SignFile(fn+".bin", fn+".signed");
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult SignFile(string fn, string destfile) {
			StreamReader re = new StreamReader(Env.GetAppDir()+"privatekey.txt");
			BigInteger d = new BigInteger(re.ReadLine(), 16);
			BigInteger p = new BigInteger(re.ReadLine(), 16);
			BigInteger q = new BigInteger(re.ReadLine(), 16);
			BigInteger u = new BigInteger(re.ReadLine(), 16);

			RSAPublicKey pub = ZPublicKey.PubKeyForExtensionKit;
			RSAKeyPair kp = new RSAKeyPair(pub.Exponent, d, pub.Modulus, u, p, q);

			byte[] data = new byte[(int)new FileInfo(fn).Length];
			FileStream s = new FileStream(fn, FileMode.Open, FileAccess.Read);
			s.Read(data, 0, data.Length);
			s.Close();
			Debug.WriteLine("Signed length="+data.Length);
			byte[] hash = new SHA1CryptoServiceProvider().ComputeHash(data, 0, data.Length);
			byte[] signature = kp.Sign(hash);
			kp.Verify(signature, hash);

			Stream strm = new FileStream(destfile, FileMode.Create, FileAccess.Write);
			strm.Write(data, 0, data.Length);
			strm.Write(signature, 0, signature.Length);
			strm.Close();
			return CommandResult.Succeeded;
		}

		public static CommandResult GenerateUserCode() {
			RegistrationDialog dlg = new RegistrationDialog();
			dlg.GeneratingCode = true;
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}
		public static CommandResult RegisterUserCode() {
			RegistrationDialog dlg = new RegistrationDialog();
			dlg.GeneratingCode = false;
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		public static CommandResult OpenCompanyInfoPage(CompanyInfoSite type) {
			try {
				string url = null;
				int code = Env.Frame.ChartCanvas.GetBrand().Code;
				switch(type) {
					case CompanyInfoSite.Yahoo:
						url = String.Format("http://profile.yahoo.co.jp/biz/fundamental/{0}.html", code);
						break;
					case CompanyInfoSite.Infoseek:
						url = String.Format("http://money.www.infoseek.co.jp/MnStock?qt={0}&sv=MN&pg=mn_creport.html", code);
						break;
					case CompanyInfoSite.Nikkei:
						url = String.Format("http://company.nikkei.co.jp/index.cfm?scode={0}", code);
						break;
					case CompanyInfoSite.Livedoor:
						url = String.Format("http://finance.livedoor.com/quote/profile?c={0}", code);
						break;
				}
				Process.Start(url);
				return CommandResult.Succeeded;
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				return CommandResult.Failed;
			}
		}
		//!!���̂�����̃y�A��delegate���g���Ȃǂ��Ă܂Ƃ߂���
		public static CommandResult ShowNextCode(){
			AbstractBrand br = Env.BrandCollection.FindNextBrand(
				Env.Frame.ChartCanvas.GetBrand().Code);
			ShowBrand(br);
			return CommandResult.Succeeded;
		}	
		public static CommandResult ShowPrevCode() {
			AbstractBrand br = Env.BrandCollection.FindPrevBrand(
				Env.Frame.ChartCanvas.GetBrand().Code);
			ShowBrand(br);
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowNextBrand(){
			BrandListPane pane = Env.Frame.CurrentBrandListPane;
			if(pane!=null) {
				AbstractBrand br = pane.NextBrand;
				if(br!=null)
					return ShowBrand(br);
				else
					return CommandResult.Failed;
			}
			return CommandResult.Ignored;
		}	
		public static CommandResult ShowPrevBrand() {
			BrandListPane pane = Env.Frame.CurrentBrandListPane;
			if(pane!=null) {
				AbstractBrand br = pane.PrevBrand;
				if(br!=null)
					return ShowBrand(br);
				else
					return CommandResult.Failed;
			}
			return CommandResult.Ignored;
		}
		public static CommandResult ShrinkCandleWidth() {
			int hcw = Env.Preference.HalfCandleWidth;
			if(--hcw < 1) {
				hcw = 1;
			}
			Env.Preference.CandleWidth = hcw * 2;
			ResetLayout();		
			return CommandResult.Succeeded;
		}
		public static CommandResult ExpandCandleWidth(){
			int hcw = Env.Preference.HalfCandleWidth;
			if(++hcw > 20) { // ���ɈӖ��Ȃ�
				hcw = 20;
			}
			Env.Preference.CandleWidth = hcw * 2;		
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetCandleWidth(int width) {
			Env.Preference.CandleWidth = width;		
			ResetLayout();
			return CommandResult.Succeeded;
		}

		public static CommandResult SetLogarithmStyle(bool value){
			Env.Preference.LogScale = value;
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetInverseStyle(bool value){
			Env.Preference.InverseChart = value;
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetChartFormat(ChartFormat fmt) {
			if(Env.Options.ChartFormat==fmt) return CommandResult.Ignored;
			Env.Options.ChartFormat = fmt;
			RefreshChart();
			return CommandResult.Succeeded;
		}
		public static CommandResult UpdateCurrentData() {
#if KENMILLE
			Env.Frame.ChartCanvas.ChartTitle.UpdateCurrentBrand();
			return CommandResult.Succeeded;
#else
			return CommandResult.Failed;
#endif
		}
		public static CommandResult DownloadIndexFile() {
			MemoryStream ns = null;
			try {
				ns = Util.HttpDownload("http://www.omegachart.org/download/index.txt");
				int dt = BrandCollection.GuessDate(ns);
				ns.Position = 0;
				if(dt > Env.BrandCollection.LastUpdatedDate) {
					Util.Information(Env.Frame, "�V�����C���f�b�N�X�t�@�C����������܂����B���f������ɂ�OmegaChart�̍ċN�����K�v�ł��B");
					Util.StreamToFile(ns, Env.GetAppDir() + "index.txt");
					return CommandResult.Succeeded;
				}
				else {
					Util.Information(Env.Frame, "�V�����C���f�b�N�X�t�@�C���͂���܂���B");
					return CommandResult.Ignored;
				}
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				return CommandResult.Failed;
			}
			finally {
				if(ns!=null) ns.Close();
			}
		}

#if DOJIMA
		public static CommandResult ExportHalfDailyData() {
			if(Env.CurrentIndicators.Format!=ChartFormat.HalfDaily) {
				Util.Warning("��������\��������Ԃł̂݃G�N�X�|�[�g�ł��܂�");
				return CommandResult.Failed;
			}

			SaveFileDialog dlg = new SaveFileDialog();
			dlg.Title = "�������f�[�^�̃G�N�X�|�[�g";
			dlg.Filter = "CSV Files(*.csv)|*.csv";
			dlg.DefaultExt = "csv";
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				string filename = dlg.FileName;
				DailyDataFarm f = (DailyDataFarm)Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
				Dojima.DojimaUtil.HalfDailyDataFarmCache.Get(f).ExportInCSV(filename);
				return CommandResult.Succeeded;
			}
			else
				return CommandResult.Ignored;
		}
#endif

		//�����f�[�^�̏k��
		public static CommandResult ShrinkData() {
			Env.Frame.Text = "�f�[�^�k����";
			int date = 20050104; //�����ȍ~�̓��t�̂ݐ؂�o��
			string dir = "shrinked";
			if(!Directory.Exists(dir))
				Directory.CreateDirectory("shrinked");

			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				BasicBrand br = ie.Value as BasicBrand;
				if(br!=null) {
					DailyDataFarm f = new DailyDataFarm();
					f.LoadFor(br);
					if(!f.IsEmpty) {
						int index = f.DateToIndex(date);

						FileStream fs = new FileStream(dir + "\\" + br.CodeAsString, FileMode.Create, FileAccess.Write);
						fs.Write(f.RawDataImage, index*DataFarm.RECORD_LENGTH, (f.TotalLength-index)*DataFarm.RECORD_LENGTH);
						fs.Close();
					}
				}
			}
			Util.Information(Env.Frame, "�I��");
			return CommandResult.Succeeded;
		}

		//����F���v���
		public static CommandResult StatisticsTest() {
			//RunPrivateScreening();
			//MyStatistics.Do();
			return CommandResult.Succeeded;

		}
#if PRIVATE_FEATURE 
		public static void RunPrivateScreening() {
			ScreeningItem item = FindScreeningItem("hottest");
			ScreeningOrder so = new ScreeningOrder(item.Header, item);
			so.Execute();
			ScreeningResult sr = so.Result as ScreeningResult; //!!as������炵��
			Util.Information(Env.Frame, "����");
			if(sr.ResultCount > 0) {
				ScreeningResultPane pane = new ScreeningResultPane(so);
				AddDockingPane(pane, item.Header, pane.RequiredWidth, IconConst.SEARCH);
			}
		}
		private static ScreeningItem FindScreeningItem(string name) {
			foreach(ScreeningItem item in Env.CurrentIndicators.GetScreeningItems()) {
				if(item.Header=="�v���Ӗ���") return item;
			}
			return null;
		}
#endif
	}
}
