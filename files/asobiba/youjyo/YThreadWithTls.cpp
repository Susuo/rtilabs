// YThreadWithTls.cpp: YThreadWithTls �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YThreadWithTls.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YThreadWithTls::YThreadWithTls(RThreadPool<YThreadWithTls>* inPool) throw(RWin32Exception)
{
	OwnerPool = inPool;
	Request = NULL;
}

YThreadWithTls::~YThreadWithTls()
{
}

/**
 * Run:		���s
 *
 * @return virtual unsigned int			��� 0
 */
unsigned int YThreadWithTls::Run()
{
	while( this->IsHoldUp() )
	{
		if (Request)
		{	//���s
			Request->Execute(this);
			//���s���I������̂Ŕj��.
			delete Request;
			//�ꉞ�I��������ǁA�V�����d������?
			Request = OwnerPool->complateRequest(this);
		}
		else
		{	//���s������̂Ȃ��̂ő҂�
			this->Wait();
		}
	}
	return 0;
}

