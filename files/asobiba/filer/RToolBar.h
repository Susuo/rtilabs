// RToolBar.h: RToolBar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTOOLBAR_H__99BAAD3F_B4F8_456C_A3FE_90B7D430FBC8__INCLUDED_)
#define AFX_RTOOLBAR_H__99BAAD3F_B4F8_456C_A3FE_90B7D430FBC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RToolBar  ;

#include "comm.h"
#include "FilerSub.h"

class RToolBar  
{
public:
	RToolBar(FilerSub* inFilerSub);
	virtual ~RToolBar();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize() = 0;
protected:
	FilerSub* FSub;

};

#endif // !defined(AFX_RTOOLBAR_H__99BAAD3F_B4F8_456C_A3FE_90B7D430FBC8__INCLUDED_)
