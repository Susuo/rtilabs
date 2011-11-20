// RMiniDump.cpp: RMiniDump �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include "RMiniDump.h"
#include "RAutoLibrary.h"
#include "RThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//mini�_���v���Ƃ邽�ߐ�p�̃N���X
class RMiniDumpChild : public RThread
{

public:
	void Set(const string & inFileName , MINIDUMP_TYPE inFlag,EXCEPTION_POINTERS *poolExceptionInfo)
	{

		this->Filename = inFileName;
		this->Flag = inFlag;
		this->PoolExceptionInfo = poolExceptionInfo;
		
		DbgHelpDLL.Load("dbghelp.dll");
		this->Func_MiniDumpWriteDump = (FUNC_MINIDUMPWRITEDUMP)DbgHelpDLL.GetProcAddress("MiniDumpWriteDump");
	}
	
	
	unsigned int RMiniDumpChild::Run();
private:
	string				Filename;
	MINIDUMP_TYPE		Flag;
	EXCEPTION_POINTERS*	PoolExceptionInfo;
	
	//dbghelp.dll ���� MiniDumpWriteDump ���C���|�[�g����.
	RAutoLibrary		DbgHelpDLL;
	typedef BOOL (WINAPI *FUNC_MINIDUMPWRITEDUMP)(
		HANDLE hProcess,
		DWORD ProcessId,
		HANDLE hFile,
		MINIDUMP_TYPE	DumpType,
		CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
		CONST PVOID	UserStreamParam,
		CONST PVOID	CallbackParam
		);
	FUNC_MINIDUMPWRITEDUMP Func_MiniDumpWriteDump;
	
};

unsigned int RMiniDumpChild::Run()
{
	HANDLE file = CreateFile(
		this->Filename.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (this->PoolExceptionInfo == NULL)
	{
		//���݂̃A�h���X.
		this->Func_MiniDumpWriteDump(
			GetCurrentProcess(), 
			GetCurrentProcessId(), 
			file, 
			this->Flag, 
			NULL,
			NULL,
			NULL
		);
	}
	else
	{
		//�N���b�V�������������Ǝv����ꏊ
		MINIDUMP_EXCEPTION_INFORMATION   stExInfo ;

		stExInfo.ThreadId = GetCurrentThreadId ( ) ;
		stExInfo.ClientPointers = TRUE ;
		stExInfo.ExceptionPointers = this->PoolExceptionInfo ;

		this->Func_MiniDumpWriteDump(
			GetCurrentProcess(), 
			GetCurrentProcessId(), 
			file, 
			this->Flag, 
			&stExInfo,
			NULL,
			NULL
		);
	}
	CloseHandle(file);
	return 0;	
}


//�C�ӂ� mini dump���쐬����.
bool RMiniDump::MiniDump(const string & inFilename , MINIDUMP_TYPE inFlag,EXCEPTION_POINTERS *poolExceptionInfo )
{
	try
	{
		RMiniDumpChild minidump;
		minidump.Set(inFilename,inFlag,poolExceptionInfo);
		//�X���b�h�J�n
		minidump.Start();
		//�I���܂ő҂�
		minidump.Join();
	}
	catch(RException &e)
	{
		printf("�G���[: %s\r\n",e.getMessage() );
		//Dump�����悤�Ȏ��̂̂Ƃ��ɗ�O���΂�����_�����낤JK
		return false;
	}
	return true;
}
