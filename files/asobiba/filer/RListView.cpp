// RListView.cpp: RListView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RListView.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RListView::RListView()
{
}

RListView::~RListView()
{
}

void RListView::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	RWindow::Create(inINstance,WC_LISTVIEW,isParentWindow,inID,inFlg,0);
}

//�g�����ڂ̃Z�b�g.
void RListView::setExtended(DWORD inStyle)
{
	ListView_SetExtendedListViewStyle(this->getWindowHandle(), inStyle); 
}

//�g�����ڂ̃Z�b�g.
DWORD RListView::getExtended()
{
	return ListView_GetExtendedListViewStyle(this->getWindowHandle() );
}

//�J���������ׂč폜����.
void RListView::ClearColumn()
{
	//���ׂ̃J�����̔j��
	while( ListView_DeleteColumn(this->getWindowHandle() ,0) );
}

//�J�����A�C�e���̒ǉ�.
bool RListView::InsertColumn(int inItem,int inSubItem,char* inString,int inWidth,UINT inMask,int inFmt)
{
	LV_COLUMN lvcol = {0};
	lvcol.mask = inMask;
	lvcol.fmt = inFmt;
	lvcol.cx = inWidth;
	lvcol.pszText = inString;
	lvcol.iSubItem = inSubItem;
	return Btob(ListView_InsertColumn(this->getWindowHandle() , inItem, &lvcol));
}

//�A�C�e���̒ǉ�.
bool RListView::SetItem(int inItem,int inSubItem,const char* inString)
{
	LVITEM	 lvitem = {0};
	lvitem.mask = LVIF_TEXT ;
	lvitem.pszText = (char*)inString;
	lvitem.iItem = inItem;
	lvitem.iSubItem = inSubItem;
	if (inSubItem == 0)
		return Btob(ListView_InsertItem(this->getWindowHandle() , &lvitem));
	else
		return Btob(ListView_SetItem(this->getWindowHandle() , &lvitem));
}

//�A�C�e���̒ǉ�.
bool RListView::SetItem(int inItem,int inSubItem,int inInt)
{
	char buf[20];
	return SetItem(inItem,inSubItem, itoa( inInt , buf , 10) );
}

//�A�C�e���̎擾
string RListView::GetItem(int inItem,int inSubitem)
{
	char buf[MAX_PATH];

	LVITEM	 lvitem = {0};
	lvitem.mask = LVIF_TEXT ;
	lvitem.pszText = buf;
	lvitem.cchTextMax = MAX_PATH;
	lvitem.iItem = inItem;
	lvitem.iSubItem = inSubitem;
	bool ret = Btob( ListView_GetItem(this->getWindowHandle() , &lvitem) );
	if (!ret) return "";

	return lvitem.pszText;
}



//���z���X�g�̏ꍇ�̍��ڐ��̐ݒ�
void  RListView::SetItemCountEx(int inCount,DWORD inFlg)
{
	ListView_SetItemCountEx(this->getWindowHandle() , inCount, inFlg);
}
//���z���X�g�̏ꍇ�̍��ڐ��̐ݒ�
void  RListView::SetItemCount(int inCount) 
{
	ListView_SetItemCount(this->getWindowHandle() , inCount);
}

//���ݑI������Ă���Ƃ���̎擾.
int	RListView::getNowSelected() const
{
	return ListView_GetNextItem(this->getWindowHandle() ,-1,LVNI_SELECTED);
}




//�A�C�e���̐��̎擾
int RListView::GetItemCount() const
{
	return ListView_GetItemCount(this->getWindowHandle() );
}

//inSearchInt������ʒu���T�[�`.
int RListView::SearchItem(int inSubItem , int inSearchInt)
{
	char buf[20];
	itoa( inSearchInt , buf , 10);

	int len = GetItemCount();

	for(int i  = 0; i < len ; i++)
	{
		if ( strcmp(GetItem( i ,inSubItem ).c_str() , buf) == 0 )return i;
	}
	return -1;
}

//�A�C�e���̍폜
bool RListView::DeleteItem(int inItem)
{
	ASSERT(inItem != -1);
	return Btob( ListView_DeleteItem(this->getWindowHandle() , inItem) );
}



//�C���[�W���X�g�Z�b�g.
bool RListView::setImageList(const HIMAGELIST inImageLIst , int inType)
{	
	return ListView_SetImageList(this->getWindowHandle() ,(HIMAGELIST)inImageLIst , inType) != NULL;
}



//�w�肵���A�C�e���̏�Ԃ�ύX���܂��B
void RListView::SetItemState(int inItem , int inState,int inMask)
{
	ASSERT(inItem != -1);

	ListView_SetItemState(this->getWindowHandle() ,inItem , inState,inMask);
}

//�A�C�e���Ƃ̃q�b�g�e�X�g
int RListView::HitTest(const POINT inPoint) const
{
	LVHITTESTINFO hit = {0};
	hit.pt = inPoint;

	return ListView_HitTest(this->getWindowHandle() , &hit);
}


//�w�肵���A�C�e���̏�Ԃ��擾���܂��B
int RListView::GetItemState(int inItem , int inMask) const
{
	ASSERT(inItem != -1);
	return ListView_GetItemState(this->getWindowHandle() ,inItem, inMask);
}

//���ׂĂ̑I��������.
void RListView::ResetSelect()
{
	int index = -1;
	while((index = ListView_GetNextItem(this->getWindowHandle(), index, LVNI_ALL | LVNI_SELECTED))!=-1)
	{
		SetItemState(index , 0 , LVIS_SELECTED | LVIS_FOCUSED);
	}
}

//���݃}�E�X�J�[�\�����̂������Ă���A�C�e�����擾���܂��B
int RListView::HitItem() const
{
	//�ǂ����I������Ă��Ȃ��ꍇ�́A�L�����Z������.
	POINT pt;
	GetCursorPos(&pt);

	//�N���C�A���g���W�Ɏ����Ă���..
	ScreenToClient(this->getWindowHandle() , &pt);

	//�Q�l http://www.alpha-net.ne.jp/users2/uk413/vc/VCT_LVitem.html
	return this->HitTest(pt) ;
}