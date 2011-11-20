// hannel.cpp: Channel �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Channel.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


Channel::Channel()
{
	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i] = new WorkerThread(this);
		Thread[i]->Start();
	}
}

Channel::~Channel()
{
	//��~���ߔ��s
	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->HoldUp();
	}
	//��~�܂ő҂��č폜.
	for(i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->Join();
		delete Thread[i];
	}
}

//�^�X�N���Z�b�g����
void Channel::putRequest(Request* inRequest)
{
	RAutoLock al(&Lock);
	RequestManage.push_back( inRequest );

	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->Notify();
	}
}

//�^�X�N���擾����
Request* Channel::takeRequest()
{
	RAutoLock al(&Lock);

	if (RequestManage.size() <= 0) return NULL;

	Request* ret = RequestManage.back();
	RequestManage.pop_back();

	return ret;
}
