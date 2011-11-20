// WindowsMain.h: WindowsMain �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
#define AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "BBanana.h"
#include "RMessageBox.h"

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
	//��ʂ̍X�V.
	static void Update();
	//���s.
	int   Run()  throw(RException);

	//���C���E�B���h�E�n���h���̎擾
	HWND getWindowHandle() const 
	{
		return BananaDlg.getWindowHandle();
	}
	//���C���C���X�^���X�n���h���̎擾
	HINSTANCE getInstance() const 
	{
		return Instance;
	}
	
	//�K����ID��U��o��(w
	static int getID() ;
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
	BBanana		BananaDlg;
	//�C���X�^���X�n���h��.
	HINSTANCE	Instance;
};

#endif // !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
