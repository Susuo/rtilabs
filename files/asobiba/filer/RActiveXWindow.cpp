// RActiveXWindow.cpp: RActiveXWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RActiveXWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RActiveXWindow::RActiveXWindow()
{
	this->Unknown = NULL;
}

RActiveXWindow::~RActiveXWindow()
{
	if (this->Unknown)
	{
		this->Unknown->Release();
		this->Unknown = NULL;
	}
}
#include <Exdisp.h>

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RActiveXWindow::Create(HINSTANCE inINstance, const string inProgID, IID inIID , void** ioInterface,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inExStyle) 
					   throw(RNotSupportException,RDoNotInitaliseException)
{
	USES_CONVERSION;
	AtlActiveXBrige *aaxb = AtlActiveXBrige::getInstance();

	HWND hwndChild = ::CreateWindowEx(inExStyle,"AtlAxWin",
                                    inProgID.c_str() , 
                                    inFlg | WS_CHILD|WS_VISIBLE,
                                    0,0,0,0,
                                    isParentWindow,
									(HMENU)inID,
                                    inINstance,
                                    NULL);

	MountWindow(hwndChild);

	ASSERT(Unknown == NULL);
	if (!SUCCEEDED(aaxb->AtlAxGetControl( this->getWindowHandle()  ,(IUnknown **) &Unknown)))
	{
		throw RNotSupportException("AtlAxGetControl�Ɏ��s���܂���");
	}

	
    // get an interface .
	if ( !SUCCEEDED( Unknown->QueryInterface(inIID, ioInterface) ) )
	{
		throw RNotSupportException("�C���^�[�t�F�[�X�\�z�Ɏ��s���܂���");
	}

	if (! (*ioInterface))
	{
		throw RNotSupportException("�C���^�[�t�F�[�X�\�z(QueryInterface)�� SUCCEEDED �ł����A���ۂɍ\�z����Ă��܂���");
	}

}
