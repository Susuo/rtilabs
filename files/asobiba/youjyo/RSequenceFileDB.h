// RSequenceFileDB.h: RSequenceFileDB �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_)
#define AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RFileDB.h"

//�A�Ԃ��ۏ؂���Ă���f�[�^�x�[�X.
class RSequenceFileDB  
{
public:
	RSequenceFileDB();
	virtual ~RSequenceFileDB();

	/**
	 * Create:					�V�K�쐬
	 *
	 * @param inFilename		�t�@�C����	����Ώ㏑��
	 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
	 * @return void 
	 */
	void Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException);

	/**
	 * Load:					�ǂݍ���
	 *
	 * @param inFilename		�t�@�C����
	 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
	 * @return void 
	 */
	void Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException);

	/**
	 * Save:					�f�B�X�N�� index ���������݂܂��B index�� OnMemory �Ȃ̂ŃZ�[�u���Ȃ��Ɩ��Ӗ��ł�
	 *
	 * @return void 
	 */
	void Save();

	/**
	 * Flush:					�f�[�^�� flush ���܂��B
	 *
	 * @return void 
	 */
	void Flush();

	/**
	 * Select:					����
	 *
	 * @param inKey				�����L�[
	 * @param outData			�f�[�^���������ރo�b�t�@
	 * @return void 
	 */
	void Select(DWORD inKey , RBuffer* outData) const throw(RNotFoundException)
	{
		DB.Select(inKey , outData);
	}

	/**
	 * Insert:					�}��
	 *
	 * @param inData			�}������f�[�^
	 * @return void 
	 */
	int Insert(const RBuffer& inData ) throw(RNotFoundException)
	{
		DB.Insert(Next , inData);

		return Next++;
	}

	/**
	 * Update:					�ύX
	 *
	 * @param inKey				�����L�[
	 * @param inData			�ύX����f�[�^
	 * @return void 
	 */
	void Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
	{
		DB.Update(inKey , inData);
	}
	/**
	 * Delete:					�폜
	 *
	 * @param inKey				�����L�[
	 * @return void				
	 */
	void Delete(DWORD inKey ) throw(RNotFoundException)
	{
		DB.Delete(inKey);
	}
	/**
	 * IsHit:					���݃`�F�b�N
	 *
	 * @param inKey				�����L�[
	 * @return bool 
	 */
	bool IsHit(DWORD inKey )  const
	{
		return DB.IsHit(inKey);
	}

	/**
	 * Seek:					Next�L�[�̈ړ� (�댯)
	 *
	 * @param inNext			�ݒ肵����Next�l
	 */
	void Seek(int inNext)
	{
		Next = inNext;
	}


	/**
	 * Vacuum:					�s�v�ȃf�[�^�𐮗����܂�	(Windows�ł����� �f�t���O)
	 *
	 * @return void 
	 */
	void Vacuum( )
	{
		DB.Vacuum();
	}
private:	
	/**
	 * Close:			�f�[�^�x�[�X�̃N���[�Y
	 *
	 * @return void 
	 */
	void RSequenceFileDB::Close();
	/**
	 * Open:					�f�[�^�x�[�X�̃I�[�v��
	 *
	 * @param inFilename		�f�[�^�x�[�X��
	 * @param mode				fopen�̃��[�h
	 * @return void 
	 */
	void RSequenceFileDB::Open(const string& inFilename , const char* mode) throw(RIOCannotOpenException);

private:
	int			Next;
	RFileDB		DB;

	FILE*		SequenceFile;
};

#endif // !defined(AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_)
