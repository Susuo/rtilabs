// YThreadWithTls.h: YThreadWithTls �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_)
#define AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class YThreadWithTls;
#include "RThread.h"
#include "RThreadPool.h"
#include "RRequest.h"
#include "RWin32Exception.h"

const int TLS_POOL_MAX		=	10 * 1024;	//10kB

class YThreadWithTls : public RThread  
{
public:
	YThreadWithTls(RThreadPool<YThreadWithTls>* inPool) throw(RWin32Exception);
	virtual ~YThreadWithTls();

	
	/**
	 * Run:		���s
	 *
	 * @return virtual unsigned int			��� 0
	 */
	virtual unsigned int Run();
	/**
	 * Bind:		�d����o�^����.
	 *
	 * @param inReq		�V�����d��
	 * @return void 
	 */
	void	Bind(RRequest* inReq)
	{
		Request = inReq;
	}

	char* getTLS() const
	{
		return (char*) Tls;
	}

private:
	//�X���b�h�v�[���{��.
	RThreadPool<YThreadWithTls>*	OwnerPool;
	//���N�G�X�g.
	RRequest*		Request;
	//TLS�̈�
	char	Tls[TLS_POOL_MAX];

};

#endif // !defined(AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_)
