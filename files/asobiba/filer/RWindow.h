// RWindow.h: RWindow �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_)
#define AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RWindow  ;

#include "comm.h"
#include "RDoNotInitaliseException.h"

class RWindow  
{
public:
	RWindow();
	virtual ~RWindow();

	void MountWindow(HWND inWindow)
	{
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

	//ClientRect �̎擾.
	void GetClientRect(RECT * outRC) const;
	//WindowRect �̎擾.
	void GetWindowRect(RECT * outRC) const;


protected:
	//�쐬
	void Create(HINSTANCE inINstance, const char* inClassName,
					HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw();

private:
	HWND			WindowHandle;
	int				ID;
};

#endif // !defined(AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_)
