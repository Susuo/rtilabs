/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Zanetti.Forms
{
	/// <summary>
	/// Registration �̊T�v�̐����ł��B
	/// </summary>
	internal class RegistrationDialog : System.Windows.Forms.Form
	{
		private bool _generatingCode;

		private System.Windows.Forms.TextBox _textBox;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.Label _lMessage;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public RegistrationDialog()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
		}
		public bool GeneratingCode {
			get {
				return _generatingCode;
			}
			set {
				_generatingCode = value;
			}
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
			this._textBox = new System.Windows.Forms.TextBox();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this._lMessage = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// _textBox
			// 
			this._textBox.Location = new System.Drawing.Point(8, 32);
			this._textBox.MaxLength = 1000;
			this._textBox.Multiline = true;
			this._textBox.Name = "_textBox";
			this._textBox.Size = new System.Drawing.Size(304, 40);
			this._textBox.TabIndex = 0;
			this._textBox.Text = "";
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(144, 80);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 1;
			this._okButton.Text = "OK";
			this._okButton.Click += new EventHandler(OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(232, 80);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 2;
			this._cancelButton.Text = "�L�����Z��";
			// 
			// _lMessage
			// 
			this._lMessage.Location = new System.Drawing.Point(16, 8);
			this._lMessage.Name = "_lMessage";
			this._lMessage.Size = new System.Drawing.Size(288, 24);
			this._lMessage.TabIndex = 3;
			this._lMessage.Text = "������Contribution Certificate Code����͂��Ă�������";
			// 
			// Registration
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(316, 110);
			this.Controls.Add(this._lMessage);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._textBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Registration";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Contribution Certificate �o�^";
			this.ResumeLayout(false);

		}
		#endregion

		private void OnOK(object sender, EventArgs args) {
			this.DialogResult = DialogResult.None;
			if(_generatingCode)
				GenerateCode();
			else
				AcceptCode();
				
		}
		private void AcceptCode() {
			if(_textBox.TextLength==0)
				Util.Warning(this, "�R�[�h�����͂���Ă��܂���B");
			else {
				try {
					string[] data = _textBox.Lines;
					string code = data[0];
					for(int i=1; i<data.Length; i++) code += data[i];
					SimpleUserKey key = SimpleUserKey.Load(code);
					Env.Options.RawCertString = code;
					Env.Options.Certificate = key;
					if(key.IsValid)
						Util.Information(this, String.Format("{0} ���܂̖��O�œo�^���܂����B���肪�Ƃ��������܂����B", key.UserName));
					else
						Util.Warning(this, "Contribution Certificate�̓o�^�͂ł��܂���ł����B");

					this.DialogResult = DialogResult.OK;
				}
				catch(Exception ex) {
					Util.Warning(this, "�o�^���ł��܂���ł����B\n"+ex.Message);
				}
			}
		}
		private void GenerateCode() {
			SimpleUserKey key = new SimpleUserKey();
			key.UserName = _textBox.Text.Trim();
			Debug.WriteLine(String.Format("Key for [{0}]", key.UserName));
            string cert = key.ComputeSignedString();
            Debug.WriteLine(cert);
            Clipboard.SetDataObject(key.UserName + "," + cert, true);
        }

	}
}
