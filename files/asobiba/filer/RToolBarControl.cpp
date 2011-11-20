// RToolBarControl.cpp: RToolBarControl �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RToolBarControl.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RToolBarControl::RToolBarControl()
{

}

RToolBarControl::~RToolBarControl()
{
}

//�쐬
void RToolBarControl::Create(HINSTANCE inInstance,
			HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	RWindow::Create(inInstance,TOOLBARCLASSNAME,isParentWindow,inID,inFlg,0);
	
}

//�{�^���̍쐬
void RToolBarControl::CreateButton(HINSTANCE inInstance,
						const char* inBitmap,TBBUTTON* inButton,int inButtonCount)
{
	SendMessage(this->getWindowHandle() , TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0); 
	
	TBADDBITMAP	tbab;
	tbab.hInst = NULL;
	tbab.nID = (UINT) ::LoadImage(NULL , inBitmap,IMAGE_BITMAP , 0 , 0 ,LR_LOADFROMFILE);

	SendMessage(this->getWindowHandle(), TB_ADDBITMAP, inButtonCount, (LPARAM)&tbab);
	SendMessage(this->getWindowHandle(), TB_ADDBUTTONS, (WPARAM)inButtonCount, (LPARAM)inButton); 
}



//�g�����ڂ̃Z�b�g.
void RToolBarControl::setExtended(DWORD inStyle)
{
	SendMessage(this->getWindowHandle(),  TB_SETEXTENDEDSTYLE , 0, (LPARAM)inStyle);
}

//�g�����ڂ̎擾.
DWORD RToolBarControl::getExtended()
{
	return (DWORD) SendMessage(this->getWindowHandle(),  TB_GETEXTENDEDSTYLE , 0, 0);
}

