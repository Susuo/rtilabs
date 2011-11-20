// RThread.cpp: RThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RThread.h"
#include "RExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RThread::RThread()
{
	//�X���b�h�̃n���h��.
	this->ThreadHandle = NULL;
	//�X���b�hID.
	this->ThreadNo = 0;

	//�I���t���O
	this->HoldUpEvent = CreateEvent(NULL,TRUE,FALSE, NULL);
}

RThread::~RThread()
{
	this->Stop();

	CloseHandle(this->HoldUpEvent);
}

//�X���b�h���X�^�[�g���܂��B
void	RThread::Start() throw(RException)
{
	if ( this->IsAlive() )
	{
		throw RException(EXCEPTIONTRACE + "���łɃX���b�h�͍쐬����Ă��܂�");
	}

	AllResetEvent();

#ifndef _WIN32_WCE
	this->ThreadHandle = (HANDLE)_beginthreadex(NULL , 0 , _Start , (void*) this ,  0 , &this->ThreadNo);
#else
	this->ThreadHandle = CreateThread(NULL , 0 , _Start , (void*) this ,0 , &this->ThreadNo);
#endif
	if ( this->ThreadHandle == NULL)
	{
		DWORD error = ::GetLastError();
		throw RException(EXCEPTIONTRACE + "���łɃX���b�h�Ɏ��s���܂���" + RExceptionUtil::WindowsCodeToString(error) );
	}
}

//�X���b�h��������~�����܂��B
//�댯�A�f�l�ɂ͂����߂ł��Ȃ��B
//�܂��A���O��f�l�� HoldUp �� Join ���ĂȂ������Ă��ƁB
void	RThread::Stop()
{
	if (this->IsAlive() )
	{
#ifndef _WIN32_WCE
		_endthreadex( (unsigned int) this->ThreadHandle );
#else
		TerminateThread(this->ThreadHandle , 0);
#endif
	}
	CloseHandle(this->ThreadHandle);
}


//��~�𖽂���
//�����͂����ɋA���Ă��܂��B
//��~�t���O�𗧂Ă܂��B
void	RThread::HoldUp()
{
	SetEvent( this->HoldUpEvent  );
}


//�X���b�h����~����܂ő҂B
//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
void	RThread::Join()
{
	::WaitForSingleObject( this->ThreadHandle  , INFINITE);
}


//�X���b�h����~����܂ł��A���Ԃ��o�߂���܂ő҂B
//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
//���Ԃ��o�߂��Ĕ������ꍇ�� false .
bool	RThread::Join(DWORD inWaitSecondTime)
{
	return	::WaitForSingleObject
				( this->ThreadHandle  , inWaitSecondTime) != WAIT_TIMEOUT;
}


//���ׂẴC�x���g���N���A���܂��B
void	RThread::AllResetEvent()
{
	ResetEvent( this->HoldUpEvent  );
}


//���̃X���b�h���L�����ǂ������ׂ�.
bool	RThread::IsAlive() const
{
	return	::WaitForSingleObject( this->ThreadHandle  , 0) == WAIT_TIMEOUT;	
}

//HoldUp ��Ԃ��ǂ������ׂ�.
bool	RThread::IsHoldUp() const
{
	return	::WaitForSingleObject( this->HoldUpEvent  , 0) == WAIT_TIMEOUT;	
}

#ifndef _WIN32_WCE
	//�X���b�h�R�[���o�b�N.
	unsigned int RThread::_Start(void* inThis)
	{
		return ((RThread*)inThis)->Run();
	}
#else
	//�X���b�h�R�[���o�b�N.
	unsigned long RThread::_Start(void* inThis)
	{
		return ((RThread*)inThis)->Run();
	}
#endif

//�X���[�v �w�莞�Ԃ����A�I������������܂� sleep ����
void RThread::Sleep(DWORD inWaitSecondTime)
{
	::WaitForSingleObject( this->HoldUpEvent  , inWaitSecondTime) ;	
}
