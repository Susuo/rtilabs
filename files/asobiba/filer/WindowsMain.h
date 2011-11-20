// WindowsMain.h: WindowsMain �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
#define AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



#include "Filer.h"
#include "DisplayWin32.h"
#include "RDoNotInitaliseException.h"
#include "RSpliter.h"
#include "BasePanel.h"

//�f�U�C��
#define TOOL_BAR_START	0
#define TOOL_BAR_SIZE	28
#define ADDRESS_BAR_START	(TOOL_BAR_SIZE)
#define ADDRESS_BAR_SIZE	20
#define TAB_BAR_START	(ADDRESS_BAR_START + ADDRESS_BAR_SIZE)
#define TAB_BAR_SIZE	20
#define STATUS_BAR_SIZE	20
#define ITEM_PANEL_SIZE	(TOOL_BAR_SIZE + ADDRESS_BAR_SIZE + TAB_BAR_SIZE)

class WindowsMain  
{
private:
	//singleton �̂��ߊO�����쐬�ł��Ȃ��悤�ɂ���
	WindowsMain();
public:
	virtual ~WindowsMain();
public:
	//���C���V�X�e���̎擾 singleton
	static WindowsMain*	getMainSystem();
	//���s.
	int   Run() throw();

	//���C���E�B���h�E�n���h���̎擾
	HWND getWindowHandle() const 
	{
		return WindowHandle;
	}
	//���C���C���X�^���X�n���h���̎擾
	HINSTANCE getInstance() const 
	{
		return Instance;
	}
	
	//BaseWindow�n���h���̎擾.
	HWND getBaseWindow(bool isLeft) const
	{
		if (isLeft)	return Panel[0].getWindowHandle();
		else		return Panel[1].getWindowHandle();
	}

	//WndProc�̏����̉������ɓo�^����.
	void registeCallbackWndProc(bool isLeft ,WndProcCallable * inProc)
	{
		if (isLeft)		Panel[0].registeCallbackWndProc(inProc);
		else			Panel[1].registeCallbackWndProc(inProc);
	}
	//WndProc�̏����̉������̉���
	void UnregisteCallbackWndProc(bool isLeft ,WndProcCallable * inProc)
	{
		if (isLeft)		Panel[0].UnregisteCallbackWndProc(inProc);
		else			Panel[1].UnregisteCallbackWndProc(inProc);
	}

	//�K����ID��U��o��(w
	static int getID() ;

private:
	//�T�C�Y�ύX
	void OnSize();

private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		return getMainSystem()->WndProc(hwnd,msg,wparam,lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


private:
	//���C���E�B���h�E�̃n���h��.
    HWND		WindowHandle;
	//�C���X�^���X�n���h��.
	HINSTANCE	Instance;
	//�x�[�X�p�l�� ���E�Ɉꖇ�Â��܂��B
	//���̏�ɃA�h���X�o�[�⃊�X�g�r���[��z�u���܂��B
	BasePanel	Panel[2];

	//���E����
	RSpliter	Spliter;
};

#endif // !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
