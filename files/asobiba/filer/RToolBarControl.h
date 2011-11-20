// RToolBarControl.h: RToolBarControl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_)
#define AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "comm.h"

#include "RWindow.h"

class RToolBarControl  : public RWindow
{
public:
	RToolBarControl();
	virtual ~RToolBarControl();

	//�쐬
	void Create(HINSTANCE inInstance,
					HWND isParentWindow,int inID, DWORD inFlg) throw();

	//�{�^���̍쐬
	void CreateButton(HINSTANCE inInstance,
				const char* inBitmap,TBBUTTON* inButton,int inButtonCount);

	//�g�����ڂ̃Z�b�g.
	void setExtended(DWORD inStyle);
	//�g�����ڂ̎擾.
	DWORD getExtended();

};

#endif // !defined(AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_)
