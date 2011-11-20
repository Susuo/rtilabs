// RThreadPool.h: RThreadPool �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_)
#define AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
template<class _USER_WORKER_THREAD>
class RThreadPool;




//#include "RWorkerThread.h"
//#define _USER_WORKER_THREAD	RWorkerThread
#include "YThreadWithTls.h"

#include "RRequest.h"
#include "RLock.h"

#ifdef _DEBUG
	//�v�[�����Ă����X���b�h�̐�.
	const int MAX_THREAD	= 10;
#else
	//�v�[�����Ă����X���b�h�̐�.(���񂽁[�Ղ炢��)
	const int MAX_THREAD	= 50;
#endif

/**
 * class RThreadPool:	�X���b�h�v�[��
 *
 * @author		rti
 */
template<class _USER_WORKER_THREAD>
class RThreadPool  
{
public:
	RThreadPool()
	{
			for(int i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i] = new _USER_WORKER_THREAD(this);
				Threads[i]->Start();

				//�����Ă���X���b�h�Ƃ������ƂłԂ����݂܂��B
				WaitThreads.push_back(Threads[i]);
			}
	}
	virtual ~RThreadPool()
	{
			//��~���ߔ��s
			for(int i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i]->HoldUp();
			}
			//��~�܂ő҂��č폜.
			for(i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i]->Join();
				delete Threads[i];
			}
	}

	/**
	 * putRequest:	���N�G�X�g�𑗂�.
	 *
	 * @param inRequest		���N�G�X�g
	 * @return void 
	 */
	void putRequest(RRequest* inRequest)
	{
			//���̃��[�`���̓N���e�B�J���Z�N�V�����ŕی삳��Ă��܂��B
			RAutoLock al(&Lock);

			//�����Ă���X���b�h����܂���?
			if (WaitThreads.size() == 0)
			{
				//�Ȃ������.
				//���N�G�X�g�L���[�ɒǉ�
				RequestQeue.push_back(inRequest);
				return ;
			}
			//�����Ă���X���b�h���ڂ��.
			_USER_WORKER_THREAD* thread = WaitThreads.front();

			//�d��������U��.
			thread->Bind(inRequest);
			//�ڊo�߂�!
			thread->Notify();

			WaitThreads.pop_front();
			//�d�����Ă���L���[�ɓ����.
//			WorkThreads.push_back(thread);
	}
	/**
	 * ComplateRequest:	���N�G�X�g�����̂��m�点
	 *
	 * @param inThread		�d���������X���b�h(���̃X���b�h���g�����̃��[�`�����Ăт܂�)
	 * @return RRequest*	�����ɐV�������N�G�X�g������ꍇ�� Request* �Ŏd�����Ȃ���� NULL
	 */
	RRequest* complateRequest(_USER_WORKER_THREAD* inThread)
	{
			//���̃��[�`���̓N���e�B�J���Z�N�V�����ŕی삳��Ă��܂��B
			RAutoLock al(&Lock);

			//���͒��Z����?
			if (RequestQeue.size() != 0)
			{
				//�ҋ@��Ԃ̃��N�G�X�g������̂ň��������������܂�.
				RRequest*  req = RequestQeue.front();
				RequestQeue.pop_front();
				return req;
			}

			//�d�������Ă���X���b�h��莩�����폜.
//			WORKER_THREADS_POINTER::iterator i = find(WorkThreads.begin() , WorkThreads.end() , inThread);
//			ASSERT(i != WorkThreads.end());
//			WorkThreads.erase(i);

			//�ҋ@��Ԃ̂Ƃ���ɓ�����...
			WaitThreads.push_back(inThread);

			//���΂��̖���ɂ��܂��B�@�΂��ԁ[.
			return NULL;
	}

	void Dump()
	{
		//���̃��[�`���̓N���e�B�J���Z�N�V�����ŕی삳��Ă��܂��B
		RAutoLock al(&Lock);

		puts("========================== ThreadPool Dump ============================");
		printf("�ҋ@��ԃX���b�h %d\r\n" , WaitThreads.size() );
		printf("�d�����̃X���b�h %d\r\n" , WorkThreads.size() );
		printf("���������N�G�X�g %d\r\n" , RequestQeue.size() );
		puts("========================== =============== ============================");
	}

private:

	//�N���e�B�J���Z�N�V����
	RLock						Lock;

	//�X���b�h���U��̎��ɂ͂����Ƀ��N�G�X�g���~�����܂��B
	typedef deque<RRequest*>	REQUESTQEUE;
	REQUESTQEUE					RequestQeue;

	//���ꂾ���̃X���b�h���v�[�����Ă����܂��B
	_USER_WORKER_THREAD*	Threads[MAX_THREAD];

	typedef deque<_USER_WORKER_THREAD*>	WORKER_THREADS_POINTER;
	//�ҋ@��Ԃ̃X���b�h
	WORKER_THREADS_POINTER	WaitThreads;
	//�d�������Ă���X���b�h
	WORKER_THREADS_POINTER	WorkThreads;
};

#endif // !defined(AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_)
