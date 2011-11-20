// RTabBar.h: RTabBar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_)
#define AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTabBar;

#include "comm.h"
#include "FilerSub.h"
#include "Window.h"

class RTabBar  
{
public:
	RTabBar(FilerSub* inFilerSub);
	virtual ~RTabBar();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize() = 0;
	//�^�u�̒ǉ�
	virtual void InertTab(const Window* inWindow) = 0;
	//�^�u�̍폜
	virtual void DeleteTab(const Window* inWindow) = 0;
	//�^�u�̕ύX
	virtual void UpdateTab(const Window* inWindow) = 0;

protected:
	FilerSub*	FSub;

};

#endif // !defined(AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_)
