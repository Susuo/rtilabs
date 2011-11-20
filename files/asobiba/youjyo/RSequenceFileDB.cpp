// RSequenceFileDB.cpp: RSequenceFileDB �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RSequenceFileDB.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSequenceFileDB::RSequenceFileDB()
{
	SequenceFile = NULL;
}

RSequenceFileDB::~RSequenceFileDB()
{
	Close();
}

/**
 * Close:			�f�[�^�x�[�X�̃N���[�Y
 *
 * @return void 
 */
void RSequenceFileDB::Close()
{
	if (SequenceFile)
	{
		fclose(SequenceFile);
		SequenceFile = NULL;
	}
}

/**
 * Open:					�f�[�^�x�[�X�̃I�[�v��
 *
 * @param inFilename		�f�[�^�x�[�X��
 * @param mode				fopen�̃��[�h
 * @return void 
 */
void RSequenceFileDB::Open(const string& inFilename , const char* mode) throw(RIOCannotOpenException)
{
	Close();

	const string sequence = inFilename + ".sequence";
	SequenceFile = fopen(sequence.c_str() , mode);
	if (SequenceFile == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " �f�[�^�x�[�X" + inFilename + "�̃V�[�P���X" + sequence + "���J���܂���");
	}
}

/**
 * Create:					�V�K�쐬
 *
 * @param inFilename		�t�@�C����	����Ώ㏑��
 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
 * @return void 
 */
void RSequenceFileDB::Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename,"w+b");

	DB.Create(inFilename , inHashTableSize);

	Next = 0;
}

/**
 * Load:					�ǂݍ���
 *
 * @param inFilename		�t�@�C����
 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
 * @return void 
 */
void RSequenceFileDB::Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename,"r+b");

	DB.Load(inFilename , inHashTableSize);

	fread(&Next , sizeof(int) , 1 , SequenceFile);
}

/**
 * Save:					�f�B�X�N�� index ���������݂܂��B index�� OnMemory �Ȃ̂ŃZ�[�u���Ȃ��Ɩ��Ӗ��ł�
 *
 * @return void 
 */
void RSequenceFileDB::Save()
{
	fseek( SequenceFile , 0 , SEEK_SET ) ;
	fwrite(&Next , sizeof(int) , 1 , SequenceFile);

	DB.Save();
}


/**
 * Flush:					�f�[�^�� flush ���܂��B
 *
 * @return void 
 */
void RSequenceFileDB::Flush()
{
	fflush(SequenceFile);

	DB.Flush();
}

