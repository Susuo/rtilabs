// REvent1.h: REvent �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REVENT_H__01C444EA_3CE7_4A65_B60A_93FF2296EFB1__INCLUDED_)
#define AFX_REVENT_H__01C444EA_3CE7_4A65_B60A_93FF2296EFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class REvent  
{
public:
	REvent()
	{
		this->Event = ::CreateEvent(NULL,TRUE,FALSE, NULL);
	}
	virtual ~REvent()
	{
		::CloseHandle( this->Event );
	}

	//�V�O�i���ɂ���
	void SetEvent()
	{
		::SetEvent( this->Event  );
	}

	//��V�O�i���ɂ���
	void ResetEvent()
	{
		::ResetEvent( this->Event  );
	}

	//�V�O�i���ɂȂ�܂ő҂�.
	//���Ԃ��o�߂��Ĕ������ꍇ�� false .
	bool WaitForSignal(DWORD inWaitSecondTime)
	{
		return	::WaitForSingleObject( this->Event  , inWaitSecondTime) != WAIT_TIMEOUT;
	}

	//�V�O�i�����ǂ������ׂ�
	//true��������V�O�i��.
	bool Pooling()
	{
		return	::WaitForSingleObject( this->Event  , 0) == WAIT_TIMEOUT;
	}

private:
	HANDLE		Event;
};

#endif // !defined(AFX_REVENT_H__01C444EA_3CE7_4A65_B60A_93FF2296EFB1__INCLUDED_)
