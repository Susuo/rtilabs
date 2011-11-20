// RFModView.h: RFModView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_)
#define AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "FModPlay.h"
#include "RMediaViewPlayer.h"
#include "RTimer.h"



class RFModView  :
 public RMediaViewPlayer ,	//�C���^�[�t�F�[�X
 public FModCallable, 		//�Đ����̃R�[���o�b�N.
 public RNormalWindow		//�ʏ�E�B���h�E
{
public:
	RFModView();
	virtual ~RFModView();

	//���Ԃ̍쐬
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//�t�@�C�����J��
	void Open(const string inFilename )	throw()
	{
		Player.Open(inFilename );
	}

	//�Đ�����.
	void Play() throw()
	{
		Player.Play(this);	//�������g�ɃR�[���o�b�N������.
	}

	//��~����.
	void Stop() throw()
	{
		Player.Stop();
	}

	//�ꎞ��~ / ����
	void Pause(bool inON)	throw()
	{
		Player.Pause(inON);
	}

	//�t�@�C���N���[�Y
	void Close()	throw()
	{
		Player.Close();
	}

	//�{�����[��
	int getVolum() const throw()
	{
		return CHANGE_SCALE(Player.getVolum() , 255 , 255);
	}

	void setVolum(int inVolum) throw()
	{
		ASSERT(inVolum <= 255);
		Player.setVolum( REV_CHANGE_SCALE(inVolum,255,255) );
	}

	//�Đ��ʒu
	int  getPos() const throw()
	{
		int a = Player.getPos() ;
		int b = Player.getLength();
		return CHANGE_SCALE( Player.getPos() , Player.getLength() , 255);
	}

	void setPos(int inPos) throw()
	{
		ASSERT(inPos <= 255);
		Player.setPos( REV_CHANGE_SCALE(inPos , Player.getLength() , 255) );
	}

	//�Đ���?
	bool IsPlay() const
	{
		return Player.IsPlay();
	}

	//�ꎞ��~��?
	bool IsPause() const
	{
		return Player.IsPause();
	}

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}

	//�T�|�[�g���Ă܂�?
	static bool IsSupport(const string inFilename);

	//�^�C�g���̎擾
	virtual string getTitle() const ;
protected:
	//�R�[���o�b�N
	virtual signed char FModCallBack(FSOUND_STREAM *stream, void *buff, int len) 
	{
		SeenProgressBar->setPos( getPos() );
		return 1;
	}

private:
	FModPlay	Player;
	RProgressBar*	SeenProgressBar;

	RTimer			Timer;
};

#endif // !defined(AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_)
