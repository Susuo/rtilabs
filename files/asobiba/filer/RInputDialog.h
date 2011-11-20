// RInputDialog.h: RInputDialog �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_)
#define AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDoNotInitaliseException.h"
#include "RNotSupportException.h"
#include "IMyEnumString.h"
#include "RWin32Exception.h"
#include "RNoComplateException.h"

class RInputDialog  
{
public:
	RInputDialog();
	virtual ~RInputDialog();

	string DoModal(string inTitle , string inAutoCompleteName , string inDefualt) throw(RWin32Exception,RNoComplateException);
private:
	//�_�C�A���O�쐬�̂Ƃ��ɌĂ΂�܂��B
	bool OnInitDialog();
	//�R���v���[�g�f�[�^�̓ǂݍ���.
	WStringList LoadComplete(string inName);
	//�R���v���[�g�f�[�^�̏�������
	void SaveComplete(string inName , WStringList inTargetString);
	//OK�������ꂽ�Ƃ��̏���.
	void OnOK();
	//�L�����Z���������ꂽ�Ƃ��̏���.
	void OnCancel();

	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_INITDIALOG)
		{
			::SetWindowLong( hwnd , GWL_USERDATA,  (long)lparam);
			((RInputDialog*)(lparam))->WindowHandle = hwnd;
		}
		RInputDialog* This = (RInputDialog*)GetWindowLong( hwnd , GWL_USERDATA);
		if (This != NULL ) return This->DlgProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��.
	LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	IAutoComplete*				AutoComplete;
	IMyEnumString*				EnumStr;
	string						InputData;

	string						Title;
	string						AutoCompleteName;
	string						Defualt;

	HWND		WindowHandle;
};

#endif // !defined(AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_)
