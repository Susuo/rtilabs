// RLock.h: RLock �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_)
#define AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RLock;

#include "comm.h"



//���̃N���X�̓��b�N��!
//�����āA�����ǂ�ł���L�~�B�����b�N��!
//by �L��(w
class RLock  
{
private:
	CRITICAL_SECTION CriticalSection;
public:
	RLock()
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~RLock()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	//�N���e�B�J���Z�N�V�����ɓ���
	void Lock()
	{
		EnterCriticalSection(&CriticalSection);
	}

	//�N���e�B�J���Z�N�V�������甲����
	void UnLock()
	{
		 LeaveCriticalSection(&CriticalSection);
	}
};

//�������b�N.
//���̃N���X�͎������b�N��!
//�����āA�����ǂ�ł���L�~�B�����b�N��!
//by �L��(w �����A�������āB�B
class RAutoLock
{
public:
	RAutoLock(RLock * l)
	{
		Lock = l;
		Lock->Lock();
	}
	~RAutoLock()
	{
		Lock->UnLock();
	}
private:
	RLock*	Lock;
};


#endif // !defined(AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_)
