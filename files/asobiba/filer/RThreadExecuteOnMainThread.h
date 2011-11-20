// RThreadExecuteOnMainThread.h: RThreadExecuteOnMainThread �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_)
#define AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RThreadExecuteOnMainThread ;
#include "comm.h"
#include "REvent.h"

//�C�x���g���荞��. 10/02 �́@�V�X�v�� repure �����J�n�L�O
#define RTI_WM_INTERRUPT	(WM_USER + 1002)

typedef void (RThreadExecuteOnMainThread::*MAIN_THREAD_CALLBACK)(void);

//���C���X���b�h�ɏ������ϑ����܂��B
class RThreadExecuteOnMainThread  
{
protected:
	RThreadExecuteOnMainThread();
	virtual ~RThreadExecuteOnMainThread();

	//�p�����ď����������̂ł���[
	virtual unsigned int Run() = 0;

public:
	//���C���ɏ������ϑ����܂��B
	void	Start(HWND inMainWindow);

	//��~�𖽂���
	//�����͂����ɋA���Ă��܂��B
	//��~�t���O�𗧂Ă܂��B
	void	HoldUp();

	//�X���b�h����~����܂ő҂B
	//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
	void	Join();

	//�X���b�h����~����܂ł��A���Ԃ��o�߂���܂ő҂B
	//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
	//���Ԃ��o�߂��Ĕ������ꍇ�� false .
	bool	Join(DWORD inWaitSecondTime);

	//Wait��Ԃ���̒E�o.
	//�X���b�h�ȊO���Ăяo���܂��B
	void	Notify();

	//���̃X���b�h���L�����ǂ������ׂ�.
	bool	IsAlive();

	//Wait ��Ԃ��ǂ������ׂ�.
	bool	IsWait();

	//HoldUp ��Ԃ��ǂ������ׂ�.
	bool	IsHoldUp();
protected:
	//Notify���Ă΂��܂ő҂�.
	//�X���b�h���g���Ăяo���܂��B
	void	Wait();

public:
	//�R�[���o�b�N.
	//���O�ŌĂԂ�
	void	_Start();

private:
	//���ׂẴC�x���g���N���A���܂��B
	void	AllResetEvent();

	//�L�����ǂ���.
	REvent		Alive;

	//Wait�Ɏg���C�x���g�I�u�W�F�N�g.
	REvent	WaitEvent;

	//HoldUp�Ɏg���C�x���g�I�u�W�F�N�g.
	REvent	HoldUpEvent;

};

#endif // !defined(AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_)
