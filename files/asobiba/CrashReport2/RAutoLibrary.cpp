// RAutoLibrary.cpp: RAutoLibrary �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RAutoLibrary.h"
#include "RException.h"
#include "RExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�\�z
RAutoLibrary::RAutoLibrary()
{
	LabraryHandle = NULL;
}

//�w�肵�����O�ō\�z
//���s�� RDoNotInitaliseException �ł��B
RAutoLibrary::RAutoLibrary(const char* inDLLName)
{
	//�R���X�g���N�^�ŗ�O�͂���܂�悭�Ȃ��񂾂��ǁA�m�������Ƃ��{�P�B���J������
	Load(inDLLName);
}

//�j��
RAutoLibrary::~RAutoLibrary()
{
	Destory();
}

//DLL�ݒ�
//@inDLLName	DLL�̖��O
//���s�� RDoNotInitaliseException �ł��B
void RAutoLibrary::Load(const char* inDLLName) 
{
	Destory();

#ifndef _WIN32_WCE
	LabraryHandle = ::LoadLibrary(inDLLName);
#else
	MY_USES_CONVERSION;
	LabraryHandle = ::LoadLibrary(_A2W(inDLLName) );
#endif
	if (!IsReading())
	{
		DWORD error = ::GetLastError();
		throw RException(PrintFormat("RAutoLibrary:%s �̓ǂݍ��݂Ɏ��s���܂����B",inDLLName) + RExceptionUtil::WindowsCodeToString(error) );
	}
}
//DLL�j��
void RAutoLibrary::Destory()
{
	if (!IsReading())
	{
		return;
	}
	::FreeLibrary((HINSTANCE)LabraryHandle);
}

//DLL���� �֐����擾
//@inFunctionName		�֐���
FARPROC RAutoLibrary::GetProcAddress(const char* inFunctionName) 
{
	//���������Ȃ��œǂݍ������Ƃ����Ȃ́H�΂��Ȃ̃A�z�Ȃ̎��ʂ́H
	ASSERT(IsReading());

#ifndef _WIN32_WCE
	FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, inFunctionName);
#else
	MY_USES_CONVERSION;
	FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, _A2W(inFunctionName) );
#endif
	if (ret == NULL)
	{
		DWORD error = ::GetLastError();
		throw RException(PrintFormat("%s �֐��̓T�|�[�g����Ă��܂���B",inFunctionName) + RExceptionUtil::WindowsCodeToString(error) );
	}
	return ret;
}
//DLL�͓ǂݍ��܂�Ă����?
bool RAutoLibrary::IsReading()
{
	return LabraryHandle != NULL;
}


