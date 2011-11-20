// FilerSub.h: FilerSub �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_)
#define AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FilerSub;

#include "comm.h"
#include "RAddressBar.h"
#include "RTabBar.h"
#include "RToolBar.h"
#include "RStatue.h"
#include "Window.h"


typedef deque<Window*> TAB_WINDOWS;

class FilerSub  
{
private:
	//�A�h���X�o�[
	RAddressBar*	AddressBar;
	//�^�u
	RTabBar*		TabBar;
	//�c�[���o�[
	RToolBar*		ToolBar;
	//�X�e�[�^�X�o�[
	RStatue*		StatueBar;
	//���ɂ����?
	bool			IsLeft;
	//�^�u�ŕ\������E�B���h�E
	TAB_WINDOWS		Wins;

	//���ݕ\������Ă���E�B���h�E
	Window*			ActiveWindow;
	//���݃^�u��őI������Ă���E�B���h�E.
	//�\������Ă��邾���ł͂Ȃ��āA�^�u�ŉE�N���b�N�����Ƃ��Ȃǂŕω�����
	Window*			TargetWindow;
public:
	FilerSub(bool inIsLeft);
	virtual ~FilerSub();

	//���ɂ����?
	bool getIsLeft() const
	{
		return IsLeft;
	}
	//�w�肵���E�B���h�E���A�N�e�B���ɂ���.
	//TAB����ĂԂ���!
	bool setActiveWindow(Window* inWin);
	//�w�肵���E�B���h�E���^�[�Q�b�g�ɂ���
	bool setTargetWindow(Window* inWin);

	//�A�N�e�B���E�B���h�E�ł���?
	bool IsActiveWindow(const Window* inWin);
	//�A�N�e�B����Window�̎擾.
	Window* getActiveWindow() const
	{
		return ActiveWindow;
	}

	//�^�[�Q�b�g�ɂȂ��Ă���E�B���h�E
	//���Ƃ��΁A�^�u�ŕ���^�[�Q�b�g�ɂȂ��Ă���E�B���h�E�ł��B
	//�����A�^�[�Q�b�g�����Ȃ��ꍇ�́A�A�N�e�B���ȃE�B���h�E��Ԃ��܂��B
	Window* getTargetWindow() const
	{
		return TargetWindow != NULL ? TargetWindow : ActiveWindow;
	}

	
	//�V�����E�B���h�E���J��
	Window* NewWindow(string inPath);
	//�E�B���h�E�����
	bool CloseWindow(Window* inTargetWindow);
	//����
	bool Search(const Window* inWin) const;
//�C�x���g
	//OnQuickChange �����������Ƃ��ɌĂ΂��
	void OnQuickChange();
	//�^�C�g�����ύX���ꂽ�ꍇ
	void OnTitleChange();
	//�E�B���h�E�����T�C�Y���ꂽ�Ƃ��ɌĂ΂��
	void OnWindowResize();

	//�X�e�[�^�X�o�[�̎擾.
	RStatue*	getStatue()
	{
		return StatueBar;
	}
private:
};

#endif // !defined(AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_)
