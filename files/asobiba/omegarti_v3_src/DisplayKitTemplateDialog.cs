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
using System.Text;

using Zanetti.Indicators;

namespace Zanetti.Forms
{
	/// <summary>
	/// DisplayKitTemplateDialog �̊T�v�̐����ł��B
	/// </summary>
	internal class DisplayKitTemplateDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label _lMessage;
		private System.Windows.Forms.TextBox _resultBox;
		private System.Windows.Forms.Button _okButton;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public DisplayKitTemplateDialog()
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
			this._lMessage = new System.Windows.Forms.Label();
			this._resultBox = new System.Windows.Forms.TextBox();
			this._okButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _lMessage
			// 
			this._lMessage.Location = new System.Drawing.Point(8, 8);
			this._lMessage.Name = "_lMessage";
			this._lMessage.Size = new System.Drawing.Size(352, 24);
			this._lMessage.TabIndex = 0;
			this._lMessage.Text = "�ȉ����N���b�v�{�[�h�ɃR�s�[���A�p�����[�^��\���t�H�[�}�b�g�ȂǁA�K�v�ȂƂ���͓K���ɕ���Ďg�p���Ă��������B";
			// 
			// _resultBox
			// 
			this._resultBox.Location = new System.Drawing.Point(8, 40);
			this._resultBox.Multiline = true;
			this._resultBox.Name = "_resultBox";
			this._resultBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this._resultBox.Size = new System.Drawing.Size(352, 184);
			this._resultBox.TabIndex = 1;
			this._resultBox.Text = "";
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(144, 232);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 2;
			this._okButton.Text = "OK";
			// 
			// DisplayKitTemplateDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(368, 262);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._resultBox);
			this.Controls.Add(this._lMessage);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DisplayKitTemplateDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "�e���v���[�g";
			this.ResumeLayout(false);

		}
		#endregion

		public void FormatIndicator(string name, IndicatorTarget target, string expr, IndicatorStyle penstyle, Color col) {
			StringBuilder bld = new StringBuilder();
			Prologue(bld);
			bld.Append(String.Format("  <indicator\n    target=\"{0}\"\r\n    name=\"{1}\"\r\n    format=\"F2\"\r\n    display=\"normal\">\r\n", FormatTarget(target), name));
			bld.Append(String.Format("    <title>{0}</title>\r\n    <header>{1}</header>\r\n", name, name));
			bld.Append(String.Format("    <expression><![CDATA[{0}]]></expression>\r\n", expr));
			bld.Append(String.Format("    <defaultappearance style=\"{0}\" color=\"{1}\"/>\r\n", penstyle.ToString(), col.Name));
			bld.Append(String.Format("  </indicator>\r\n"));
			Epilogue(bld);
			_resultBox.Text = bld.ToString();
		}
		public void FormatLibrary(string name, string expr) {
			StringBuilder bld = new StringBuilder();
			Prologue(bld);
			bld.Append(String.Format("  <library name=\"{0}\">\r\n", name));
			bld.Append(String.Format("    <expression><![CDATA[{0}]]></expression>\r\n", expr));
			bld.Append(String.Format("  </library>\r\n"));
			Epilogue(bld);
			_resultBox.Text = bld.ToString();
		}
		private void Prologue(StringBuilder bld) {
			bld.Append("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\r\r\n");
			bld.Append("<omegachart-extension version=\"1.0\">\r\n");
			bld.Append("  <info title=\"\" id=\"\" author=\"\" expires=\"never\"/>\r\n");
		}
		private void Epilogue(StringBuilder bld) {
			bld.Append("</omegachart-extension>\r\n");
		}

		private static string FormatTarget(IndicatorTarget target) {
			return target.ToString().ToLower();
		}
	}
}
