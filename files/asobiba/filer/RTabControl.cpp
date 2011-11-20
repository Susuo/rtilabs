// RTabControl.cpp: RTabControl �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTabControl.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTabControl::RTabControl()
{

}

RTabControl::~RTabControl()
{
}

//�쐬
void RTabControl::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,WC_TABCONTROL,isParentWindow,inID,inFlg,0);
}

//�^�u�����ׂč폜����.
bool RTabControl::ClearTab()
{
	return Btob( TabCtrl_DeleteAllItems(this->getWindowHandle() ) );
}

//�^�u�̒ǉ�.
bool RTabControl::InsertTab(int inItem,const char* inText,LPARAM inLPARAM)
{
	TCITEM	tc = {0};
	tc.mask = TCIF_TEXT | TCIF_PARAM;
	tc.pszText = (char*)inText;
	tc.lParam = inLPARAM;
	return Btob( TabCtrl_InsertItem(this->getWindowHandle() , inItem , &tc)  );
}

//�^�u�̕ύX.
bool RTabControl::UpdateTab(int inItem,const char* inText,LPARAM inLPARAM)
{
	TCITEM	tc = {0};
	tc.mask = TCIF_TEXT | TCIF_PARAM;
	tc.pszText = (char*)inText;
	tc.lParam = inLPARAM;
	return Btob( TabCtrl_SetItem(this->getWindowHandle() , inItem , &tc)  );
}
//�^�u�̎擾
LPARAM RTabControl::GetTabParam(int inItem) const
{
	//�擾
	TCITEM	tc = {0};
	tc.mask = TCIF_PARAM;
	TabCtrl_GetItem(this->getWindowHandle() , inItem , &tc  );

	return tc.lParam;
}

//�^�u�̍폜
bool RTabControl::DeleteTab(int inItem)
{
	return Btob( TabCtrl_DeleteItem(this->getWindowHandle() , inItem) );
}

//�^�u�̐��̎擾
int RTabControl::GetTabCount() const
{
	//�^�u���̎擾.
	return TabCtrl_GetItemCount(this->getWindowHandle() );
}

//���ݑI������Ă���^�u�̈ʒu���擾.
int RTabControl::GetCurSel() const
{
	return TabCtrl_GetCurSel(this->getWindowHandle() );
}

//�w�肵���^�u��I��.
void RTabControl::SelectTab(int inItem)
{
	TabCtrl_SetCurFocus( this->getWindowHandle() ,inItem);
}




