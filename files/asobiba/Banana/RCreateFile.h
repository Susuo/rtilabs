// RCreateFile.h: RCreateFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_)
#define AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

class RIStream
{
public:
	virtual int Read(void* inPtr , DWORD inSize ) throw(RWin32Exception) = 0;
	virtual int Write(const void* inPtr , int inSize ) throw(RWin32Exception) = 0;
};


class RCreateFile  : public RIStream
{
public:
	RCreateFile()
	{
		FilePointer = NULL;
	}
	virtual ~RCreateFile()
	{
		Close();
	}

	//�J��.
	void Open(
			const string & inFileName,                  // �t�@�C����
			DWORD dwDesiredAccess,                      // �A�N�Z�X���[�h
			DWORD dwShareMode = 0,                      // ���L���[�h
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, // �Z�L�����e�B�L�q�q
			DWORD dwCreationDisposition = OPEN_ALWAYS,  // �쐬���@
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL, // �t�@�C������
			HANDLE hTemplateFile = NULL                 // �e���v���[�g�t�@�C���̃n���h��
			) throw (RWin32Exception)
	{

		Close();

#ifndef _WIN32_WCE
		FilePointer = 
			::CreateFile(inFileName.c_str() , dwDesiredAccess , dwShareMode , 
				lpSecurityAttributes , dwCreationDisposition , dwFlagsAndAttributes , hTemplateFile);
#else
		MY_USES_CONVERSION;
		FilePointer = 
			::CreateFile( _A2W(inFileName.c_str()) , dwDesiredAccess , dwShareMode , 
				lpSecurityAttributes , dwCreationDisposition , dwFlagsAndAttributes , hTemplateFile);
#endif
		if (FilePointer == INVALID_HANDLE_VALUE)
		{
			FilePointer = NULL;
			throw RWin32Exception("CreateFile(%s) �����s���܂���" , inFileName.c_str() );
		}

	}

	//�ǂݎ��Ƃ��ĊJ��(�����Ȃ���΃G���[)
	void ReadFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_READONLY , NULL);
	}
	//�V�K�������݂Ƃ��ĊJ��(�Ȃ���΍쐬�A��������㏑��)
	void WriteFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ | GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL);
	}
	//�ǉ��Ƃ��ĊJ��(�Ȃ���΍쐬�A�������畁�ʂɊJ��)
	void AppendFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ | GENERIC_WRITE , 0 , NULL , OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL);
	}

	//����.
	void Close()
	{
		if (FilePointer == NULL) return ;

		CloseHandle(FilePointer);
		FilePointer = NULL;
	}

	virtual int Read(void* inPtr , DWORD inSize ) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD readBytes;
		if ( ::ReadFile(FilePointer,inPtr,inSize , &readBytes , NULL) == FALSE)
		{
			throw RWin32Exception("ReadFile �����s���܂���");
		}
		return readBytes;
	}
	virtual int Write(const void* inPtr , int inSize ) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD writeBytes;
		if ( ::WriteFile(FilePointer,inPtr,inSize , &writeBytes , NULL) == FALSE)
		{
			throw RWin32Exception("WriteFile �����s���܂���");
		}
		return writeBytes;
	}


	//�T�C�Y�̎擾
	DWORD GetSize()
	{
		ASSERT(FilePointer != NULL);
		return GetFileSize(FilePointer,NULL);
	}
	//�w��ꏊ�ֈړ�
	void Seek(LONG inOffset,DWORD inSeek = FILE_BEGIN) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);
		if (SetFilePointer(FilePointer,inOffset,NULL,inSeek) == 0xFFFFFFFF)
		{
			throw RWin32Exception("SetFilePointera(seek) �����s���܂���");
		}
	}
	//���݈ʒu�̎擾
	DWORD Tell()  throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD tell = SetFilePointer(FilePointer,0,NULL,FILE_CURRENT);
		if (tell == 0xFFFFFFFF)
		{
			throw RWin32Exception("SetFilePointer(tell) �����s���܂���");
		}

		return tell;
	}

	//�n���h���̎擾(�댯�Ȃ̂ł���܂�g����!)
	HANDLE getHandle() const
	{
		return FilePointer;
	}
private:
	HANDLE	FilePointer;
};


class StreamChecker
{
public:
	StreamChecker(){}
	~StreamChecker(){}
	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead) = 0;
	virtual int getSeparater() const = 0;
	virtual int getNext() const = 0;
};


//�A��������s�R�[�h�܂�
class StreamCheckerForSecutiveReturnCode : public StreamChecker
{
public:
	StreamCheckerForSecutiveReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~StreamCheckerForSecutiveReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		if (inAllReadyRead -4  < 0) inAllReadyRead = 0;

		for (int i = inAllReadyRead; i < (inNowRead-3) ; i ++) 
		{
			if ( (inBuffer[i] == '\r' && inBuffer[i+1] == '\n' && inBuffer[i+2] == '\r' && inBuffer[i+3] == '\n') )
			{
				ConectStart = i + 4;
				Separater = i;
				return false;
			}
			else if (	(inBuffer[i] == '\r' && inBuffer[i+1] == '\r') ||
						(inBuffer[i] == '\r' && inBuffer[i+1] == '\r')  )
			{
				ConectStart = i + 2;
				Separater = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};
//���s�R�[�h�܂�
class StreamCheckerForReturnCode : public StreamChecker
{
public:
	StreamCheckerForReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~StreamCheckerForReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		for (int i = inAllReadyRead; i < inNowRead ; i ++) 
		{
			if ( inBuffer[i] == '\n' )
			{
				Separater = i - 1;
				ConectStart = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};

class RStream
{
public:
	RStream()
	{
		Buffer = NULL;
		Stream = NULL;
	}
	~RStream()
	{
		delete [] Buffer;
	}

	void Create(RIStream* ioStream , int inTotalBufer = 65535)
	{
		ASSERT(Buffer == NULL);

		Buffer =	new char[inTotalBufer];
		BufferLength = inTotalBufer;
		BufferUse = 0;

		Stream = ioStream;
	}

	int Read(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException);


	


	static void test();
private:
	RIStream*	Stream;
	char	*	Buffer;
	int			BufferLength;
	int			BufferUse;
};
#endif // !defined(AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_)
