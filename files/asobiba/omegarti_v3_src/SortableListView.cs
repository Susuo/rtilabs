using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using UtilityLibrary.Win32;

namespace Zanetti.UI
{
	/// <summary>
	/// �\�[�g�@�\�t�� ListView�B
	/// </summary>
	internal class SortableListView : ListView
	{
		private int _listSortKey;	// ���X�g�̃\�[�g�L�[ (0:�������, ��:��ԍ�����, ��:��ԍ��~��)
		private ListSorter _listSorter;

		public SortableListView()
		{
			_listSortKey = 0;
			_listSorter = new ListSorter(this);
			ColumnClick += new ColumnClickEventHandler(OnColumnClicked);
		}

		/// <summary>
		/// ListViewItem �̔�r�B
		/// </summary>
		/// <param name="item1">��r�Ώ�1</param>
		/// <param name="item2">��r�Ώ�2</param>
		/// <param name="sortKey">��r���邽�߂̃L�[�i0:�������, ��:��ԍ�����, ��:��ԍ��~���j</param>
		/// <param name="columnHeader">�\�[�g�Ώۂ� ColumnHeader</param>
		/// <returns>��r����</returns>
		public virtual int CompareItem(ListViewItem item1, ListViewItem item2, int sortKey, ColumnHeader columnHeader) {
			if(sortKey != 0){
				// ���ڂ̏��� or �~���\�[�g
				int index = Math.Abs(sortKey) - 1;
				int illegal1 = 0;
				int illegal2 = 0;
				string str1 = item1.SubItems[index].Text;
				string str2 = item2.SubItems[index].Text;
				int result = 0;
				if(columnHeader.TextAlign == HorizontalAlignment.Right){
					// �E�񂹂̏ꍇ�͐��l�\�[�g
					double d1 = 0.0;
					double d2 = 0.0;
					int len1 = GetNumberLen(str1);
					int len2 = GetNumberLen(str2);
					if(len1 > 0) d1 = double.Parse(str1.Substring(0, len1));
					else illegal1 = 1;
					if(len2 > 0) d2 = double.Parse(str2.Substring(0, len2));
					else illegal2 = 1;
					if(d1 < d2) result = -1;
					else if(d1 > d2) result = 1;

					// ���ɔ񐔂̏ꍇ�̏���
					if((illegal1 & illegal2) == 1){
						illegal1 = illegal2 = 0;
						if(str1.Length == 0) illegal1 = 1;
						if(str2.Length == 0) illegal2 = 1;
						result = str1.CompareTo(str2);
					}
				}
				else{
					// �E�񂹈ȊO�̏ꍇ�͕�����\�[�g
					if(str1.Length == 0) illegal1 = 1;
					if(str2.Length == 0) illegal2 = 1;
					result = str1.CompareTo(str2);
				}

				// ���������肷��ꍇ�͂����Ԃ�
				if(illegal1 != illegal2){
					// �󔒂�񐔂͌�̕��ɒǂ��o������
					return illegal1 - illegal2;
				}
				if(result != 0){
					return sortKey * result;
				}
			}
			
			// �f�t�H���g�̃\�[�g
			if((item1.Tag is IComparable) && (item2.Tag is IComparable)){
				IComparable c1 = (IComparable) item1.Tag;
				IComparable c2 = (IComparable) item2.Tag;
				return c1.CompareTo(c2);
			}

			return 0;
		}

		private void OnColumnClicked(object sender, ColumnClickEventArgs e) {
			int sortKey = e.Column + 1;
			ListHeader header = getHeader();
			HDITEM hdi = new HDITEM();
			int index;

			hdi.mask = (uint)HeaderItemFlags.HDI_FORMAT;

			// �w�b�_�[�̈ȑO�̃\�[�g�\�������ɖ߂�
			if(_listSortKey != 0){
				index = Math.Abs(_listSortKey) - 1;
				header.GetItem(index, ref hdi);
				hdi.fmt &= ~(int)(HeaderControlFormats.HDF_SORTDOWN | HeaderControlFormats.HDF_SORTUP);
				header.SetItem(index, ref hdi);
			}

			if(_listSortKey == sortKey){
				// ���ݏ����Ȃ�~���ɂ���
				_listSortKey *= -1;
			}
			else if(_listSortKey == -sortKey){
				// ���ݍ~���Ȃ珉����Ԃɖ߂�
				_listSortKey = 0;
			}
			else{
				// ���̑��̏ꍇ�́A���ڂŏ����\�[�g
				_listSortKey = sortKey;
			}

			// �w�b�_�[�Ƀ\�[�g�\���i�����j���s��
			if(_listSortKey != 0){
				index = Math.Abs(_listSortKey) - 1;
				header.GetItem(index, ref hdi);
				hdi.fmt |= (int)(_listSortKey > 0 ?
									HeaderControlFormats.HDF_SORTUP :
									HeaderControlFormats.HDF_SORTDOWN);
				header.SetItem(index, ref hdi);
			}

			// �\�[�g
			_listSorter.SetKey(_listSortKey);
			ListViewItemSorter = _listSorter;
			Sort();
		}

		// ���l�Ƃ��ėL���ȕ�������Ԃ�
		private static int GetNumberLen(string str){
			int n = str.Length;
			if(n <= 0){
				return 0;
			}
			char c = str[0];
			if((c < '0' || '9' < c) && c != '-' && c != '+'){
				return 0;
			}
			for(int i = 1; i < n; i++){
				c = str[i];
				if((c < '0' || '9' < c) && c != '.'){
					return i;
				}
			}
			
			return n;
		}

		private ListHeader getHeader() {
			// TODO: �\�ȏꍇ�� ListHeader ���L���b�V������Ə��������Ȃ�B
			return new ListHeader(Handle);
		}

		private class ListSorter : IComparer {
			int _sortKey; // �\�[�g�L�[ (0:�������, ��:��ԍ�����, ��:��ԍ��~��)
			ColumnHeader _columnHeader; // �\�[�g�Ώۂ� ColumnHeader
			SortableListView _list; // �\�[�g�Ώۂ� ListView

			public ListSorter(SortableListView list) {
				_list = list;
			}

			public void SetKey(int sortKey) {
				_sortKey = sortKey;

				if(_sortKey != 0){
					int index = Math.Abs(_sortKey) - 1;
					_columnHeader = (ColumnHeader)_list.Columns[index];
				}
				else{
					_columnHeader = null;
				}
			}

			public int Compare(object x, object y) {
				return _list.CompareItem((ListViewItem)x, (ListViewItem)y,
											_sortKey, _columnHeader);
			}
		}

		private class ListHeader {
			private IntPtr _hwnd;

			public ListHeader(IntPtr hwndList){
				_hwnd = WindowsAPI.SendMessage(hwndList, ListViewMessages.LVM_GETHEADER, 0, 0);
				Debug.Assert(_hwnd != IntPtr.Zero);
			}

			public int GetItemCount(){
				if(_hwnd == IntPtr.Zero){
					return 0;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_GETITEMCOUNT, 0, 0);
			}

			public bool GetItem(int i, ref HDITEM phdi){
				if(_hwnd == IntPtr.Zero){
					return false;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_GETITEMW, i, ref phdi);
			}

			public bool SetItem(int i, ref HDITEM phdi){
				if(_hwnd == IntPtr.Zero){
					return false;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_SETITEMW, i, ref phdi);
			}
		}
	}
}

