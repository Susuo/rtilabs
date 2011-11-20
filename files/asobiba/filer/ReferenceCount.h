// ReferenceCount.h: ReferenceCount �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_)
#define AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ReferenceCount  ;

#include "comm.h"
#include "RLock.h"

class ReferenceCount  
{
public:
	ReferenceCount()
	{
		this->UseCount = 0;
		//�쐬.
		Inc();
	}
protected:
	//�f�X�g���N�^�͏���ɌĂяo�����Ƃ͂ł��܂���
	virtual ~ReferenceCount()
	{
	}
private:
	//�R�s�[�R���X�g���N�^�͌Ăׂ܂���.
	ReferenceCount(ReferenceCount & inOrignal)
	{
	}
public:
	//���̃N���X���J�n����
	void	Inc()
	{
		Lock.Lock();
		{//�����̊Ԃ̓N���e�B�J���Z�N�V�����Ŏ���Ă��܂��B
			UseCount++;
		}
		Lock.UnLock();
	}
	//���̃N���X�̎g�p���~�߂�
	bool	Dec()
	{
		Lock.Lock();
		{//�����̊Ԃ̓N���e�B�J���Z�N�V�����Ŏ���Ă��܂��B
			UseCount--;
			if (UseCount <= 0)
			{
				Lock.UnLock();
				delete this;
				return true;
			}
		}
		Lock.UnLock();
		return false;
	}
private:
	//���b�N.
	RLock			Lock;
	//�Q�ƃJ�E���g.
	int		UseCount;
};

#endif // !defined(AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_)
