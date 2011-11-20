// RDialog.cpp: RDialog �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDialog.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RDialog::RDialog()
{

}

RDialog::~RDialog()
{
	if (this->getWindowHandle() != NULL)
		SetWindowLong(this->getWindowHandle() ,  GWL_USERDATA , NULL);
}

void RDialog::Create(HINSTANCE inInstance , HWND inParentWindow , const char* inResourceName) throw(RWin32Exception)
{
#ifndef _WIN32_WCE
	HWND handle = ::CreateDialogParam(inInstance , inResourceName , inParentWindow , (DLGPROC)DlgProcStatic , (LPARAM)this);
#else
	MY_USES_CONVERSION;
	HWND handle = ::CreateDialogParam(inInstance , _A2W(inResourceName) , inParentWindow , (DLGPROC)DlgProcStatic, (LPARAM)this);
#endif
	if (handle == NULL)
	{
		throw RWin32Exception("�_�C�A���O�̍쐬�Ɏ��s");
  	}

}

int RDialog::DoModel(HINSTANCE inInstance , HWND inParentWindow , const char* inResourceName) throw(RWin32Exception)
{
#ifndef _WIN32_WCE
	int ret = ::DialogBoxParam(inInstance , inResourceName , inParentWindow , (DLGPROC)DlgProcStatic , (LPARAM)this);
#else
	MY_USES_CONVERSION;
	int ret = ::DialogBoxParam(inInstance , _A2W(inResourceName) , inParentWindow , (DLGPROC)DlgProcStatic, (LPARAM)this);
#endif
	if (ret == -1)
	{
		throw RWin32Exception("�_�C�A���O�̍쐬�Ɏ��s");
  	}

	return ret;
}

void RDialog::EndDialog(int inExitCode)
{
	::EndDialog(this->getWindowHandle() ,inExitCode );
}

HWND RDialog::GetDlgItem(int inID)
{
	ASSERT( ::GetDlgItem(this->getWindowHandle() , inID) != NULL);
	return ::GetDlgItem(this->getWindowHandle() , inID);
}

