// RAutoLibrary.h: RAutoLibrary �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
#define AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//DLL�̓ǂݍ��݂Ƃ�
class RAutoLibrary
{
public:
	//�\�z
	RAutoLibrary();

	//�w�肵�����O�ō\�z
	//���s�� RDoNotInitaliseException �ł��B
	RAutoLibrary(const char* inDLLName) ;

	//�j��
	virtual ~RAutoLibrary() ;

	//DLL�ǂݍ���
	//@inDLLName	DLL�̖��O
	void Load(const char* inDLLName) ;

	//DLL�j��
	void Destory();

	//DLL���� �֐����擾
	//@inFunctionName		�֐���
	//������������Ă��Ȃ��ꍇ�� RDoNotInitaliseException �ł��B
	//����Ȋ֐��Ȃ��ꍇ�� RNotSupportException �ł��B
	FARPROC GetProcAddress(const char* inFunctionName);

	//DLL�͓ǂݍ��܂�Ă����?
	bool IsReading();

private:
	HANDLE	LabraryHandle;
};

#endif // !defined(AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
