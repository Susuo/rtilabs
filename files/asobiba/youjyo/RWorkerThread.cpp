// RWorkerThread.cpp: RWorkerThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWorkerThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RWorkerThread::RWorkerThread(RThreadPool<RWorkerThread>* inPool)
{
	OwnerPool = inPool;
	Request = NULL;
}

RWorkerThread::~RWorkerThread()
{

}

/**
 * Run:		���s
 *
 * @return virtual unsigned int			��� 0
 */
unsigned int RWorkerThread::Run()
{
	while( this->IsHoldUp() )
	{
		if (Request)
		{	//���s
			Request->Execute(this);
			//���s���I������̂Ŕj��.
			delete Request;
			//�ꉞ�I��������ǁA�V�����d������?
			Request = OwnerPool->complateRequest(this);
		}
		else
		{	//���s������̂Ȃ��̂ő҂�
			this->Wait();
		}
	}
	return 0;
}

