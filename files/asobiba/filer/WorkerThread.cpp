// WorkerThread.cpp: WorkerThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WorkerThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

WorkerThread::WorkerThread(Channel* inChannel) : RThread()	
{
	this->OwnerChannel = inChannel;
}

WorkerThread::~WorkerThread()
{

}

unsigned int WorkerThread::Run()
{
	while( this->IsHoldUp() )
	{
		//���̃^�X�N�v���[�Y
		Request* req = OwnerChannel->takeRequest();
		if (req)
		{	//���s
			req->Execute();
		}
		else
		{	//���s������̂Ȃ��̂ő҂�
			this->Wait();
		}
	}
	return 0;
}

