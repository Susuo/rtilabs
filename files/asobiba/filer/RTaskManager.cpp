// RTaskManager.cpp: RTaskManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTaskManager.h"
#include "RTaskLS.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTaskManager::RTaskManager()
{
}

RTaskManager::~RTaskManager()
{

}

RTaskManager* RTaskManager::getInstance()
{
	static RTaskManager rtm;
	return &rtm;
}


//RTask���������I������Ƃ��ɌĂяo�� 
void RTaskManager::CallEndOfTask(RTask * inTask,const Window* inTaskWindow)
{
	if (inTask == NULL) return ;	//�s��.

	//�^�X�N���X�g��茟��.
	TASK_MANAGER::iterator it = find( Task.begin() , Task.end() , inTask);
	if (it != Task.end())
	{
		//�L���ȃ^�X�N �Ȃ̂� Window�ɔ��f�������Ǝv���B

		//���̂܂��ɗL���ȃE�B���h�E�ł��邩����
		if (Filer::getFiler()->IsValidWindow(inTaskWindow) )
		{//�L���ȃE�B���h�E�Ȃ̂ōX�V����.
			inTask->WindowUpdate();
		}
		else
		{//�����ȃE�B���h�E�Ȃ̂ŁA�����ȃ^�X�N�Ƃ��ď���.
			TRACE("warning: Task %p is valid , but window is invalid!!\n",inTask);
			inTask->Invalid();
		}
		Task.erase( it );
	}
	else
	{
		//�����ȃ^�X�N�ł�.
		TRACE("warning: Task %p is invalid!!\n",inTask);

		inTask->Invalid();
	}
	delete inTask;
}

//�^�X�N���Z�b�g.
void RTaskManager::putTask(RTask* inTask)
{
	//�^�X�N�Ǘ��ɓo�^.
	Task.push_back(inTask);
	//���s���Ă˂Ɠo�^ ���[�J�[�X���b�h�p�^�[��.
	TaskChannel.putRequest(inTask);
}

//�^�X�N���~.
void RTaskManager::stopTask(const RTask* inTask)
{
	ASSERT(inTask != NULL);

	//�^�X�N���X�g��茟��.
	TASK_MANAGER::iterator it = find( Task.begin() , Task.end() , inTask);
	if (it != Task.end())
	{
		//��~�𑣂�(�^�X�N�͋}�ɂ͎~�܂�Ȃ�)
		(*it)->Stop();
		//�Ǘ��L���[���珜�O���܂��B
		Task.erase( it );
	}
	//���̂Ƃ��^�X�N���̂͐����Ă���̂�
	//delete ���Ă͂��߂ł��B
}


//����Window�Ɋ֘A�t�����Ă���^�X�N�����ׂč폜.
void RTaskManager::stopTasks(const Window* inWin)
{
	ASSERT(inWin != NULL);

	TASK_MANAGER::iterator it;
	for(it = Task.begin() ; it != Task.end() ;)
	{
		if (inWin == (*it)->getWindow() )
		{
			//��~�𑣂�(�^�X�N�͋}�ɂ͎~�܂�Ȃ�)
			(*it)->Stop();
			//�Ǘ��L���[���珜�O���܂��B
			it = Task.erase( it );
		}
		else
		{
			++it;
		}
	}

}
