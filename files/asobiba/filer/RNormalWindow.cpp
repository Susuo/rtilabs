// RNormalWindow.cpp: RNormalWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNormalWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RNormalWindow::RNormalWindow()
{

}

RNormalWindow::~RNormalWindow() 
{
	//�T�u�N���X����
	SetWindowLong(this->getWindowHandle() , GWL_USERDATA , NULL);
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RNormalWindow::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw()
{

    static WNDCLASS	wc = {0};
	if (! wc.lpszClassName )
	{
		wc.lpszClassName = "NormalWindow";
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)WndProcStatic;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = inINstance;
		wc.hIcon         = NULL;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName  = NULL;
		if(!RegisterClass(&wc))
		{
			throw RDoNotInitaliseException("�E�B���h�E�N���X�̍쐬�Ɏ��s���܂���");
		}

	}

	RWindow::Create(inINstance,wc.lpszClassName,isParentWindow,inID,inFlg  ,0);

	//��[���p�p�A�T�u�N���X�ɂ����Ⴄ���[
	SetWindowLong(this->getWindowHandle() , GWL_USERDATA , (long)this);
}
