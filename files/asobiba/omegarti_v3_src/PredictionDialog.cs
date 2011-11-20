/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using Zanetti.Prediction;
using Zanetti.Indicators;

namespace Zanetti.Forms
{
	/// <summary>
	/// PredictionDialog �̊T�v�̐����ł��B
	/// </summary>
	internal class PredictionDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.Button _okButton;
		//private System.Windows.Forms.Button _quizButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		private Hashtable _buttonToPredictionItem;
		private PredictionItem _selectedPrediction;
		private VerificationResult _result;

		public PredictionDialog()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
			LoadRadioButtons();
		}

		private void LoadRadioButtons() {
			_buttonToPredictionItem = new Hashtable();
			int i = 0;

			int y = _message.Top+_message.Height+8;
			foreach(PredictionItem pi in Env.CurrentIndicators.PredictionItems) {
				RadioButton rb = new RadioButton();
				rb.Text = pi.Title;
				rb.FlatStyle = FlatStyle.System;
				rb.Width = this.ClientSize.Width-16;
				rb.Left = 8;
				rb.Top = y + i*24;
				rb.Height = 24;
				rb.CheckedChanged += new EventHandler(OnCheckedChanged);
				_buttonToPredictionItem[rb] = pi;
				rb.Checked = (i==0);
				this.Controls.Add(rb);
				i++;
			}

			_okButton.Top = y + i*24;
			_cancelButton.Top = y + i*24;
			this.ClientSize = new Size(this.ClientSize.Width, _okButton.Top+32);
		}

		/// <summary>
		/// �g�p����Ă��郊�\�[�X�Ɍ㏈�������s���܂��B
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows �t�H�[�� �f�U�C�i�Ő������ꂽ�R�[�h 
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
		/// </summary>
		private void InitializeComponent()
		{
			this._message = new System.Windows.Forms.Label();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _message
			// 
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(320, 23);
			this._message.TabIndex = 0;
			this._message.Text = "�@�\�����̖����ɂ��āA�w�肵���@�����ǂ̒��x�������邩�����؂��܂��B";
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Location = new System.Drawing.Point(160, 40);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 1;
			this._okButton.Text = "OK";
			this._okButton.Click += new EventHandler(OnOK);
			this._okButton.FlatStyle = FlatStyle.System;
			/*
			// 
			// _quizButton
			// 
			this._quizButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;	// quiz�I�������_�C�A���O����
			this._quizButton.Location = new System.Drawing.Point(208, 40);
			this._quizButton.Name = "_quizButton";
			this._quizButton.TabIndex = 2;
			this._quizButton.Text = "�N�C�Y";
			this._quizButton.Click += new EventHandler(OnQuiz);
			this._quizButton.FlatStyle = FlatStyle.System;
			*/
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.Location = new System.Drawing.Point(240, 40);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 2;
			this._cancelButton.Text = "�L�����Z��";
			this._cancelButton.FlatStyle = FlatStyle.System;
			// 
			// PredictionDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(362, 70);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._message);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "PredictionDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "�Z�I���[�̌���";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			if(_buttonToPredictionItem.Count==0) {
				Util.Warning(this, "�Z�I���[���؂��g���L�b�g���ɓo�^����Ă��܂���B");
				Close();
			}
		}


		public VerificationResult Result {
			get {
				return _result;
			}
		}

		private void OnCheckedChanged(object sender, EventArgs args) {
			_selectedPrediction = (PredictionItem)_buttonToPredictionItem[sender];
		}

		private void OnOK(object sender, EventArgs args) {
			try {
				VerificationResult vr = new VerificationResult(_selectedPrediction);
				vr.Verify(Env.Frame.ChartCanvas.GetBrand().ReserveFarm());
				_result = vr;
			}
			catch(ArgumentException ex) {
				Util.Warning(this, ex.Message);
				this.DialogResult = DialogResult.Abort;
			}

		}
		private void OnQuiz(object sender, EventArgs args) 
		{
			try 
			{
				int seikai = 0;
				int machigai = 0;
				
				this.Visible = false;
				VerificationResult vr = new VerificationResult(_selectedPrediction);
				vr.Verify(Env.Frame.ChartCanvas.GetBrand().ReserveFarm());
				// ���K���瓪�̏��Ԃœ����Ă���̂ŋt�ɂ���@�������F�z��ɂ���
				Stack s = new Stack();
				foreach(VerificationResultEntry e in vr.Entries)
				{
					s.Push(e);
				}				
				foreach(VerificationResultEntry e in s) 
				{
					Zanetti.Data.AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
					Env.Frame.ChartCanvas.ForceVisibleDate(e.Date,true);
					DialogResult ans = Util.AskUserYesNo(this,String.Format("{0}�N{1}��{2}�� �F �Z�I���[���������Ă���Ɨ\�z���܂����H",e.Date / 10000,e.Date / 100 % 100,e.Date % 100));
					Env.Frame.ChartCanvas.ForceVisibleDate(e.Date,false);
					
					if((ans == DialogResult.Yes && e.Result == PredictionResult.True) ||
						(ans == DialogResult.No && e.Result == PredictionResult.False)){
						// ����
						Util.Information(this,"�����I");
						seikai++;
					}
					else{
						// �ԈႢ
						Util.Information(this,"�s�����I");
						machigai++;
					}					
				}
				Util.Information(this,String.Format("�N�C�Y���I�����܂����B\n���𐔁F{0}  �s���𐔁F{1}  ���𗦁F{2:0.##}�ł��B",seikai,machigai,(double)seikai/(seikai+machigai)));
				
			}
			catch(ArgumentException ex) 
			{
				Util.Warning(this, ex.Message);
				this.DialogResult = DialogResult.Abort;
			}

		}
	}
}
