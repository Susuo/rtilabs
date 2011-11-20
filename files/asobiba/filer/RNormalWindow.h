// RNormalWindow.h: RNormalWindow �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_)
#define AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"

class RNormalWindow : public RWindow  
{
public:
	RNormalWindow();
	virtual ~RNormalWindow();
protected:
	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw();
protected:
	//�E�B���h�E�v���V�[�W��(none static)
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		RNormalWindow* p = (RNormalWindow*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

};

#endif // !defined(AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_)
