// RWindowBase.h: RWindowBase �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_)
#define AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RWindowBase  
{
public:
	RWindowBase();
	virtual ~RWindowBase();

	void MountWindow(HWND inWindow)
	{
		ASSERT(inWindow != NULL);
		WindowHandle = inWindow;
	}

	//�E�B���h�E�n���h���̎擾.
	HWND		getWindowHandle() const 
	{
		return WindowHandle;
	}
	//ID�̎擾.
	int			getID()
	{
		return ID;
	}

	//�ʒu�����ړ�.
	void MoveWindow(int x,int y) const;
	//���ƍ��������ړ�.
	void ResizeWindow(int width,int height) const;
	//�ړ�.
	void MoveWindow(int x,int y,int width,int height) const;
	//�ő�\��
	void FullWindow(HWND inParentWidowHandle) const;
	//�\������
	virtual void Show() const;
	//��\��
	virtual void Hide() const;
	//�X�^�C���̃Z�b�g.
	void setStyle(DWORD inStyle);
	//�X�^�C���̎擾.
	DWORD getStyle();

	void EnableWindow(bool inEnable);

	//ClientRect �̎擾.
	void GetClientRect(RECT * outRC) const;
	//WindowRect �̎擾.
	void GetWindowRect(RECT * outRC) const;

	//�w�肵���R���g���[���� �����̃E�B���h�E���W��̂ǂ��ɂ��邩?
	void getControlClientRect(HWND inTarget , RECT * outRC) const;

	//SendMessage
	LRESULT SendMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const;
	//PostMessage
	LRESULT PostMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const;


	string getText() const;
	void setText(const string & inStr);

private:
	HWND			WindowHandle;
	int				ID;
};

#endif // !defined(AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_)
