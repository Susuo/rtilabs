// XLExceptionUtil.cpp: XLExceptionUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLExceptionUtil::XLExceptionUtil()
{

}

XLExceptionUtil::~XLExceptionUtil()
{

}

string XLExceptionUtil::WindowsCodeToString(DWORD inErrorCode)
{
	LPVOID msgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		inErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
		(LPTSTR) &msgBuf,
		0,
		NULL 
	);
	if (msgBuf == NULL)
	{
		msgBuf = "NULL";
	}

	string errorString;
#ifndef _WIN32_WCE
	errorString = format("::[WIN32 FormatMessage] ErrorCode:%d(%s)" , inErrorCode , (const char*)msgBuf ) ;
#else
	MY_USES_CONVERSION;

	TCHAR* wmsg = (TCHAR*)msgBuf;
	errorString = format("::[WIN32 FormatMessage] ErrorCode:%d(%s)" , inErrorCode ,  (const char*)W2A(wmsg) ) ;
#endif
	::LocalFree( msgBuf );

	return errorString;
}

