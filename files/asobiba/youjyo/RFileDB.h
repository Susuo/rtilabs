// RFileDB.h: RFileDB �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_)
#define AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotOpenException.h"
#include "RNotFoundException.h"

//�t�@�C�����f�[�^�x�[�X�Ƃ��Ďg�p����DB���ǂ�.
//postgresql �݂����ȃf�[�^�Ǘ������܂��B
//�����A�����͂��Ȃ�ł����A�N���x���`�Ƃ�(w
class RFileDB
{
public:
	RFileDB();
	virtual ~RFileDB();

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
	void Select(DWORD inKey , RBuffer* outData) const throw(RNotFoundException);

	/**
	 * Insert:					�}��
	 *
	 * @param inKey				�����L�[
	 * @param inData			�}������f�[�^
	 * @return void 
	 */
	void Insert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException);
	/**
	 * Update:					�ύX
	 *
	 * @param inKey				�����L�[
	 * @param inData			�ύX����f�[�^
	 * @return void 
	 */
	void Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException);
	/**
	 * Delete:					�폜
	 *
	 * @param inKey				�����L�[
	 * @return void				
	 */
	void Delete(DWORD inKey ) throw(RNotFoundException);
	/**
	 * IsHit:					���݃`�F�b�N
	 *
	 * @param inKey				�����L�[
	 * @return bool 
	 */
	bool IsHit(DWORD inKey )  const;
	/**
	 * Upsert:					Update / Insert
	 *
	 * @param inKey				�����L�[
	 * @return bool 
	 */
	bool Upsert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
	{
		if ( IsHit(inKey) )
		{
			Update(inKey,inData);
		}
		else
		{
			Insert(inKey,inData);
		}
	}


	/**
	 * Vacuum:					�s�v�ȃf�[�^�𐮗����܂�	(Windows�ł����� �f�t���O)
	 *
	 * @return void 
	 */
	void Vacuum( );

	//�e�X�g
	static void RFileDB::test();
private:
	//�X�g���X�e�X�g�p�f�[�^
	struct StTestData
	{
		int pos;
		int str;
	};

	//�����̏��Ƀf�[�^�[�x�[�X�Ɋi�[����Ă��邩�ǂ����e�X�g����   �e�X�g�p
	static void RFileDB::AssertFinder(const RFileDB * inDB , int inNumber ,...);
	//�X�g���X�e�X�g�p�̃f�[�^���쐬.
	static void RFileDB::StressTestMakeRandomData(StTestData * outData , const int inTestCount , const int inTestDataCount);


private:
	/**
	 * Close:			�f�[�^�x�[�X�̃N���[�Y
	 *
	 * @return void 
	 */
	void Close() throw(RIOCannotOpenException);
	/**
	 * InsertIndex:		index �ɑ}��
	 *
	 * @param inKey		�����L�[
	 * @param inPos		�}���ꏊ
	 * @param inSize	�f�[�^�̃T�C�Y
	 * @return void 
	 */
	void InsertIndex(DWORD inKey , DWORD inPos, DWORD inSize);
	/**
	 * FindIndex:		index ��茟��
	 *
	 * @param inKey		�����L�[
	 * @param outPos	�f�[�^�̏ꏊ
	 * @param outSize	�f�[�^�̃T�C�Y
	 * @return bool		���������ꍇ�@true 
	 */
	bool FindIndex(DWORD inKey , DWORD * outPos , DWORD * outSize) const;
	/**
	 * UpdateIndex:		index �̍X�V
	 *
	 * @param inKey		�����L�[
	 * @param inPos		�f�[�^�̏ꏊ
	 * @param inSize	�f�[�^�T�C�Y
	 * @return void 
	 */
	void UpdateIndex(DWORD inKey , DWORD inPos, DWORD inSize);
	/**
	 * DeleteIndex:		index �̍폜
	 *
	 * @param inKey		�����L�[
	 * @return void 
	 */
	void DeleteIndex(DWORD inKey );

	/**
	 * Open:					�f�[�^�x�[�X�̃I�[�v��
	 *
	 * @param inFilename		�f�[�^�x�[�X��
	 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
	 * @param mode				fopen�̃��[�h
	 * @return void 
	 */
	void Open(const string& inFilename , int inHashTableSize , const char* mode);

	/**
	 * CheckFreeSize:			���݂̏ꏊ���ǂꂾ���̃t���[�X�y�[�X���m�ۂł��邩���ׂ܂�
	 *
	 * @param inStart			�J�n����ꏊ
	 * @return DWORD			�m�ۂł���T�C�Y
	 */
	DWORD CheckFreeSize(DWORD inStart);

	//INDEX
	typedef pair<DWORD,DWORD> OneParts;
	typedef map<DWORD , OneParts>	SecondIndexDef;
	SecondIndexDef*		Index;

	//�n�b�V���T�C�Y.
	DWORD				HashSize;

	//INDEX���i�[����t�@�C��
	FILE*		IndexFile;
	//�i�[����t�@�C��.
	FILE*		File;
};

#endif // !defined(AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_)
