// RFlashPlayer.h: RFlashPlayer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_)
#define AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RActiveXWindow.h"
#include "RMediaViewPlayer.h"
#include "RNotSupportException.h"
#include "AtlActiveXBrige.h"
#import "C:\WINNT\system32\Macromed\Flash\Flash.ocx" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")

class RFlashPlayer : 
	public RMediaViewPlayer, 
	public RActiveXWindow  
{
public:
	RFlashPlayer();
	virtual ~RFlashPlayer();
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
	virtual string getTitle() const ;

private:

	CComPtr<ShockwaveFlashObjects::IShockwaveFlash>	ShockwaveFlash;
};

#endif // !defined(AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_)
