// RMmap.cpp: RMmap �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RMmap.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMmap::RMmap()
{
	this->MapFile = NULL;
	this->MapObject = NULL;
	this->MapMemory = NULL;
}

RMmap::~RMmap()
{
	if (this->MapObject)	Release();
}

void RMmap::Create(DWORD inMappingSize) throw(RWin32Exception)
{
	ASSERT(this->MapFile == NULL);	
	ASSERT(this->MapObject == NULL);
	ASSERT(this->MapMemory == NULL);

	this->MapObject = CreateFileMapping( INVALID_HANDLE_VALUE , NULL, PAGE_READWRITE , 0, inMappingSize	, NULL);
	if (this->MapObject == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "�������}�b�v�̍쐬�Ɏ��s");

	this->MapMemory = MapViewOfFile(this->MapObject , FILE_MAP_ALL_ACCESS , 0, 0, 0);
	if (this->MapMemory == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "�������}�b�v�̊֘A�t���Ɏ��s");
}

void RMmap::Create(const string inFilename ,DWORD inMappingSize, DWORD inOpenFlg) throw(RWin32Exception)
{
	ASSERT(this->MapFile == NULL);
	ASSERT(this->MapObject == NULL);
	ASSERT(this->MapMemory == NULL);

	this->MapFile = CreateFile( inFilename.c_str() ,
								GENERIC_READ | GENERIC_WRITE,
								NULL,NULL,inOpenFlg,
								FILE_FLAG_RANDOM_ACCESS,NULL );
	if (this->MapFile == INVALID_HANDLE_VALUE)
	{
		throw RWin32Exception(EXCEPTIONTRACE + "�t�@�C��" + inFilename + "���J���܂���");
	}

	this->MapObject = CreateFileMapping( this->MapFile , NULL, PAGE_READWRITE, 0, inMappingSize	, NULL);
	if (this->MapObject == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "�������}�b�v�̍쐬�Ɏ��s");

	this->MapMemory = MapViewOfFile(this->MapObject , FILE_MAP_ALL_ACCESS  , 0, 0, 0);
	if (this->MapMemory == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "�������}�b�v�̊֘A�t���Ɏ��s");
	DWORD err = GetLastError();
	if (err  == ERROR_ALREADY_EXISTS ) throw RWin32Exception(err,EXCEPTIONTRACE + "�������}�b�v�̊֘A�t���Ɏ��s");
}

void RMmap::Release()
{
	ASSERT(this->MapObject != NULL);
	ASSERT(this->MapMemory != NULL);

	UnmapViewOfFile(this->MapMemory);
	CloseHandle(this->MapObject);

	if (this->MapFile)
	{
		CloseHandle(this->MapFile);
	}
	this->MapFile = NULL;
	this->MapObject = NULL;
	this->MapMemory = NULL;
}

void RMmap::Flush(DWORD inFlushSize)
{
	if (this->MapObject)
	{
		if ( !FlushViewOfFile(this->MapMemory,inFlushSize ) ) throw RWin32Exception(EXCEPTIONTRACE + "�}�b�s���O�t�@�C���̃t���b�V���Ɏ��s");
	}
}

void RMmap::test()
{
	//4�o�C�g�̃t�@�C�����}�b�v���� 5�o�C�g�ڂ���������ŁA�t���b�V�����Ă݂�
	{
		RMmap	map;
		RDebug::MemoryToFile("testdata/rmmap1.txt" , "abcd");
		map.Create( "testdata/rmmap1.txt" , 100 );
		char * p = (char*) map.getMemory();
		//�ǂ߂܂�?
		ASSERT(p[0] == 'a');
		ASSERT(p[1] == 'b');
		ASSERT(p[2] == 'c');
		//�����܂�?
		p[4] = 'X';
		ASSERT(p[4] == 'X');
		map.Flush(0);
		map.Release();
		RDebug::FileVerify("testdata/rmmap1.txt" ,"testdata/rmmap1_1.txt" );
	}
}