#include "StartWithExeFile.h"
#ifdef THREAD_LIFE

#ifndef ___TTHREADH
#define ___TTHREADH
#include <windows.h>

class TThread
{
public:
	//�Ăяo�����[�`�� , ���� , ���̑� , �D��x  
    TThread(LPTHREAD_START_ROUTINE lpStartAdd,LPVOID lpvThreadParm,DWORD fdwCreate,int Priority);
    ~TThread();
	void WaitFor();
	void TheEnd();


	HANDLE ThreadHandle; //�X���b�h�n���h��
	DWORD lpIDThread;	 //�X���b�hID
	bool AllBreaker;     //�X���b�h���f
};

#endif

#endif   // LIFE END
