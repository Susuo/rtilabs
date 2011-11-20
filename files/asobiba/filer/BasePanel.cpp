// BasePanel.cpp: BasePanel �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "BasePanel.h"
#include "WindowsMain.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BasePanel::BasePanel()
{
	this->WindowHandle = NULL;
}

BasePanel::~BasePanel()
{
	::DestroyWindow(this->WindowHandle);
}

//�E�B���h�E�̍쐬
//���s����RDoNotInitaliseException��O.
void BasePanel::Create(bool isLeft) throw()
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	

    this->WindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE , "STATIC",
				            "",WS_CHILD | WS_VISIBLE | WS_THICKFRAME ,
						    CW_USEDEFAULT, CW_USEDEFAULT,
							CW_USEDEFAULT, CW_USEDEFAULT,
							mainSystem->getWindowHandle(), (HMENU)mainSystem->getID(),
							mainSystem->getInstance(), NULL);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("BasePanel�̍쐬�Ɏ��s");
	}
	this->OrignalWndProc = (WNDPROC)GetWindowLong(this->WindowHandle , GWL_WNDPROC );
	SetWindowLong(this->WindowHandle , GWL_WNDPROC ,  (long) WndProcStatic);
	SetWindowLong(this->WindowHandle , GWL_USERDATA , (long)this);
}


//�E�B���h�E�v���V�[�W��(none static)
LRESULT BasePanel::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT ret;
	for ( WNDPROCCALLABLE::iterator it = WndProcCallOnList.begin() ; it != WndProcCallOnList.end() ; ++it)
	{
		//���������ł����� true ���A���Ă��āA ret �ɖ߂�n���Z�b�g����Ă���̂�
		//�����ς����܂��B
		if ( (*it)->WndProc(hwnd,msg, wparam, lparam,&ret) )
		{
			return ret;
		}
	}
	return ::CallWindowProc( OrignalWndProc ,hwnd,msg,wparam,lparam);
}

