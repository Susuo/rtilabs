// RGISNameDatabaseDisk.cpp: RGISNameDatabaseDisk �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISNameDatabaseDisk.h"
#include "RStdioFile.h"
#include "RDiskUtil.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISNameDatabaseDisk::RGISNameDatabaseDisk()
{

}

RGISNameDatabaseDisk::~RGISNameDatabaseDisk()
{

}

unsigned short RGISNameDatabaseDisk::Write(const string & inFilename , const string & inName) throw (RException)
{
	RStdioFile file;

	//������́A \0 ��؂�œo�^����Ă��܂��B
	file.Open(inFilename , "a+b");

	long size = file.getSize();
	char* buffer = new char[size + 1];

	int line = 0;
	if (size > 0)
	{
		file.fseek(0);	//�擪��.
		file.fread(buffer , size );	//�S���ǂ�...

		//�ŁA inName ���ēo�^����Ă��܂�����?
		int addpoint = 0;
		for(line = 0 ;  ; line++)
		{
			const char * name = buffer + addpoint ;

			if ( strcmp(inName.c_str() , name) == 0)
			{
				delete [] buffer ;
				//���������񂪂������̂ł��������L����̂ł��B
				return line;
			}
			//���̕�����.
			addpoint += strlen(name) + 1;	//+1�� \0�̕�.
			if (addpoint >= size)
			{
				line ++;
				break;	//����ȏ�Ȃ��̂ł��B
			}
		}
	}
	//���[��A�o�^����Ă��Ȃ��悤�ł��ˁB
	//�ł́A�V�K�o�^�Ƃ������Ƃ�...

	file.fseek(0,SEEK_END);	//�Ō㕔��.
	file.fwrite(inName.c_str() , inName.size() + 1);

	delete [] buffer ;

	if (line > 0xffff)
	{
		throw RException(EXCEPTIONTRACE + "������ 65535 �𒴂��܂���");
	}
	return line ;
}

void RGISNameDatabaseDisk::test()
{
	string file = "test/namedatabasedisk.test";
	
	try
	{
		RDiskUtil::Delete(file);
	}
	catch(RException e)
	{
	}

	RGISNameDatabaseDisk disk;
	int r;
	r = disk.Write(file , "���ɂ����");
	ASSERT(r == 0);
	r = disk.Write(file , "���ɂ����");
	ASSERT(r == 0);
	r = disk.Write(file , "���ɂ��݂���");
	ASSERT(r == 1);
	r = disk.Write(file , "���ɂ��݂���");
	ASSERT(r == 1);
	r = disk.Write(file , "���ɂ������");
	ASSERT(r == 2);
	r = disk.Write(file , "���ɂ����");
	ASSERT(r == 0);
}
