// RAutoLibrary.h: RAutoLibrary �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_)
#define AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RAutoLibrary  
{
public:
	//�\�z
	RAutoLibrary()
	{
		LabraryHandle = NULL;
	}
	//�w�肵�����O�ō\�z
	RAutoLibrary(const char* inDLLName)
	{
		Create(inDLLName);
	}

	//�j��
	virtual ~RAutoLibrary()
	{
		Destory();
	}

	//DLL�ݒ�
	//@inDLLName	DLL�̖��O
	bool Create(const char* inDLLName)
	{
		Destory();

		LabraryHandle = ::LoadLibrary(inDLLName);
		return IsReading();
	}
	//DLL�j��
	void Destory()
	{
		if (!IsReading()) return;
		::FreeLibrary((HINSTANCE)LabraryHandle);
	}	

	//DLL���� �֐����擾
	//@inFunctionName		�֐���
	FARPROC GetProcAddress(const char* inFunctionName)
	{
		if (!IsReading()) return NULL;
		return ::GetProcAddress((HINSTANCE)LabraryHandle, inFunctionName);
	}

	//DLL�͓ǂݍ��܂�Ă����?
	bool IsReading()
	{
		return LabraryHandle != NULL;
	}

private:
	HANDLE	LabraryHandle;
};

#endif // !defined(AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_)
