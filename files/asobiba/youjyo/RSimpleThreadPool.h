// RSimpleThreadPool.h: RSimpleThreadPool �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_)
#define AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YMain.h"

#ifdef _DEBUG
	//�v�[�����Ă����X���b�h�̐�.
	const int MAX_CLIENT_THREAD	= 10;
#else
	//�v�[�����Ă����X���b�h�̐�.(���񂽁[�Ղ炢��)
	const int MAX_CLIENT_THREAD	= 100;
#endif


class RSimpleThreadPool  
{
public:
	RSimpleThreadPool()
	{
		for(int i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i] = new YMain();
			Threads[i]->Start();
		}
	}

	virtual ~RSimpleThreadPool()
	{
		//��~���ߔ��s
		for(int i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i]->HoldUp();
		}
		//��~�܂ő҂��č폜.
		for(i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i]->Join();
			delete Threads[i];
		}
	}
private:
	YMain*	Threads[MAX_CLIENT_THREAD];
};

#endif // !defined(AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_)
