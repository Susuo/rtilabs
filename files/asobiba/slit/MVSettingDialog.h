// MVSettingDialog.h: MVSettingDialog �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_)
#define AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RDialog.h"
#include "RComboBox.h"

class MVMain;

class MVSettingDialog : public RDialog  
{
public:
	MVSettingDialog();
	virtual ~MVSettingDialog();

	void Create(MVMain	*	poolMVMain) throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnCancel();
	void OnOK();

	//����O�̏�Ԃ̋L�^
	RComboBox			LastMemory;
	//�W���X�g�t�B�b�g
	RComboBox			JustFit;
	//�X�N���[���J�E���g
	RComboBox			ScrollCount;
	//�X�N���[���^�C�v
	RComboBox			ScrollType;

	MVMain *			PoolMVMain;
};

#include "MVMain.h"

#endif // !defined(AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_)
