#include "StartWithExeFile.h"
#ifdef THREAD_LIFE

#include <.\VCL\rti_vcl_thread.h>
/*******************************************************************/
//�R���X�g���N�^
/*******************************************************************/
TThread::TThread(LPTHREAD_START_ROUTINE lpStartAdd,LPVOID lpvThreadParm = NULL,DWORD fdwCreate = NULL,int Priority = THREAD_PRIORITY_NORMAL)
{
	//�X���b�h�쐬
	ThreadHandle = CreateThread(
		NULL, 0, lpStartAdd, lpvThreadParm, fdwCreate, &lpIDThread);
	if(Priority != THREAD_PRIORITY_NORMAL)
	{ //�D��x�ݒ�
		SetThreadPriority(ThreadHandle, Priority);
	}
	//AllBreaker �� true �ɐݒ肳�ꂽ��X���b�h�𔲂���悤�ɂ���K�v������܂�
	AllBreaker = false;
}

/*******************************************************************/
//�f�X�g���N�^
/*******************************************************************/
TThread::~TThread()
{
	if(ThreadHandle)
	{ //�X���b�h���܂������Ă���
		WaitFor(); //�I��������܂ő҂�
		//�X���b�h�n���h�������
		CloseHandle(ThreadHandle);
	}
}

/*******************************************************************/
//�X���b�h���I���܂ő҂�
/*******************************************************************/
void TThread::WaitFor()
{
	AllBreaker = true;
	WaitForSingleObject(ThreadHandle,INFINITE);
}
/*******************************************************************/
//�n���h�����N���A����
/*******************************************************************/
void TThread::TheEnd()
{
	if(ThreadHandle)
	{
		CloseHandle(ThreadHandle);
		ThreadHandle=NULL;
	}
}

#endif //LIFE END
