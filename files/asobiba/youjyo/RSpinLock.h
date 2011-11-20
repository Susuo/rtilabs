// RSpinLock.h: RSpinLock �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_)
#define AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RLock.h"
/*
class RSpinLock  
{
public:
	RSpinLock()
	{
		ReadCount = 0;
	}
	virtual ~RSpinLock()
	{

	}

	void ReadLock()
	{
		printf("%x ReadLock() start\r\n",this);

		//��������ł��Ȃ����Ƃ��m�F.
		this->WriteLockKey.Lock();

		//���b�N�I�u�W�F�N�g���Ԃ��L.
		this->SystemLock.Lock();

		//�N���ǂݍ���ł��Ȃ��ꍇ�͓ǂݍ��݃��b�N��������.
		if (this->ReadCount == 0)
		{
			ReadLockKey.Lock();
				printf("%x @Read Lock\r\n",this);
		}
				printf("%x %d\n" , this,this->ReadCount);
		this->ReadCount ++;
		
		//���b�N�I�u�W�F�N�g�̐�L���~�߂�
		this->SystemLock.UnLock();
		//���R�������݂ł͂Ȃ��̂ŏ������݃��b�N�͔r������.
		this->WriteLockKey.UnLock();

		printf("%x ReadLock() end\r\n",this);
	}
	void ReadUnLock()
	{
		printf("%x ReadUnLock() start\r\n",this);

		//���b�N�I�u�W�F�N�g���Ԃ��L.
		this->SystemLock.Lock();
	
		//�������Ō�̏������݂�������A�ǂݍ��݃��b�N�����.
		if (this->ReadCount == 1)
		{
			ReadLockKey.UnLock();
				printf("%x @Read UnLock\r\n",this);
		}
		this->ReadCount --;
				printf("%x %d\n" , this,this->ReadCount);

		//���b�N�I�u�W�F�N�g�̐�L���~�߂�
		this->SystemLock.UnLock();

		printf("%x ReadUnLock() end\r\n",this);
	}

	void WriteLock()
	{
		printf("%x WriteLock() start\r\n",this);

		//�N���ǂݍ���ł��Ȃ��̂��m�F.
		this->ReadLockKey.Lock();
		//�N����������ł��Ȃ��̂��m�F
		this->WriteLockKey.Lock();
			printf("%x *Write Lock\r\n",this);
		//�ǂݍ��݂͂��Ȃ��̂œǂݍ��݃��b�N������
		this->ReadLockKey.UnLock();

		printf("%x WriteLock() end\r\n",this);
	}

	void WriteUnLock()
	{
		printf("%x WriteUnLock() start\r\n",this);

		//�������݂̉��.
		this->WriteLockKey.UnLock();
			printf("%x *Write UnLock\r\n",this);

		printf("%x WriteUnLock() end\r\n",this);
	}

private:
	RLock	SystemLock;

	RLock	ReadLockKey;
	RLock	WriteLockKey;
	int		ReadCount;
};


//�����ǂݍ��݃��b�N
class RAutoReadSpinLock
{
public:
	RAutoReadSpinLock(RSpinLock * l)
	{
		Lock = l;
		Lock->ReadLock();
	}
	~RAutoReadSpinLock()
	{
		Lock->ReadUnLock();
	}
private:
	RSpinLock*	Lock;
};
//�����������݃��b�N
class RAutoWriteSpinLock
{
public:
	RAutoWriteSpinLock(RSpinLock * l)
	{
		Lock = l;
		Lock->WriteLock();
	}
	~RAutoWriteSpinLock()
	{
		Lock->WriteUnLock();
	}
private:
	RSpinLock*	Lock;
};
*/

#define RSpinLock RLock
#define RAutoReadSpinLock RAutoLock
#define RAutoWriteSpinLock RAutoLock

#endif // !defined(AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_)
