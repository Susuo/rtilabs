// RTabBarWin32.h: RTabBarWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_)
#define AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTabBarWin32;

#include "comm.h"
#include "RTabBar.h"
#include "RTabControl.h"
#include "RTableControl.h"

class RTabBarWin32 : public RTabBar  
{
public:
	RTabBarWin32(FilerSub* inFilerSub) throw();
	virtual ~RTabBarWin32();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize();
	//�^�u�̒ǉ�
	virtual void InertTab(const Window* inWindow) ;
	//�^�u�̍폜
	virtual void DeleteTab(const Window* inWindow) ;
	//�^�u�̕ύX
	virtual void UpdateTab(const Window* inWindow) ;


	//�E�B���h�E�v���V�[�W��.
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	//�I������Ă���^�u���ω����܂����B
	void OnSelChange();
	//�E�N���b�N
	bool OnRClick(const NMHDR * inNmhdr) const;

	//�J�����g�̃E�B���h�E�̎擾.
	Window* getCurrentWindow() const;
private:
	RTabControl		TabControl;
	RTableControl<RTabBarWin32>	Table;
};

#endif // !defined(AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_)
