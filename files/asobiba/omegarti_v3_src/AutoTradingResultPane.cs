/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id
 */
using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Diagnostics;

using Travis.Util;

using Zanetti.Commands;
using Zanetti.Indicators;
using Zanetti.Data;
using Zanetti.Forms;
using Zanetti.SystemTrading;
using Zanetti.SystemTrading.AutoTrading;



namespace Zanetti.UI {

    //hacked by rti
    //�J�������� enum ���A���������������ł���
    internal enum AutoTradingListViewColumn
    {
        Brand = 0,
        SignalDate = 1,
        EntryPrice = 2,
        Result = 3,
        EndDate = 4,
        EndPrice = 5,
        Performance = 6,
        Bikou = 7,
        TooManyResult,
    }
    //-----------------------

	internal class AutoTradingResultPane : System.Windows.Forms.UserControl {
		private AutoTradingResult _result;
		
		private Panel _panel;
		private Button _summaryButton;
		private Button _exportButton;
		private System.Windows.Forms.Label _explanation;
		private ListViewWithCustomKey _listView;
		private System.Windows.Forms.ColumnHeader _colBrand;
		private System.Windows.Forms.ColumnHeader _colSignalDate;
		private System.Windows.Forms.ColumnHeader _colEntryPrice;
		private System.Windows.Forms.ColumnHeader _colResult;
		private System.Windows.Forms.ColumnHeader _colEndDate;
		private System.Windows.Forms.ColumnHeader _colEndPrice;
		private System.Windows.Forms.ColumnHeader _colPerformance;
		//hacked by rti
        private System.Windows.Forms.ColumnHeader _colBikou;
        //----

		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public AutoTradingResultPane(AutoTradingResult result) {
			// ���̌Ăяo���́AWindows.Forms �t�H�[�� �f�U�C�i�ŕK�v�ł��B
			InitializeComponent();
			this.SetStyle(ControlStyles.DoubleBuffer|ControlStyles.AllPaintingInWmPaint, true);

			_result = result;
			// TODO: InitializeComponent �Ăяo���̌�ɏ�����������ǉ����܂��B
			InitList();
		}

		public int RequiredWidth {
			get {
				return 468;
			}
		}

		/// <summary>
		/// �g�p����Ă��郊�\�[�X�Ɍ㏈�������s���܂��B
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if(components != null) {
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
		private void InitializeComponent() {
            this._panel = new System.Windows.Forms.Panel();
            this._summaryButton = new System.Windows.Forms.Button();
            this._exportButton = new System.Windows.Forms.Button();
            this._explanation = new System.Windows.Forms.Label();
            this._listView = new Zanetti.UI.ListViewWithCustomKey();
            this._colBrand = new System.Windows.Forms.ColumnHeader();
            this._colSignalDate = new System.Windows.Forms.ColumnHeader();
            this._colEntryPrice = new System.Windows.Forms.ColumnHeader();
            this._colResult = new System.Windows.Forms.ColumnHeader();
            this._colEndDate = new System.Windows.Forms.ColumnHeader();
            this._colEndPrice = new System.Windows.Forms.ColumnHeader();
            this._colPerformance = new System.Windows.Forms.ColumnHeader();
            this._colBikou = new System.Windows.Forms.ColumnHeader();
            this._panel.SuspendLayout();
            this.SuspendLayout();
            // 
            // _panel
            // 
            this._panel.Controls.Add(this._summaryButton);
            this._panel.Controls.Add(this._exportButton);
            this._panel.Controls.Add(this._explanation);
            this._panel.Dock = System.Windows.Forms.DockStyle.Top;
            this._panel.Location = new System.Drawing.Point(0, 0);
            this._panel.Name = "_panel";
            this._panel.Size = new System.Drawing.Size(144, 72);
            this._panel.TabIndex = 0;
            // 
            // _summaryButton
            // 
            this._summaryButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._summaryButton.Location = new System.Drawing.Point(8, 31);
            this._summaryButton.Name = "_summaryButton";
            this._summaryButton.Size = new System.Drawing.Size(64, 23);
            this._summaryButton.TabIndex = 0;
            this._summaryButton.Text = "�W�v(&S)...";
            this._summaryButton.Click += new System.EventHandler(this.OnShowSummary);
            // 
            // _exportButton
            // 
            this._exportButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._exportButton.Location = new System.Drawing.Point(80, 31);
            this._exportButton.Name = "_exportButton";
            this._exportButton.Size = new System.Drawing.Size(80, 23);
            this._exportButton.TabIndex = 1;
            this._exportButton.Text = "�G�N�X�|�[�g(&E)...";
            this._exportButton.Click += new System.EventHandler(this.OnExport);
            // 
            // _explanation
            // 
            this._explanation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._explanation.Location = new System.Drawing.Point(8, 4);
            this._explanation.Name = "_explanation";
            this._explanation.Size = new System.Drawing.Size(56, 19);
            this._explanation.TabIndex = 0;
            this._explanation.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _listView
            // 
            this._listView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this._listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this._colBrand,
            this._colSignalDate,
            this._colEntryPrice,
            this._colResult,
            this._colEndDate,
            this._colEndPrice,
            this._colPerformance,
            this._colBikou});
            this._listView.Dock = System.Windows.Forms.DockStyle.Fill;
            this._listView.FullRowSelect = true;
            this._listView.GridLines = true;
            this._listView.Location = new System.Drawing.Point(0, 72);
            this._listView.MultiSelect = false;
            this._listView.Name = "_listView";
            this._listView.Size = new System.Drawing.Size(144, 72);
            this._listView.TabIndex = 1;
            this._listView.UseCompatibleStateImageBehavior = false;
            this._listView.View = System.Windows.Forms.View.Details;
            this._listView.SelectedIndexChanged += new System.EventHandler(this.OnItemActivated);
            // 
            // _colBrand
            // 
            this._colBrand.Text = "����";
            // 
            // _colSignalDate
            // 
            this._colSignalDate.Width = 80;
            // 
            // _colEntryPrice
            // 
            this._colEntryPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._colEntryPrice.Width = 64;
            // 
            // _colResult
            // 
            this._colResult.Text = "����";
            this._colResult.Width = 40;
            // 
            // _colEndDate
            // 
            this._colEndDate.Width = 80;
            // 
            // _colEndPrice
            // 
            this._colEndPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._colEndPrice.Width = 64;
            // 
            // _colPerformance
            // 
            this._colPerformance.Text = "�p�t�H�[�}���X";
            this._colPerformance.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._colPerformance.Width = 80;
            // 
            // _colBikou
            // 
            this._colBikou.Text = "���l";
            this._colBikou.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._colBikou.Width = 80;
            // 
            // AutoTradingResultPane
            // 
            this.Controls.Add(this._listView);
            this.Controls.Add(this._panel);
            this.Name = "AutoTradingResultPane";
            this.Size = new System.Drawing.Size(144, 144);
            this._panel.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		private void InitList() {
			_explanation.Text = _result.Item.Header;
			_listView.SuspendLayout();
			_listView.BeginUpdate();

			
			_colSignalDate.Text = "�V�O�i�����t";
			_colEndDate.Text = "�I�����t";
			if(_result.Item.TradingType==TradingType.Long) {
				_colEntryPrice.Text = "�w�����i";
				_colEndPrice.Text = "���p���i";
			}
			else { //����̌��؂ł͕\�������ւ���
				_colEntryPrice.Text = "���p���i";
				_colEndPrice.Text = "�w�����i";
			}
            //hacked by rti
            //�A�i���X�g�̂��ӌ�����ǉ�����
            for (int n = 0; n < _result.Item.GetAnalystCount(); n++)
            {
                AutoTradingAnalystItem anaS = _result.Item.GetAnalyst(n);
                this._listView.Columns.Add(anaS.Name, 80, HorizontalAlignment.Right );
            }
            //----

			for(int i=0; i<_result.ResultCount; i++) {
				AddItem(_result.GetAtS(i));
			}
			_listView.EndUpdate();

			_listView.ResumeLayout();
		}

		//hacked by rti
		/*
		private void AddItem(SignalResult sr) {
			string[] values = new string[7];
			values[0] = sr.Brand.Name;
			values[1] = Util.FormatShortDate(sr.StartDate);
			if(sr.Result==SignalResultType.Skip) {
				values[2] = "��肹��";
				for(int i=3; i<=6; i++) values[i] = "";
			}
			else {
				values[2] = sr.StartPrice.ToString("F0");
				if(sr.Result==SignalResultType.Exit || sr.Result==SignalResultType.Losscut) {
					values[3] = PerformanceSymbol(sr.Result, sr.Performance(_result.Item.TradingType));
					values[4] = Util.FormatShortDate(sr.EndDate);
					values[5] = sr.EndPrice.ToString("F0");
					double p = sr.Performance(_result.Item.TradingType);
					values[6] = String.Format("{0:F2}%", p*100);
				}
				else {
					values[3] = "�H";
					values[4] = sr.Result==SignalResultType.Draw? Util.FormatShortDate(sr.EndDate) : "";
					values[5] = "";
					values[6] = sr.Result==SignalResultType.Draw? "���v�m���LC������" : "����s��";
				}
			}
			ListViewItem li = new ListViewItem(values);
			li.Tag = sr;
			_listView.Items.Add(li);
		}
		*/
 
		private void AddItem(SignalResult sr) {
            //���J������
            int columnCount = (int)AutoTradingListViewColumn.TooManyResult + _result.Item.GetAnalystCount();

            string[] values = new string[columnCount];
			values[0] = sr.Brand.Name;
			values[1] = Util.FormatShortDate(sr.StartDate);
			if(sr.Result==SignalResultType.Skip) {
				values[2] = "��肹��";
                for (int i = 3; i <= (int)AutoTradingListViewColumn.TooManyResult; i++) values[i] = "";
			}
			else {
				values[2] = sr.StartPrice.ToString("F0");
				if(sr.Result==SignalResultType.Exit || sr.Result==SignalResultType.Losscut) {
					values[3] = PerformanceSymbol2(sr.Performance(_result.Item.TradingType));
					values[4] = Util.FormatShortDate(sr.EndDate);
					values[5] = sr.EndPrice.ToString("F0");
					double p = sr.Performance(_result.Item.TradingType);
					values[6] = String.Format("{0:F2}%", p*100);
                    values[7] = PerformanceBikou(sr.Result, sr.Performance(_result.Item.TradingType));	//�����������Ƃ͔��l�ɏ���
				}
                else if (sr.Result == SignalResultType.Draw)
                {
					//�h���[�Ɋ֌W�Ȃ��A����������\������
                    values[3] = PerformanceSymbol2(sr.Performance(_result.Item.TradingType));
                    values[4] = Util.FormatShortDate(sr.EndDate);
                    values[5] = sr.EndPrice.ToString("F0");
                    double p = sr.Performance(_result.Item.TradingType);
                    values[6] = String.Format("{0:F2}%", p * 100);
                    values[7] = "���v�m���LC������";													//�����������Ƃ͔��l�ɏ���
                }
                else
                {
                    values[3] = "�H";
                    values[4] = "";
                    values[5] = "";
                    double p = sr.Performance(_result.Item.TradingType);
                    values[6] = "";
                    values[7] = "����s��";																//�����������Ƃ͔��l�ɏ���
                }
			}
            //�A�i���X�g�̈ӌ���ǉ�
            for (int i = 0; i < _result.Item.GetAnalystCount(); i++)
            {
                values[(int)AutoTradingListViewColumn.TooManyResult + i] = sr.GetAnalystComment(i);
            }

			ListViewItem li = new ListViewItem(values);
			li.Tag = sr;
			_listView.Items.Add(li);
		}
		//^^^

		private void OnItemActivated(object sender, EventArgs args) {
			ListView.SelectedListViewItemCollection t = _listView.SelectedItems;
			if(t.Count>0) {
				ListViewItem li = t[0];
				SignalResult sr = li.Tag as SignalResult;
				if(sr!=null) {
					AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
					if(br!=sr.Brand) CommandExec.ShowBrand(sr.Brand);
                    //hacked by rti.
                    //���������̊J�n�ƏI���Ƀ��C��������
                    Env.Frame.ChartCanvas.DrawAutoTradingLine(sr.StartDate, sr.EndDate);
                    //-------------------------
                    Env.Frame.ChartCanvas.ForceVisibleDate(sr.StartDate, false);
                }
			}
		}

		private void OnShowSummary(object sender, EventArgs args) {
			new AutoTradingSummaryDialog(_result).ShowDialog(this.ParentForm);
		}
		private void OnExport(object sender, EventArgs args) {
			ContextMenu cm = new ContextMenu();
			Util.AddMenuItem(cm, "�N���b�v�{�[�h�փR�s�[(&C)", new EventHandler(OnCopyResult));
			Util.AddMenuItem(cm, "CSV�`���ŕۑ�(&S)...", new EventHandler(OnSaveResult));
			cm.Show(this, new Point(_exportButton.Left, _exportButton.Bottom));
		}
		private void OnCopyResult(object sender, EventArgs args) {
			ListViewUtil.CopyListViewContentToClipboard(_listView, new ListViewUtil.ItemChecker(ListViewItemChecker));
		}
		private void OnSaveResult(object sender, EventArgs args) {
			ListViewUtil.SaveListViewContentToFile(_listView, new ListViewUtil.ItemChecker(ListViewItemChecker));
		}
		private bool ListViewItemChecker(ListViewItem li) {
			return true; //��Ƀf�[�^�͗L��
		}

		//hacked by rti
		//���l����
        private static string PerformanceBikou(SignalResultType r, double p)
        {
            //���X�g�J�b�g�Ȃ̂ɖׂ�������A
            //���v�m��Ȃ̂ɁA��������
            //�Ȃǂ̂܂�Ȃ����R�͂������ŕԂ�
            if (r == SignalResultType.Exit)
            {
                if (p > 0)
                    return "";  //����i�V
                else
                    return "��(���ʂ͑���)";
            }
            else if (r == SignalResultType.Losscut)
            {
                if (p < 0)
                    return "";  //����i�V
                else
                    return "�~(���ʂ͗��v)";
            }
            else
            {
                Debug.Assert(false); //should not reach here
                return null;
            }

        }
		//����������������
        private static string PerformanceSymbol2(double p)
        {
            //���X�g�J�b�g�Ɉ��������낤���A���v�m�肪�ł��悤���A�悤�ׂ͖���� ��k
            //���ׂĂ̐l�͂����̑O�ɂ̂ݕ����ł���B
            if (p > 0)
            {
                return "��";
            }
            else
            {
                return "�~";
            }

        }
        //---
        
        private static string PerformanceSymbol(SignalResultType r, double p)
        {
			//���̓p�t�H�[�}���X�̕�����Exit/Losscut�Ɉ�v���邪�A���̏������ɂ���Ă͂����͂Ȃ�Ȃ��̂ŋ��
			if(r==SignalResultType.Exit) {
				if(p > 0)
					return "��";
				else
					return "��(���ʂ͑���)";
			}
			else if(r==SignalResultType.Losscut) {
				if(p < 0)
					return "�~";
				else
					return "�~(���ʂ͗��v)";
			}
			else {
				Debug.Assert(false); //should not reach here
				return null;
			}
		}
	}
}
