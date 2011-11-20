// RThread.h: RThread �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
#define AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RThread  ;

#include "comm.h"
#include "RException.h"

//�X���b�h�N���X
//java �� �X���b�h�̗򉻃R�s�[�ł�(w
class RThread  
{
protected:
	RThread();
	virtual ~RThread();

	//�p�����ăX���b�h�̏����������̂ł���[
	virtual unsigned int Run() = 0;

public:
	//�X���b�h���X�^�[�g���܂��B
	//��O
	//RDoNotInitaliseException	�X���b�h���쐬�ł��Ȃ�
	void	Start() throw(RException);

	//�X���b�h��������~�����܂��B
	//�댯�A�f�l�ɂ͂����߂ł��Ȃ��B
	//�܂��A���O��f�l�� HoldUp �� Join ���ĂȂ������Ă��ƁB
	void	Stop();

	//��~�𖽂���
	//�����͂����ɋA���Ă��܂��B
	//��~�t���O�𗧂Ă܂��B
	virtual void	HoldUp();

	//�X���b�h����~����܂ő҂B
	//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
	void	Join();

	//�X���b�h����~����܂ł��A���Ԃ��o�߂���܂ő҂B
	//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
	//���Ԃ��o�߂��Ĕ������ꍇ�� false .
	bool	Join(DWORD inWaitSecondTime);

	//���̃X���b�h���L�����ǂ������ׂ�.
	bool	IsAlive() const;

	//HoldUp ��Ԃ��ǂ������ׂ�.
	bool	IsHoldUp() const;

	//�X���[�v �w�莞�Ԃ����A�I������������܂� sleep ����
	void 	Sleep(DWORD inWaitSecondTime);

protected:
	//Notify���Ă΂��܂ő҂�.
	//�X���b�h���g���Ăяo���܂��B
	void	Wait();

private:
	#ifndef _WIN32_WCE
		//�X���b�h�R�[���o�b�N.
		static	unsigned int __stdcall  _Start(void* inThis);
	#else
		//�X���b�h�R�[���o�b�N.
		static	unsigned long __stdcall  _Start(void* inThis);
	#endif

	//���ׂẴC�x���g���N���A���܂��B
	void	AllResetEvent();
private:
	//�X���b�h�̃n���h��.
	HANDLE	ThreadHandle;

	#ifndef _WIN32_WCE
		//�X���b�hID.
		unsigned int	ThreadNo;
	#else
		//�X���b�hID.
		unsigned long	ThreadNo;
	#endif

	//HoldUp�Ɏg���C�x���g�I�u�W�F�N�g.
	HANDLE	HoldUpEvent;
};

#endif // !defined(AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
