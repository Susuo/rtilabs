// RfileLocal1.cpp: RfileLocal �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "comm.h"
#include "RfileLocal.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RfileLocal::RfileLocal()
{
	Handle = NULL;
}

RfileLocal::~RfileLocal()
{

}

//�I�[�v��
//@inPath	�p�X(Rfile�̓t�@�C�������������Ă��Ȃ�)
//@inMode	�J�����[�h
//#�J���Ȃ������ꍇ�� RIOCannotOpenException �𓊂���
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
void RfileLocal::Open(string inPath ,__RFILEOPENTYPE inMode) throw()
{
	string fullpath = inPath + getVars(__FILECOLUMN_TYPE_NAME);
	if (Handle != NULL) throw RIOCannotOpenException("���łɃt�@�C�� %s �͊J����Ă��܂�!", fullpath.c_str() );

	switch(inMode)
	{
	case __RFILEOPENTYPE_READ:
		Handle = CreateFile( fullpath.c_str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN  | FILE_FLAG_OVERLAPPED, NULL);
		break;
	case __RFILEOPENTYPE_WRITE:
		Handle = CreateFile( fullpath.c_str() , GENERIC_WRITE , FILE_SHARE_READ , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED, NULL);
		break;
	default:
		throw RNotSupportException("�t�@�C�� %s �͎w�肵������𗝉��ł��܂���", fullpath.c_str() );
	}
	if (Handle == NULL) throw RIOCannotOpenException("�t�@�C�� %s �̃I�[�v���Ɏ��s���܂��� OS ErrorCode %ld ", fullpath.c_str() , GetLastError() );


	OverLapped.hEvent = CreateEvent(NULL,FALSE,FALSE, fullpath.c_str() );
	OverLapped.Internal = 0;
	OverLapped.InternalHigh = 0;
	OverLapped.Offset = 0;
	OverLapped.OffsetHigh = 0;

	return;
}

//�N���[�Y
//#����Ȃ������ꍇ�� RIOCannotCloseException �𓊂���
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
void RfileLocal::Close() throw()
{
	if (Handle == NULL) return;

	CloseHandle( Handle );
	CloseHandle(OverLapped.hEvent);

	Handle = NULL;
}

//���[�h
//@outBuf		�o�b�t�@
//@inBufferSize	�o�b�t�@��
//#�ǂݍ��ݎ��s�� RIOCannotReadException ��Ԃ�
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
bool RfileLocal::Read(char* outBuf,int inBufferSize) throw()
{
	ASSERT(inBufferSize > 0);
//	printf("read %d\n" , inBufferSize);

	while(1)
	{
		if ( ReadFile( Handle , outBuf, inBufferSize , NULL, &OverLapped  ) )
			break;
		switch(GetLastError())
		{
		case ERROR_HANDLE_EOF:	//�Ō�܂œǂ񂾁[
			return false;
		case ERROR_IO_PENDING:	//���ݏ������������
			return true;
		case ERROR_WORKING_SET_QUOTA:
			puts("read ERROR_WORKING_SET_QUOTA");
			::Sleep(0);
			break;				//���g���C
		default:
			throw RIOCannotReadException("�ǂݍ��ݎ��s (%ld)" , GetLastError() );
		}
	}
	return true;
}

//���C�g
//@outBuf		�o�b�t�@
//@inBufferSize	�o�b�t�@��
//?	>= 0 �������񂾃o�C�g��
//  <  0 Eof
//#�������ݎ��s�� RIOCannotReadException ��Ԃ�
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
bool RfileLocal::Write(char* outBuf,int inBufferSize) throw()
{
	ASSERT(inBufferSize > 0);

//	printf("write %d\n" , inBufferSize);

	while(1)
	{
		if ( WriteFile( Handle , outBuf, inBufferSize , NULL, &OverLapped)  )
			break;
		switch(GetLastError())
		{
		case ERROR_IO_PENDING:	//���ݏ������������
			return true;
		case ERROR_WORKING_SET_QUOTA:
			puts("write ERROR_WORKING_SET_QUOTA");
			::Sleep(0);
			break;				//���g���C
		default:
			throw RIOCannotReadException("�������ݎ��s (%ld)" , GetLastError() );
		}
	}
	return true;
}


//�ǂݍ��ݏI���܂ő҂�.
//#�������ݎ��s�� RIOException ��Ԃ�
void RfileLocal::WaitFor() throw()
{
	DWORD ret = ::WaitForSingleObject( OverLapped.hEvent , INFINITE);
	if (ret  != WAIT_OBJECT_0)
		throw RIOException("WaitForSingleObject �� %ld ��Ԃ��܂���",ret);
}

//�I��������ǂ������ׂ�
//? true	�I�����WaitFor() ���ł���
//? false	�܂�
//#�������ݎ��s�� RIOException ��Ԃ�
bool RfileLocal::Poll() throw()
{
	DWORD ret = ::WaitForSingleObject( OverLapped.hEvent , 0);
	if (ret  == WAIT_TIMEOUT)		return false;
	else if (ret == WAIT_OBJECT_0)	return true;

	throw RIOException("WaitForSingleObject �� %ld ��Ԃ��܂���",ret);
}

//�����̌��ʂ����炢�܂��B
//Poll �� true / �܂��́AWaitFor �̃��^�[����Ɏg���Ă�������.
//#�������ݎ��s�� RIOException ��Ԃ�
unsigned long RfileLocal::GetReport() throw()
{
	DWORD tranceByte;
	BOOL ret = GetOverlappedResult( Handle , &OverLapped , &tranceByte , FALSE );
	if (ret == FALSE)
		throw RIOException("GetOverlappedResult �����s���܂���(%ld)", GetLastError() );

//	printf("�t�@�C�� %s comp %ld  %ld\n" , Name.c_str() , tranceByte , OverLapped.InternalHigh );

	OverLapped.Offset += tranceByte;
	OverLapped.OffsetHigh = 0;

	ResetEvent( OverLapped.hEvent );

	return tranceByte;
}
