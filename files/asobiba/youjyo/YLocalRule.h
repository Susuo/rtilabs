// YLocalRule.h: YLocalRule �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_)
#define AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RSpinLock.h"

class YLocalRule  
{
public:
	YLocalRule();
	virtual ~YLocalRule();

	//���[�J�����[���̓ǂݍ���.
	void Load(const string inBoardName);

	const string getLocalRule()  const
	{
		RAutoReadSpinLock al(&Lock);
		return this->LocalRule;
	}

private:
	string		LocalRule;
	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_)
