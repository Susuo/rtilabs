/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;

using Zanetti.Config;
using Zanetti.Data;		// for use Brand
using Zanetti.Commands;

namespace Zanetti.UI
{
	internal class TreeViewWithCustomKey : TreeView {
        public TreeViewWithCustomKey() {
            this.BeforeLabelEdit += new NodeLabelEditEventHandler(OnBeforeLabelEdit);
            this.AfterLabelEdit += new NodeLabelEditEventHandler(OnAfterLabelEdit);
        }

        protected bool _editingLabel;
        public bool EditingLabel {
            get {
                return _editingLabel;
            }
            set {
                _editingLabel = value;
            }
        }

		/*
		protected override bool ProcessKeyMessage(ref Message m) {
			if(!_editingLabel && Env.Command.Exec(ref m)!=CommandResult.Ignored)
				return true;
			else {
				return base.ProcessKeyMessage(ref m);
			}
		}
		*/
		protected override bool IsInputKey(Keys keyData) {
			bool r = base.IsInputKey(keyData);
			//Debug.WriteLine("IsInputKey(TV) " + r + keyData.ToString());
			return r;
		}

		protected override bool ProcessDialogKey(Keys keyData) {
			if(_editingLabel) return false;
			bool r = base.ProcessDialogKey(keyData);
			//Debug.WriteLine("ProcessDialogKey(TV) " + r + keyData.ToString());
			return r;
		}
		protected override bool ProcessCmdKey(ref Message msg, Keys keyData) {
			bool r = base.ProcessCmdKey(ref msg, keyData);
			//Debug.WriteLine("ProcessCmdKey(TV) " + r + keyData.ToString());
			if(_editingLabel)
				return r;
			else if(Env.Frame.IsPriorShortcutKey(keyData))
				return Env.Frame.ProcessShortcut(keyData);
			else
				return r;
		}


        private void OnBeforeLabelEdit(object sender, NodeLabelEditEventArgs args) {
			//Debug.WriteLine("before labeledit");
            _editingLabel = true;
        }
        private void OnAfterLabelEdit(object sender, NodeLabelEditEventArgs args) {
			//Debug.WriteLine("after labeledit");
			_editingLabel = false;
        }

    }

	/// <summary>
	/// BookmarkPane �̊T�v�̐����ł��B
	/// </summary>
	internal class BookmarkPane : BrandListPane, IBookmarkEventListener
	{

		const int DD_EXPAND_FOLDER_WAIT = 500;
		const int DD_SCROLL_WAIT = 50;

		//private bool _ignoreBookmarkEvent;
		private TreeNode _rootNode;
		private ContextMenu _contextMenu;
		private DateTime _dragOverTime;

		private TreeViewWithCustomKey _treeView;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public BookmarkPane()
		{
			// ���̌Ăяo���́AWindows.Forms �t�H�[�� �f�U�C�i�ŕK�v�ł��B
			InitializeComponent();

			// TODO: InitializeComponent �Ăяo���̌�ɏ�����������ǉ����܂��B
			//InitTreeView();
			
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
				Env.Bookmark.EventListener = null;
			}
			base.Dispose( disposing );
		}

		#region �R���|�[�l���g �f�U�C�i�Ő������ꂽ�R�[�h 
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
		/// </summary>
		private void InitializeComponent()
		{
			this._treeView = new TreeViewWithCustomKey();
			this.SuspendLayout();
			// 
			// _treeView
			// 
			this._treeView.AllowDrop = true;
			this._treeView.Dock = System.Windows.Forms.DockStyle.Fill;
			this._treeView.HotTracking = true;
			this._treeView.LabelEdit = true;
			this._treeView.Location = new System.Drawing.Point(0, 0);
			this._treeView.Name = "_treeView";
			this._treeView.ShowRootLines = false;
			this._treeView.Sorted = false;
			this._treeView.TabIndex = 0;
			this._treeView.ImageList = Env.ImageList16;
			this._treeView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnTreeViewClicked);
			this._treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.OnAfterSelect);
			this._treeView.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this._treeView_AfterLabelEdit);
			this._treeView.BeforeLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this._treeView_BeforeLabelEdit);
			this._treeView.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.OnItemDrag);
			this._treeView.DragEnter += new System.Windows.Forms.DragEventHandler(this.OnDragEnter);
			this._treeView.DragOver += new System.Windows.Forms.DragEventHandler(this.OnDragOver);
			this._treeView.DragDrop += new System.Windows.Forms.DragEventHandler(this.OnDragDrop);
			
			// 
			// BookmarkPane
			// 
			this.Controls.Add(this._treeView);
			this.Name = "BookmarkPane";
			this.ResumeLayout(false);

		}
		#endregion

		private void InitTreeView() {
			_treeView.BeginUpdate();
			_rootNode = Env.Bookmark.Root.CreateTreeNode(false);
			_treeView.Nodes.Add(_rootNode);
			_treeView.SelectedNode = _rootNode;
			_rootNode.Expand();
			_treeView.EndUpdate();
		}

		private void InitContextMenu() {
			_contextMenu = new ContextMenu();
			Util.AddMenuItem(_contextMenu, "�t�H���_�̍쐬(&F)", new EventHandler(OnCreateFolderMenu));
			Util.AddMenuItem(_contextMenu, "�\�������̒ǉ�(&B)", new EventHandler(OnAddBookmark));
            Util.AddMenuItem(_contextMenu, "���O�̕ύX(&R)", new EventHandler(OnRenameBookmark));
            Util.AddMenuItem(_contextMenu, "�폜(&R)", new EventHandler(OnRemoveMenu));
        }

		//OnLoad��InitTreeView�Ȃ���Ȃ����A�R���X�g���N�^�ŌĂԂƕςȃX�N���[���o�[��Tree���ɏo�Ă��܂�
		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			InitTreeView();
			Env.Bookmark.EventListener = this;
		}

		private TreeNode FindNode(BookmarkItem item) {
			if(item.Parent==null)
				return _treeView.Nodes[0]; //root
			else {
				TreeNode p = FindNode(item.Parent);
				foreach(TreeNode ch in p.Nodes)
					if(ch.Tag==item) return ch;
				return null;
			}
		}
		private void AdjustContextMenu(BookmarkItem item) {
			bool d = item!=null && item is BookmarkFolder;
			bool r = item!=_rootNode.Tag;

            MenuItem.MenuItemCollection c = _contextMenu.MenuItems;
            c[0].Enabled = d; // �t�H���_�쐬
            c[1].Enabled = d; // �����ǉ�
            c[2].Enabled = d; // rename
            c[3].Enabled = r; // �폜
		}

		private void _treeView_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e) {
			if(e.Node.Tag is BrandBookmark)
				e.CancelEdit = true;
		}
		private void _treeView_AfterLabelEdit(object sender, NodeLabelEditEventArgs e) {
			((BookmarkFolder)e.Node.Tag).Name = e.Label;
		}

		private void OnAfterSelect(object sender, TreeViewEventArgs e) {
			BrandBookmark m = e.Node.Tag as BrandBookmark;
			if(m!=null)
				CommandExec.ShowBrand(Env.BrandCollection.FindBrand(m.Code));
		}
		private void OnTreeViewClicked(object sender, MouseEventArgs args) {
			if(args.Button!=MouseButtons.Right) return;
			if(_contextMenu==null) InitContextMenu();

			TreeNode nd = _treeView.GetNodeAt(args.X, args.Y);
			if(nd!=null) {
				_treeView.SelectedNode = nd;
				AdjustContextMenu(nd.Tag as BookmarkItem);
				_contextMenu.Show(this, new Point(args.X, args.Y));
			}
		}

		private void OnAddBookmark(object sender, EventArgs args) {
			AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
			// �������d�����ĂĂ��G���[�ʒm���Ȃ�
			BookmarkFolder bfp = _treeView.SelectedNode.Tag as BookmarkFolder;
			if(bfp != null) 
				bfp.AddChild(new BrandBookmark(bfp,br.Code),null);
		}
		private void OnCreateFolderMenu(object sender, EventArgs args) {
			BookmarkFolder bfp = _treeView.SelectedNode.Tag as BookmarkFolder;
			if(bfp != null) {
				BookmarkFolder bf = new BookmarkFolder(bfp);
				bf.Name = "�V�����t�H���_";
				bfp.AddChild(bf,null);
			}
		}
		private void OnRemoveMenu(object sender, EventArgs args) {
			BookmarkItem m = _treeView.SelectedNode.Tag as BookmarkItem;
			if(m!=null)
				m.Parent.Remove(m);
		}
        private void OnRenameBookmark(object sender, EventArgs args) {
            BookmarkFolder f = _treeView.SelectedNode.Tag as BookmarkFolder;
            if(f != null)
                _treeView.SelectedNode.BeginEdit();
        }
        private void OnRemoveAllMenu(object sender, EventArgs args) {
            DialogResult dr = Util.AskUserYesNo(this,"���C�ɓ����S�č폜���Ă�낵���ł����H");
			if(dr == DialogResult.Yes) {
				_rootNode.Nodes.Clear();
				Env.Bookmark.Clear();
			}
		}
		private void OnItemDrag(object sender, System.Windows.Forms.ItemDragEventArgs  args) {
			_treeView.DoDragDrop((TreeNode)args.Item, DragDropEffects.Move);
		}
		private void OnDragEnter(object sender, System.Windows.Forms.DragEventArgs args) {
			args.Effect = DragDropEffects.Move;
			_dragOverTime = DateTime.Now;
		}
		private void OnDragOver(object sender, System.Windows.Forms.DragEventArgs args){
			Point p = _treeView.PointToClient(new Point(args.X,args.Y));
			TreeNode item = _treeView.GetNodeAt(p.X,p.Y);
            if(item == null) return;

            double d = DateTime.Now.Subtract(_dragOverTime).TotalMilliseconds;
			if(d >= DD_EXPAND_FOLDER_WAIT){
				item.Expand();
				_dragOverTime = DateTime.Now;
			}
			else if(d >= DD_SCROLL_WAIT){
				if(item.PrevVisibleNode!=null && !item.PrevVisibleNode.IsVisible){
					item.PrevVisibleNode.EnsureVisible();
					_dragOverTime = DateTime.Now;					
				}
				if(item.NextVisibleNode!=null && !item.NextVisibleNode.IsVisible)
				{
					item.NextVisibleNode.EnsureVisible();
					_dragOverTime = DateTime.Now;
				}
			}
			_treeView.SelectedNode = item; 
		}
		private void OnDragDrop(object sender, System.Windows.Forms.DragEventArgs args){
			Point p = _treeView.PointToClient(new Point(args.X,args.Y));
			TreeNode src = (TreeNode)args.Data.GetData(typeof(TreeNode));
			TreeNode dest = _treeView.GetNodeAt(p.X,p.Y);
			
			if(dest.Tag is BookmarkItem){
				BookmarkItem srcItem = (BookmarkItem)src.Tag;
				if(dest.Tag is BrandBookmark){
					BookmarkFolder destFolder = (BookmarkFolder)dest.Parent.Tag;
					BookmarkItem neighborItem = (BookmarkItem)dest.Tag;
					if(srcItem != neighborItem)
						srcItem.Move(destFolder,neighborItem);
				}
				else{
					BookmarkFolder destFolder = (BookmarkFolder)dest.Tag;
					srcItem.Move(destFolder,null);
				}
			}
		}
		
		//IBookmarkEventListener �����o

		public void OnChildAdded(BookmarkFolder parent, BookmarkItem child, BookmarkItem neighbor) {
			//if(_ignoreBookmarkEvent) return;
			TreeNode r = child.CreateTreeNode(false);	// Drag&Drag�ł�Event�����̂���false
			TreeNode p = FindNode(parent);
			if(neighbor!=null){
				int ii = p.Nodes.IndexOf(FindNode(neighbor));		// ������ƐM����
				p.Nodes.Insert(ii,r);
			}
			else{
				p.Nodes.Add(r);
			}

            if(child is BookmarkFolder) {
                _treeView.EditingLabel = true;
                r.BeginEdit(); //�t�H���_�쐬�͂����ɕҏW�J�n
            } else
                _treeView.SelectedNode = r;
        }

		public void OnChildRemoved(BookmarkFolder parent, BookmarkItem child) {
			//if(_ignoreBookmarkEvent) return;
			TreeNode r = FindNode(child);
			r.Remove();
		}
		public override AbstractBrand NextBrand {
			get {
				TreeNode s = _treeView.SelectedNode;
				if(s==null) return null;
				TreeNode n = s.NextNode;
				if(n==null) return null;
				_treeView.SelectedNode = n;
				return Env.BrandCollection.FindBrand((n.Tag as BrandBookmark).Code);
			}
		}
		public override AbstractBrand PrevBrand {
			get {
				TreeNode s = _treeView.SelectedNode;
				if(s==null) return null;
				TreeNode n = s.PrevNode;
				if(n==null) return null;
				_treeView.SelectedNode = n;
				return Env.BrandCollection.FindBrand((n.Tag as BrandBookmark).Code);
			}
		}

	}
}
