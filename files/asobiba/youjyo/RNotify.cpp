// RNotify.cpp: RNotify �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RNotify.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RNotify::RNotify()
{
	NotifyHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,   NULL,    0,   0);
}
RNotify::~RNotify()
{
	CloseHandle(NotifyHandle);
}

//�ʒm����.
void RNotify::Post(void * inConnectPack)
{
	PostQueuedCompletionStatus(NotifyHandle, 0,(DWORD)inConnectPack , NULL);
}

//�I����`����.
void RNotify::Shutdown()
{
	PostQueuedCompletionStatus(NotifyHandle, 0, NULL, NULL);
}

//�ʒm�܂ł܂�.
bool RNotify::Wait(void ** outConnectPack)
{
	DWORD			dummy;
	OVERLAPPED *	dummy2 ;
	return Btob(GetQueuedCompletionStatus(NotifyHandle , &dummy , (DWORD*)outConnectPack , &dummy2 , INFINITE));
}
