// RListViewWithMouseGesture.h: RListViewWithMouseGesture �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_)
#define AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RListView.h"
#include "RMouseGestureMouseCapture.h"	//�}�E�X�W�F�X�`���[ �}�E�X�̓�����ϑ��A�܂�}�E�X�̓�����`�F�L����̃f�X!

class RListViewWithMouseGesture : public RListView  
{
public:
	RListViewWithMouseGesture();
	virtual ~RListViewWithMouseGesture();

	//�쐬.	�I�[�o�[���C�h
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		RListViewWithMouseGesture* p = (RListViewWithMouseGesture*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//�T�u�N���X��.
	WNDPROC			OrignalWindowProc;
private:
	//�}�E�X�W�F�X�`���[ �}�E�X�̓�����ϑ��A�܂�}�E�X�̓�����`�F�L����̃f�X!
	RMouseGestureMouseCapture	MouseGestureMouseCapture;
};

#endif // !defined(AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_)
