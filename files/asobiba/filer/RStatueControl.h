// RStatueControl.h: RStatueControl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTATUECONTROL_H__E08C71A1_B5F1_4993_B38E_EDD05A5E016F__INCLUDED_)
#define AFX_RSTATUECONTROL_H__E08C71A1_B5F1_4993_B38E_EDD05A5E016F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"

class RStatueControl : public RWindow  
{
public:
	RStatueControl();
	virtual ~RStatueControl();

	//�쐬.
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
};

#endif // !defined(AFX_RSTATUECONTROL_H__E08C71A1_B5F1_4993_B38E_EDD05A5E016F__INCLUDED_)
