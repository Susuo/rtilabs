// RDebug.cpp: RDebug �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RDebug::RDebug()
{

}

RDebug::~RDebug()
{

}

/**
 * MemoryToFile:		���������t�@�C����
 *
 * @param inFilename	�t�@�C����
 * @param inMemory		������
 * @param inSize		�������T�C�Y
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,const void* inMemory , int inSize)
{
	FILE * fp = fopen( inFilename.c_str() ,"wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return;
	}

	fwrite( inMemory  ,sizeof(char) , inSize , fp);

	fclose(fp);
}

/**
 * MemoryToFile:		���������t�@�C����
 *
 * @param inFilename	�t�@�C����
 * @param inMemory		������
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,const char* inMemory )
{
	RDebug::MemoryToFile(inFilename , inMemory , strlen(inMemory) );
}

/**
 * MemoryToFile:		���������t�@�C����
 *
 * @param inFilename	�t�@�C����
 * @param inMemory		������
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,string inMemory )
{
	RDebug::MemoryToFile(inFilename , inMemory.c_str() , inMemory.size() );
}

/**
 * FileCheck:			��̃t�@�C�����������ǂ����`�F�b�N.
 *
 * @param inSrc			�t�@�C����1
 * @param inDest		�t�@�C����2
 * @return void 
 */
void RDebug::FileVerify(const string inSrc,const string inDest)
{
	FILE* src = fopen(inSrc.c_str() , "rb" );
	ASSERT(src);
	FILE* dest = fopen(inDest.c_str() , "rb" );
 	ASSERT(dest);

	string error;

	char* srcBuffer = new char[1024];
	char* destBuffer = new char[1024];
	while(1)
	{
		int srclen = fread( srcBuffer , 1 , 1024 , src);
		int destlen = fread( destBuffer , 1 , 1024 , dest);

		if (srclen == 0 && destlen == 0) break;

		if (srclen != destlen)
		{
			fclose(src);			fclose(dest);
			//�T�C�Y�s��v
			ASSERT(0);
			break;
		}
		if ( memcmp(srcBuffer ,  destBuffer ,srclen) != 0)
		{
			fclose(src);			fclose(dest);
			//���e�s��v
			ASSERT(0);
			break;
		}
	}
	fclose(src);			fclose(dest);
	delete []  srcBuffer ;
	delete [] destBuffer ;
}
