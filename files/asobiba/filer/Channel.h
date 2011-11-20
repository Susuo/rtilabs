// hannel.h: Channel �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_)
#define AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Channel  ;

#include "Request.h"
#include "RLock.h"
#include "WorkerThread.h"


//�X���b�h�̍ő吔
const int MAX_WORKER_THREAD	=	5;

//���[�J�[�X���b�h�p�^�[���Ŏd��(Request)���Ǘ�����N���X.
class Channel  
{
public:
	Channel();
	virtual ~Channel();

	//�^�X�N���Z�b�g����
	void putRequest(Request* inRequest);
	//�^�X�N���擾����
	Request* takeRequest();
private:
	//enum hack

	typedef deque<Request*>		REQUEST;
	REQUEST	RequestManage;

	//�X���b�h����.
	WorkerThread*			Thread[MAX_WORKER_THREAD];
	//���b�N.
	RLock	Lock;
};

#endif // !defined(AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_)
