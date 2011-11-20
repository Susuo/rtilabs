// RWindowBase.cpp: RWindowBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RWindowBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RWindowBase::RWindowBase()
{
	WindowHandle = NULL;
}

RWindowBase::~RWindowBase()
{
	if (this->WindowHandle)
		::DestroyWindow( this->WindowHandle );
}


//�ړ�.
void RWindowBase::MoveWindow(int x,int y,int width,int height) const
{
	::MoveWindow( this->WindowHandle , x , y , width , height ,TRUE);
}

//�ʒu�����ړ�.
void RWindowBase::MoveWindow(int x,int y) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , x , y, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
}

//���ƍ��������ړ�.
void RWindowBase::ResizeWindow(int width,int height) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , 0 , 0, width , height , SWP_NOMOVE | SWP_NOZORDER);
}


//�ő�\��
void RWindowBase::FullWindow(HWND inParentWidowHandle) const
{
	RECT rc;
	::GetClientRect(inParentWidowHandle , &rc);

	this->MoveWindow(0,0,rc.right,rc.bottom);
}


//�\������
void RWindowBase::Show() const
{
    ::ShowWindow(this->WindowHandle,SW_SHOW);
    ::UpdateWindow(this->WindowHandle);
}

//��\��
void RWindowBase::Hide() const
{
    ::ShowWindow(this->WindowHandle,SW_HIDE);
}


//�X�^�C���̃Z�b�g.
void RWindowBase::setStyle(DWORD inStyle)
{
	SetWindowLong(this->getWindowHandle(), GWL_STYLE, (LONG)inStyle);
}

//�X�^�C���̎擾.
DWORD RWindowBase::getStyle()
{
	return GetWindowLong(this->getWindowHandle(), GWL_STYLE);
}


//ClientRect �̎擾.
void RWindowBase::GetClientRect(RECT * outRC) const
{
	::GetClientRect( this->getWindowHandle() , outRC);
}

//WindowRect �̎擾.
void RWindowBase::GetWindowRect(RECT * outRC) const
{
	::GetWindowRect( this->getWindowHandle() , outRC);
}

string RWindowBase::getText() const
{
#ifndef _WIN32_WCE
	char buf[MAX_PATH];
	GetWindowText(this->getWindowHandle() , buf , MAX_PATH);

	return string(buf);
#else
	MY_USES_CONVERSION;
	TCHAR buf[MAX_PATH];
	GetWindowText(this->getWindowHandle() , buf , MAX_PATH);

	return string(_W2A(buf));
#endif
}

void RWindowBase::setText(const string & inStr)
{
#ifndef _WIN32_WCE
	SetWindowText(this->getWindowHandle() , inStr.c_str() );
#else
	MY_USES_CONVERSION;
	SetWindowText(this->getWindowHandle() , _A2W(inStr.c_str()) );
#endif
}

//SendMessage
LRESULT RWindowBase::SendMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::SendMessage(this->getWindowHandle() , inMsg , wParam , lParam );
}
//PostMessage
LRESULT RWindowBase::PostMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::PostMessage(this->getWindowHandle() , inMsg , wParam , lParam );
}

void RWindowBase::EnableWindow(bool inEnable)
{
	::EnableWindow(this->getWindowHandle() , btoB( inEnable ) );
}


//�w�肵���R���g���[���� �����̃E�B���h�E���W��̂ǂ��ɂ��邩?
void RWindowBase::getControlClientRect(HWND inTarget , RECT * outRC) const
{
	::GetWindowRect( inTarget , outRC );

	POINT pt;
	pt.x = outRC->left;
	pt.y = outRC->top;
	ScreenToClient( this->getWindowHandle() , &pt);
	outRC->left = pt.x;
	outRC->top = pt.y;

	pt.x = outRC->right;
	pt.y = outRC->bottom;
	ScreenToClient( this->getWindowHandle() , &pt);
	outRC->right = pt.x;
	outRC->bottom = pt.y;
}

