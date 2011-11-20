// RMediaPlayer.h: RMediaPlayer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_)
#define AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "AtlActiveXBrige.h"
#include "RNotSupportException.h"
#include "RActiveXWindow.h"
#import "C:\WINNT\System32\msdxm.ocx" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")


class RMediaPlayer  :
 public RMediaViewPlayer ,	//�C���^�[�t�F�[�X
 public RActiveXWindow
{
public:
	RMediaPlayer();
	virtual ~RMediaPlayer();
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
	string getTitle() const;
	
private:

	CComPtr<MediaPlayer::IMediaPlayer2>	MediaPlayer;
};

#endif // !defined(AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_)
