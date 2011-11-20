// RStdioFile.cpp: RStdioFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStdioFile::RStdioFile()
{
	FilePointer = NULL;
}

RStdioFile::~RStdioFile()
{
	if (FilePointer)	Close();
	
}

//�J��.
void RStdioFile::Open(string inFilename,string inMode) throw (RIOCannotOpenException)
{
	if (FilePointer) Close();

	FilePointer = ::fopen(inFilename.c_str() , inMode.c_str() );
	if (!FilePointer)
	{
		throw RIOCannotOpenException("�t�@�C�� %s ���J���܂���" , inFilename.c_str() );
	}
}

//����.
void RStdioFile::Close()
{
	ASSERT(FilePointer != NULL);

	fclose(FilePointer);
	FilePointer = NULL;
}


//�ǂݍ���.
string RStdioFile::ReadLine() throw (RIOCannotReadException)
{
	ASSERT(FilePointer != NULL);

	string ret;

	char buf[MAX_PATH];

	while(!feof(FilePointer))
	{
		if (  ::fgets( buf , MAX_PATH , FilePointer) == NULL)
		{
			if (feof(FilePointer))	break;	//����I��.
			throw RIOCannotReadException("�ǂݍ��ݒ��ɃG���[���������܂���");
		}

		int skip = ret.size() ;

		//�ǂݍ��񂾃o�b�t�@�ɘA��.
		ret += buf;
		//���s������܂���?
		if ( ret.find( string("\r\n") , skip) != -1) break;
	}
	return ret;
}

//��������
void RStdioFile::WriteLine(string inWrittingString) throw (RIOCannotWriteException)
{
	ASSERT(FilePointer != NULL);

	int count = ::fwrite( inWrittingString.c_str() , sizeof(char) , inWrittingString.size(), FilePointer);
	if (count != inWrittingString.size())
		throw RIOCannotWriteException("�������ݒ��ɃG���[���������܂���");
}

//����������?
bool RStdioFile::IsEof()
{
	return feof(FilePointer) != false;
}
