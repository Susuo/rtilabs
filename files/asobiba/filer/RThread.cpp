// RThread.cpp: RThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RThread::RThread()
{
	//�X���b�h�̃n���h��.
	this->ThreadHandle = NULL;
	//�X���b�hID.
	this->ThreadNo = 0;

	//Wait�Ɏg���C�x���g�I�u�W�F�N�g.
	this->WaitEvent = CreateEvent(NULL,TRUE,FALSE, NULL);
	this->HoldUpEvent = CreateEvent(NULL,TRUE,FALSE, NULL);
}

RThread::~RThread()
{
	this->Stop();

	CloseHandle(this->HoldUpEvent);
	CloseHandle(this->WaitEvent);
}

//�X���b�h���X�^�[�g���܂��B
void	RThread::Start() throw()
{
	if ( this->IsAlive() ) throw("���łɃX���b�h�͍쐬����Ă��܂�");

	AllResetEvent();

	this->ThreadHandle = (HANDLE)_beginthreadex(NULL , 0 , _Start , (void*) this ,  0 , &this->ThreadNo);
	if ( this->ThreadHandle == NULL) throw("���łɃX���b�h�Ɏ��s���܂���");
}

//�X���b�h��������~�����܂��B
//�댯�A�f�l�ɂ͂����߂ł��Ȃ��B
//�܂��A���O��f�l�� HoldUp �� Join ���ĂȂ������Ă��ƁB
void	RThread::Stop()
{
	if (this->IsAlive() )
		_endthreadex( (unsigned int) this->ThreadHandle );
}


//��~�𖽂���
//�����͂����ɋA���Ă��܂��B
//��~�t���O�𗧂Ă܂��B
void	RThread::HoldUp()
{
	SetEvent( this->HoldUpEvent  );

	Notify();
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


//Notify���Ă΂��܂ő҂�.
//�X���b�h���g���Ăяo���܂��B
void	RThread::Wait()
{
	ResetEvent( this->WaitEvent  );
	::WaitForSingleObject( this->WaitEvent  , INFINITE);
}

//Wait��Ԃ���̒E�o.
//�X���b�h�ȊO���Ăяo���܂��B
void	RThread::Notify()
{
	SetEvent( this->WaitEvent  );
}

//���ׂẴC�x���g���N���A���܂��B
void	RThread::AllResetEvent()
{
	ResetEvent( this->WaitEvent  );
	ResetEvent( this->HoldUpEvent  );
}


//���̃X���b�h���L�����ǂ������ׂ�.
bool	RThread::IsAlive()
{
	return	::WaitForSingleObject( this->ThreadHandle  , 0) == WAIT_TIMEOUT;	
}

//Wait ��Ԃ��ǂ������ׂ�.
bool	RThread::IsWait()
{
	return	::WaitForSingleObject( this->WaitEvent  , 0) == WAIT_TIMEOUT;	
}

//HoldUp ��Ԃ��ǂ������ׂ�.
bool	RThread::IsHoldUp()
{
	return	::WaitForSingleObject( this->HoldUpEvent  , 0) == WAIT_TIMEOUT;	
}

//�X���b�h�R�[���o�b�N.
unsigned int RThread::_Start(void* inThis)
{
	return ((RThread*)inThis)->Run();
}

