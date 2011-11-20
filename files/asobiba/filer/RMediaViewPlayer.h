// RMediaViewPlayer.h: RMediaViewPlayer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_)
#define AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNormalWindow.h"
#include "RProgressBar.h"

//interface �N���X.
class RMediaViewPlayer		//���o�G�t�F�N�g��\������E�B���h�E.
{
public:
	RMediaViewPlayer()
	{
	}
	virtual ~RMediaViewPlayer()
	{
	}

	//���Ԃ̍쐬
	virtual void Create(HINSTANCE inINstance,HWND isParentWindow) throw() = 0;

	//�t�@�C�����J��
	virtual void Open(const string inFilename )	throw() = 0;

	//�Đ�����.
	virtual void Play() throw() = 0;

	//��~����.
	virtual void Stop() throw() = 0;

	//�ꎞ��~ / ����
	virtual void Pause(bool inON)	throw() = 0;

	//�t�@�C���N���[�Y
	virtual void Close()	throw() = 0;

	//�{�����[��
	virtual int getVolum() const throw() = 0;

	virtual void setVolum(int inVolum) throw() = 0;

	//�Đ��ʒu
	virtual int  getPos() const throw() = 0;

	virtual void setPos(int inPos) throw() = 0;

	//�Đ���?
	virtual bool IsPlay() const = 0;

	//�ꎞ��~��?
	virtual bool IsPause() const = 0;

	virtual const RWindow*	getWindow() const = 0;

	//�^�C�g���̎擾
	virtual string getTitle() const = 0;

};

#endif // !defined(AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_)
