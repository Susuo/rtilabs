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

using Zanetti.Config;
using Zanetti.UI;
using Zanetti.Commands;

namespace Zanetti.Forms
{
	internal class KeyConfigDialog : System.Windows.Forms.Form
	{
		private class ComboBoxTag {
			public string caption;
			public KeyConfig.DirectionalEntry entry;
			public ComboBoxTag(KeyConfig.DirectionalEntry e, string c) {
				entry = e;
				caption = c;
			}
		}

		private ArrayList _configComboBoxes;
		private KeyConfig _keyConfig;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public KeyConfigDialog()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
			InitUI();
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
			this._cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(160, 192);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			this._okButton.Click += new EventHandler(OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.Location = new System.Drawing.Point(248, 192);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 1;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Text = "�L�����Z��";
			// 
			// KeyBind
			// 
			this.AcceptButton = this._okButton;
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(338, 222);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "KeyBind";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "�L�[���蓖�ẴJ�X�^�}�C�Y";
			this.ResumeLayout(false);

		}
		#endregion

		private void InitUI() {
			_keyConfig = Env.Options.KeyConfig.Clone();
			int tabindex = 0;
			_configComboBoxes = new ArrayList();
			AddUI(CID.MoveToPrevDate,  "�`���[�g�����ʃX�N���[��", "(&T)",  8, ref tabindex);
			AddUI(CID.MoveToPrevPage,  "�`���[�g���P�y�[�W�X�N���[��", "(&P)", 32, ref tabindex);
			AddUI(CID.ShowPrevBrand,   "������\�����ňړ�", "(&B)", 56, ref tabindex);
			AddUI(CID.ShowPrevCode,    "�������R�[�h���ňړ�", "(&C)", 80, ref tabindex);
			_okButton.Top = 108;
			_cancelButton.Top = 108;
			this.ClientSize = new Size(this.ClientSize.Width, 140);
		}
		private void AddUI(CID command, string text, string nim, int y, ref int tabindex) {
			Label l = new Label();
			l.Top = y;
			l.Left = 8;
			l.Width = 160;
			l.Height = 24;
			l.TextAlign = ContentAlignment.MiddleLeft;
			l.Text = text+nim;
			l.TabIndex = tabindex++;
			this.Controls.Add(l);

			ComboBox c = new ComboBox();
			c.DropDownStyle = ComboBoxStyle.DropDownList;
			c.Top = y;
			c.Left = 176;
			c.Width = 144;
			c.Height = 24;
			InitComboBoxOptions(c);

			KeyConfig.DirectionalEntry ent = _keyConfig.Find(command);
			_configComboBoxes.Add(c);
			c.SelectedIndex = ToSelectedIndex(ent);
			c.TabIndex = tabindex++;
			c.Tag = new ComboBoxTag(ent, text);
			c.SelectedIndexChanged += new EventHandler(OnSelectedIndexChanged);
			this.Controls.Add(c);
		}

		private static string[] _keyDescriptions = new string[] {"PageUp/Down", "�J�[�\���L�[�㉺", "�J�[�\���L�[���E", "�}�E�X�z�C�[��"};
		private void InitComboBoxOptions(ComboBox c) {
			foreach(string t in _keyDescriptions) {
				c.Items.Add(t);
			}						
			foreach(string t in _keyDescriptions) {
				c.Items.Add("Ctrl+"+t);
			}																				
			foreach(string t in _keyDescriptions) {
				c.Items.Add("Shift+"+t);
			}																				
		}

		//������Ƃ킩��ɂ����ϊ���������Ȃ������傤���Ȃ���Ȃ�

		private int ToSelectedIndex(KeyConfig.DirectionalEntry ent) {
			int a = ent.Modifiers==Keys.Control? 4 : ent.Modifiers==Keys.Shift? 8 : 0;
			switch(ent.Key) {
				case DirectionKey.PageUpDown:
					return 0 + a;
				case DirectionKey.UpDown:
					return 1 + a;
				case DirectionKey.LeftRight:
					return 2 + a;
				case DirectionKey.Wheel:
					return 3 + a;
			}
			return -1;
		}

		private void OnSelectedIndexChanged(object sender, EventArgs args) {
			ComboBox cb = (ComboBox)sender;
			KeyConfig.DirectionalEntry ent = (cb.Tag as ComboBoxTag).entry;
			switch(cb.SelectedIndex / 4) {
				case 0:
					ent.Modifiers = Keys.None;
					break;
				case 1:
					ent.Modifiers = Keys.Control;
					break;
				case 2:
					ent.Modifiers = Keys.Shift;
					break;
			}

			switch(cb.SelectedIndex % 4) {
				case 0:
					ent.Key = DirectionKey.PageUpDown;
					break;
				case 1:
					ent.Key = DirectionKey.UpDown;
					break;
				case 2:
					ent.Key = DirectionKey.LeftRight;
					break;
				case 3:
					ent.Key = DirectionKey.Wheel;
					break;
			}
		}

		private void OnOK(object sender, EventArgs args) {
			//�����ݒ肪��������x��
			for(int i=0; i<_configComboBoxes.Count-1; i++) {
				ComboBoxTag e1 = (_configComboBoxes[i] as ComboBox).Tag as ComboBoxTag;
				for(int j=i+1; j<_configComboBoxes.Count; j++) {
					ComboBoxTag e2 = (_configComboBoxes[j] as ComboBox).Tag as ComboBoxTag;
					if(e1.entry.Key==e2.entry.Key && e1.entry.Modifiers==e2.entry.Modifiers) {
						Util.Warning(this, String.Format("\"{0}\" �� \"{1}\" ���������蓖�ĂɂȂ��Ă��܂��B", e1.caption, e2.caption));
						this.DialogResult = DialogResult.None;
						return;
					}
				}
			}
			Env.Options.KeyConfig = _keyConfig;
		}
	}
}
