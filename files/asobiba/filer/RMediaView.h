// RMediaView.h: RMediaView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_)
#define AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"
#include "RImageButton.h"
#include "RProgressBar.h"
#include "RMedia.h"
#include "RTimer.h"
#include "RSetupLoad.h"

class RMediaView : public RWindow  
{
public:
	RMediaView();
	virtual ~RMediaView();
public:
	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//���[�h
	bool Load(const string inFilename, HINSTANCE inINstance);
	//�^�C�g�����擾����
	string getTitle() const ;

private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		RMediaView* p = (RMediaView*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//�{�^��.
	RImageButton	Start;
	RImageButton	Stop;
	RImageButton	FastForward;
	RImageButton	FastRewind;
	RImageButton	Forward;
	RImageButton	Rewind;

	RProgressBar	Seen;
	RProgressBar	Volum;
	//�Đ�������.
	RMedia			Media;

	RTimer			Timer;

	//�ݒ�̃��[�h
	RSetupLoad		SetupLoad;
};

#endif // !defined(AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_)
