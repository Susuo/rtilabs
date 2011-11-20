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
using System.Diagnostics;

namespace Zanetti.Forms
{
	/// <summary>
	/// PromptContribution �̊T�v�̐����ł��B
	/// </summary>
	internal class PromptContribution : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.LinkLabel linkLabel1;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PromptContribution()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
			Timer t = new Timer();
			t.Tick += new EventHandler(OnTimer);
			t.Interval = 20000;
			t.Start();
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
			this._okButton = new System.Windows.Forms.Button();
			this._message = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Enabled = false;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(176, 216);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			// 
			// _message
			// 
			this._message.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(420, 176);
			this._message.TabIndex = 1;
			// 
			// linkLabel1
			// 
			this.linkLabel1.Location = new System.Drawing.Point(0, 192);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.TextAlign = ContentAlignment.MiddleCenter;
			this.linkLabel1.Size = new System.Drawing.Size(424, 16);
			this.linkLabel1.TabIndex = 2;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "���̃��b�Z�[�W���o�����R�ɂ��Ă͂�������N���b�N���Ă��������B";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
			// 
			// PromptContribution
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(432, 248);
			this.ControlBox = false;
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this._message);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PromptContribution";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Contribution Certificate�ɂ���";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			_message.Text = String.Format("�@���̃��b�Z�[�W�́AContribution Certificate���o�^����Ă��Ȃ��ꍇ�ɁA10��ɂP��̊����œo�ꂵ�܂��B\n\n" +
				"�@OmegaChart�́A�F����̋��͂�ʂ��Ă��悢�c�[��������Ă������Ƃ����ړI�̃v���W�F�N�g�ł��B���̂��߁A" +
				"Contribution Certificate(�v���ؖ���)�Ƃ����d�g�݂ɂ���āA�v�����������l��\������悤�ɂ��Ă��܂��B\n\n" +
				"�@�ؖ�����o�^����ƁA���̃��b�Z�[�W���o�Ȃ��Ȃ�ƂƂ��Ƀo�[�W�������ɖ��O���o�^����A��]�҂�OmegaChart��Web�T�C�g�ɂ����O���f�ڂ���܂��B\n" +
				"�@�v�������ɂ͊J���ɎQ�����鑼�A��`���t�Ȃǂ��낢�날��܂��B�e�����ł��鋦�͂����肢���܂��B\n" +
				"�@�Ȃ��A���̃��b�Z�[�W�͕\������Ă���20�b��ɕ��邱�Ƃ��ł���悤�ɂȂ�܂��B");
		}

		private void OnTimer(object sender, EventArgs args) {
			_okButton.Enabled = true;
		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e) {
			try {
				Process.Start("http://www.omegachart.org/prompt.html");
			}
			catch(Exception) {
			}
		}
	}

	internal class PromptDonation : System.Windows.Forms.Form {
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.LinkLabel linkLabel1;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PromptDonation() {
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

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

		#region Windows �t�H�[�� �f�U�C�i�Ő������ꂽ�R�[�h 
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
		/// </summary>
		private void InitializeComponent() {
			this._okButton = new System.Windows.Forms.Button();
			this._message = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Enabled = true;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(176, 128);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			// 
			// _message
			// 
			this._message.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(420, 88);
			this._message.TabIndex = 1;
			// 
			// linkLabel1
			// 
			this.linkLabel1.Location = new System.Drawing.Point(0, 104);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.TextAlign = ContentAlignment.MiddleCenter;
			this.linkLabel1.Size = new System.Drawing.Size(424, 16);
			this.linkLabel1.TabIndex = 2;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "��t�ɂ��Ă̏ڂ����ē��͂�����ł�";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
			// 
			// PromptContribution
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(432, 160);
			this.ControlBox = false;
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this._message);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PromptDonation";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "��t�ɂ���";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			_message.Text = String.Format("�@OmegaChart�ł́A�펞��t���W���ł��B\n\n" +
				"�@�܂���t�����Ă��Ȃ����͑���Ő��ʂ����߂��Ƃ��ɂł����Ђ��������������B�V�@�\�̎����Ƃ̌�������������������ȂǁA" +
				"���C�y�ɂ����k���������B��t�����������A�@�\�ɂ��Ă̈ӌ��𕷂��͍̂�҂Ƒ��̋��͎҂̂��C�ɂȂ���܂��B");
		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e) {
			try {
				Process.Start("http://www.omegachart.org/donation.html");
			}
			catch(Exception) {
			}
		}
	}
}
