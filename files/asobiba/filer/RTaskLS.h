// RTaskLS.h: RTaskLS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_)
#define AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTask  ;


#include "comm.h"


#include "RTask.h"
#include "Rfiles.h"

class RTaskLS : public RTask  
{
public:
	RTaskLS(Rfiles* inNewFiles,string inMask, Display* inDisp , Window* inCallback);
	virtual ~RTaskLS();

	//�i���� �J�n0 ----> ���S100 
	virtual int Parsent();
	//�^�X�N�r���[�ɕ\�����郁�b�Z�[�W
	virtual string getMessage();

private:
	//ls���s�J�n.
	virtual void Execute();

private:
	//�������ʂ��}�X�N.
	string Mask;
};

#endif // !defined(AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_)
