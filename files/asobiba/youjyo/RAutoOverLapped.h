// RAutoOverLapped.h: RAutoOverLapped �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_)
#define AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RAutoOverLapped  : public OVERLAPPED
{
public:
	RAutoOverLapped()
	{
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~RAutoOverLapped()//�p���֎~
	{
		CloseHandle(hEvent);
	}
};

#endif // !defined(AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_)
