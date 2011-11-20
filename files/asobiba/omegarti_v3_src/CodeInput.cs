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

using Zanetti.UI;

namespace Zanetti.Forms
{
	/// <summary>
	/// CodeInput �̊T�v�̐����ł��B
	/// </summary>
	internal class CodeInput : System.Windows.Forms.Form
	{
		private CodeBox _codeBox;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public CodeInput()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
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
			this._codeBox = new CodeBox();
			this.SuspendLayout();
			// 
			// _codeBox
			// 
			this._codeBox.Font = new System.Drawing.Font("MS UI Gothic", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._codeBox.ImeMode = System.Windows.Forms.ImeMode.Off;
			this._codeBox.Location = new System.Drawing.Point(0, 0);
			this._codeBox.MaxLength = 4;
			this._codeBox.Name = "_codeBox";
			this._codeBox.StockCode = 0;
			this._codeBox.TabIndex = 0;
			this._codeBox.Text = "";
			this._codeBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this._codeBox.CodeComplete += new System.EventHandler(this.OnCodeComplete);
			// 
			// CodeInput
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(98, 24);
			this.Controls.Add(this._codeBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CodeInput";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "�R�[�h����";
			this.ResumeLayout(false);

		}
		#endregion

		public int StockCode {
			get {
				return _codeBox.StockCode;
			}
			set {
				_codeBox.StockCode = value;
			}
		}

		private void OnCodeComplete(object sender, EventArgs args) {
			this.DialogResult = DialogResult.OK;
			Close();
		}

		protected override bool ProcessDialogKey(Keys keyData) {
			if(keyData==Keys.Escape) {
				this.DialogResult = DialogResult.Cancel;
				Close();
				return true;
			}
			else
				return base.ProcessDialogKey (keyData);
		}



	}
}
