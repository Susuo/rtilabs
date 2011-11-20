// RTabControl.h: RTabControl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_)
#define AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"


class RTabControl  : public RWindow
{
public:
	RTabControl();
	virtual ~RTabControl();

	//�쐬.
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�^�u�����ׂč폜����.
	bool ClearTab();
	//�^�u�̒ǉ�.
	bool InsertTab(int inItem,const char* inText,LPARAM inLPARAM);
	//�^�u�̕ύX.
	bool UpdateTab(int inItem,const char* inText,LPARAM inLPARAM);
	//�^�u�̎擾
	LPARAM GetTabParam(int inItem) const;
	//�^�u�̍폜
	bool DeleteTab(int inItem);
	//�^�u�̐��̎擾
	int GetTabCount() const;
	//�w�肵���^�u��I��.
	void SelectTab(int inItem);

	//���ݑI������Ă���^�u�̈ʒu���擾.
	int GetCurSel() const;

};

#endif // !defined(AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_)
