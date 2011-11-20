// RWorkerThread.h: RWorkerThread �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_)
#define AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RWorkerThread;

#include "RThread.h"
#include "RThreadPool.h"
#include "RRequest.h"

class RWorkerThread : public RThread  
{
public:
	RWorkerThread(RThreadPool<RWorkerThread>* inPool);
	virtual ~RWorkerThread();

	
	/**
	 * Run:		���s
	 *
	 * @return virtual unsigned int			��� 0
	 */
	virtual unsigned int Run();
	/**
	 * Bind:		�d����o�^����.
	 *
	 * @param inReq		�V�����d��
	 * @return void 
	 */
	void	Bind(RRequest* inReq)
	{
		Request = inReq;
	}

private:
	//�X���b�h�v�[���{��.
	RThreadPool<RWorkerThread>*	OwnerPool;
	//���N�G�X�g.
	RRequest*		Request;
};

#endif // !defined(AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_)
