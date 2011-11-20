// RImageButton.h: RImageButton �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_)
#define AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"
#include "RImage.h"

class RImageButton : public RWindow  
{
public:
	RImageButton();
	virtual ~RImageButton();
public:
	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�C���[�W�̃Z�b�g
	bool CreateImage(const string inFilename , bool isRegion);
	//�G�̑傫���ɃE�B���h�E�����킹��.
	void FitPictureTo();
private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		RImageButton* p = (RImageButton*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//�T�u�N���X��.
	WNDPROC			OrignalWindowProc;

	RImage			LoadImage;
};

#endif // !defined(AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_)
