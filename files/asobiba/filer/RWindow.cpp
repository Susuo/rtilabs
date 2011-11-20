// RWindow.cpp: RWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RWindow::RWindow()
{
	WindowHandle = NULL;
}

RWindow::~RWindow()
{
	if (this->WindowHandle)
		::DestroyWindow( this->WindowHandle );
}

//�쐬
void RWindow::Create(HINSTANCE inINstance, const char* inClassName,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw()
{
	this->ID = inID;

	this->WindowHandle = ::CreateWindowEx(inFlgEx, inClassName, "",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, NULL);
	if (this->WindowHandle == NULL) throw RDoNotInitaliseException("�������Ɏ��s���܂���");
}
/*
//�쐬
void RWindow::CreateActiveX(HINSTANCE inINstance, 
					   HWND isParentWindow,int inID,DWORD inFlg,DWORD inFlgEx) throw()
{
	this->ID = inID;

	this->WindowHandle = ::CreateWindowEx(inFlgEx, inClassName, "aaa",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU) inINstance, NULL);
	if (this->WindowHandle == NULL) throw RDoNotInitaliseException("�������Ɏ��s���܂���");
}
*/

//�ړ�.
void RWindow::MoveWindow(int x,int y,int width,int height) const
{
	::MoveWindow( this->WindowHandle , x , y , width , height ,TRUE);
}

//�ʒu�����ړ�.
void RWindow::MoveWindow(int x,int y) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , x , y, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
}

//���ƍ��������ړ�.
void RWindow::ResizeWindow(int width,int height) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , 0 , 0, width , height , SWP_NOMOVE | SWP_NOZORDER);
}


//�ő�\��
void RWindow::FullWindow(HWND inParentWidowHandle) const
{
	RECT rc;
	::GetClientRect(inParentWidowHandle , &rc);

	this->MoveWindow(0,0,rc.right,rc.bottom);
}


//�\������
void RWindow::Show() const
{
    ::ShowWindow(this->WindowHandle,SW_SHOW);
    ::UpdateWindow(this->WindowHandle);
}

//��\��
void RWindow::Hide() const
{
    ::ShowWindow(this->WindowHandle,SW_HIDE);
}


//�X�^�C���̃Z�b�g.
void RWindow::setStyle(DWORD inStyle)
{
	SetWindowLong(this->getWindowHandle(), GWL_STYLE, (LONG)inStyle);
}

//�X�^�C���̎擾.
DWORD RWindow::getStyle()
{
	return GetWindowLong(this->getWindowHandle(), GWL_STYLE);
}


//ClientRect �̎擾.
void RWindow::GetClientRect(RECT * outRC) const
{
	::GetClientRect( this->getWindowHandle() , outRC);
}

//WindowRect �̎擾.
void RWindow::GetWindowRect(RECT * outRC) const
{
	::GetWindowRect( this->getWindowHandle() , outRC);
}
