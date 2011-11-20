// RWindow.cpp: RWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RWindow::RWindow()
{
}

RWindow::~RWindow()
{
}

//�쐬
void RWindow::Create(HINSTANCE inINstance, const char* inClassName,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw(RWin32Exception)
{
	HWND handle;
#ifndef _WIN32_WCE
	handle = ::CreateWindowEx(inFlgEx, inClassName, "",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, (LPVOID )this);
#else
	MY_USES_CONVERSION;
	handle = ::CreateWindowEx(inFlgEx, _A2W(inClassName), L"",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, (LPVOID )this);
#endif
	if (handle == NULL) throw RWin32Exception("�������Ɏ��s���܂���");

	MountWindow(handle);
}

