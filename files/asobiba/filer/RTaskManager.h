// RTaskManager.h: RTaskManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_)
#define AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786) 
#pragma warning (disable: 4503) 

#include "comm.h"

#include "RTask.h"
#include "RLock.h"
#include "Rfiles.h"
#include "Window.h"
#include "RNotify.h"
#include "Channel.h"

typedef deque<RTask*>	TASK_MANAGER;

//RTask���Ǘ�����N���X�B
//signleton �ł���B
//�܂��A RTask ����� Observer �Ƃ��ď������I�������Ăяo�����B
class RTaskManager  
{
private:
	//�^�X�N�Ǘ�
	TASK_MANAGER		Task;

	Channel				TaskChannel;
public:
	//singleton
	static RTaskManager* getInstance();

	//�^�X�N���Z�b�g.
	void putTask(RTask* inTask);

	//�^�X�N���~.
	void stopTask(const RTask* inTask);
	///����Window�Ɋ֘A�t�����Ă���^�X�N�����ׂč폜.
	void stopTasks(const Window* inWin);

	//RTask���������I������Ƃ��ɌĂяo�� 
	void CallEndOfTask(RTask * inTask,const Window* inTaskWindow);
public:
	RTaskManager();
	virtual ~RTaskManager();



};

#endif // !defined(AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_)
