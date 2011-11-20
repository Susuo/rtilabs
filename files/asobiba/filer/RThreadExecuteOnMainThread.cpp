// RThreadExecuteOnMainThread.cpp: RThreadExecuteOnMainThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RThreadExecuteOnMainThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RThreadExecuteOnMainThread::RThreadExecuteOnMainThread()
{
	Alive.ResetEvent();
	AllResetEvent();
}

RThreadExecuteOnMainThread::~RThreadExecuteOnMainThread()
{
	Join();
}

//���C���ɏ������ϑ����܂��B
void	RThreadExecuteOnMainThread::Start(HWND inMainWindow)
{
	ASSERT(inMainWindow != NULL);

	if ( Alive.Pooling() )
	{
		return ;	//���łɋN�����Ă��܂��B
	}
	Alive.SetEvent();
	AllResetEvent();

	::PostMessage(inMainWindow , RTI_WM_INTERRUPT , (WPARAM)this , (LPARAM)0);
}

//�R�[���o�b�N.
void  RThreadExecuteOnMainThread::_Start()
{
	ASSERT(	Alive.Pooling() );

	this->Run();

	Alive.ResetEvent();
}


//��~�𖽂���
//�����͂����ɋA���Ă��܂��B
//��~�t���O�𗧂Ă܂��B
void	RThreadExecuteOnMainThread::HoldUp()
{
	HoldUpEvent.SetEvent();
	Notify();
}

//�X���b�h����~����܂ő҂B
//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
void	RThreadExecuteOnMainThread::Join()
{
	Alive.WaitForSignal(INFINITE);
}

//�X���b�h����~����܂ł��A���Ԃ��o�߂���܂ő҂B
//�Ȃ��A�������g��������ĂԂƃf�b�h���b�N���܂�(w
//���Ԃ��o�߂��Ĕ������ꍇ�� false .
bool	RThreadExecuteOnMainThread::Join(DWORD inWaitSecondTime)
{
	return Alive.WaitForSignal(inWaitSecondTime);
}

//Notify���Ă΂��܂ő҂�.
//�X���b�h���g���Ăяo���܂��B
void	RThreadExecuteOnMainThread::Wait()
{
	WaitEvent.ResetEvent();
	WaitEvent.WaitForSignal(INFINITE);
}

//Wait��Ԃ���̒E�o.
//�X���b�h�ȊO���Ăяo���܂��B
void	RThreadExecuteOnMainThread::Notify()
{
	WaitEvent.SetEvent();
}

//���ׂẴC�x���g���N���A���܂��B
void	RThreadExecuteOnMainThread::AllResetEvent()
{
	WaitEvent.ResetEvent();
	HoldUpEvent.ResetEvent();
}


//���̃X���b�h���L�����ǂ������ׂ�.
bool	RThreadExecuteOnMainThread::IsAlive()
{
	return Alive.Pooling();
}

//Wait ��Ԃ��ǂ������ׂ�.
bool	RThreadExecuteOnMainThread::IsWait()
{
	return WaitEvent.Pooling();
}

//HoldUp ��Ԃ��ǂ������ׂ�.
bool	RThreadExecuteOnMainThread::IsHoldUp()
{
	return HoldUpEvent.Pooling();
}

