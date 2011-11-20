// RRealPlayer.h: RRealPlayer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_)
#define AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "AtlActiveXBrige.h"
#include "RNotSupportException.h"
#include "RActiveXWindow.h"

#import "C:\WINNT\System32\rmoc3260.dll" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")

class RRealPlayer : 
 public RMediaViewPlayer ,	//�C���^�[�t�F�[�X
 public RActiveXWindow  
{
public:
	RRealPlayer();
	virtual ~RRealPlayer();

	//���Ԃ̍쐬
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//�t�@�C�����J��
	virtual void Open(const string inFilename )	throw() ;

	//�Đ�����.
	virtual void Play() throw() ;

	//��~����.
	virtual void Stop() throw() ;

	//�ꎞ��~ / ����
	virtual void Pause(bool inON)	throw() ;

	//�t�@�C���N���[�Y
	virtual void Close()	throw() ;

	//�{�����[��
	virtual int getVolum() const throw() ;

	virtual void setVolum(int inVolum) throw() ;

	//�Đ��ʒu
	virtual int  getPos() const throw() ;

	virtual void setPos(int inPos) throw() ;

	//�Đ���?
	virtual bool IsPlay() const ;

	//�ꎞ��~��?
	virtual bool IsPause() const ;
	//�T�|�[�g���Ă܂�?
	static bool IsSupport(const string inFilename);

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}

	//�^�C�g���̎擾
	virtual string getTitle() const;

private:
	//�E�B���h�E�v���V�[�W��
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

private:
	CComPtr<RealAudioObjects::IRealAudio>	RealAudio;
};

#endif // !defined(AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_)
