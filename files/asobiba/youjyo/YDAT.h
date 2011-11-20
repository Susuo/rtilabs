// YDAT.h: YDAT �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_)
#define AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"
#include "RSpinLock.h"
#include "RStreamUtil.h"
#include "YHttpdConf.h"
#include "RMmap.h"

typedef unsigned long DATNUMBER;
/*
readme.html:Step2
<B>���X�X�g�͂����Ȃ�ł���?</B>
	�`���b�gch �Ɍh�ӂ�\���� 5000 �ƂȂ��Ă��܂��B
	�����ύX����ꍇ YDAT.h �� MAX_NUMBER �����������Ă��������B

	�������AMAX�������X���b�h�ɑ΂��č폜�˗��Ȃǂ�����Ƃ����Ȃ��̂ŁA
	�V�X�e�����̉B���G���A�Ƃ��� + 100���Ă��܂��B
	�܂�A 5100 ���{���̃��X�X�g�ƂȂ�܂��B

	����ȏ�́A�ǂ����������Ə������߂܂���B

<B>�X���e�ʐ����͂����ł���?</B>
	�X���e�ʐ����́A800kb �ƂȂ��Ă��܂��B
	�`���b�gch �� DAT ���݂�� 5000 �������݂� 540kb ���炢�Ȃ̂ŁA�\���ɐ₦����Ǝv���܂��B
	����āA AA �E�l�₫��ŃQ�b�^�[�͉����Ȃ� AA ��\��t���邱�Ƃ��ł��܂��B
*/

//�ő发�����ݐ�.
const int MAX_NUMBER	= 5000 ;
//���ۂ̌��E�l.
const int MAX_NUMBER_FULL = MAX_NUMBER + 100;
//�x�����o����.
const int WARNING_NUMBER	= ((int)(MAX_NUMBER * 0.85)) ;


//DAT�̍ő�T�C�Y.
const int MAX_DAT_SIZE	=	800 * 1024;	//800k
//�x�����o���T�C�Y
const int WARNING_DAT_SIZE	=	((int)(MAX_DAT_SIZE * 0.85));



class YDAT  
{
public:
	/**
	 * YDAT:	�R���X�g���N�^
	 *
	 * @param inDatNumber		DAT�̖��O
	 * @return  
	 */
	YDAT::YDAT(DATNUMBER	inDatNumber);

	/**
	 * ~YDAT:		�f�X�g���N�^
	 *
	 * @return  
	 */
	YDAT::~YDAT();

	/**
	 * Load:		INDEX,DAT�t�@�C���̓ǂݍ���.
	 *
	 * @return void 
	 */
	void YDAT::Load(const string inBoardName) throw(RIOCannotOpenException)
	{
		RAutoReadSpinLock al(&Lock) ;

		IndexLoad(inBoardName);
		DATLoad(inBoardName);
	}
	/**
	 * Save:		INDEX,DAT�t�@�C���̏�������.
	 *
	 * @return void 
	 */
	void YDAT::Save(const string inBoardName) const throw(RIOCannotWriteException)  
	{
		RAutoWriteSpinLock al(&Lock) ;

		IndexSave(inBoardName);
		DATSave(inBoardName);
	}
	/**
	 * ReadAt:			inStartNo �` inEndNo �܂œǂݍ���
	 *
	 * @param outRPack	�ǂݍ��񂾓��e���ۑ������o�b�t�@.
	 * @param inStartNo �J�n�n�_
	 * @param inEndNo	�I���n�_
	 * @return void	
	 */
	bool YDAT::ReadAt(RPack * outRPack,unsigned int inStartNo,unsigned int inEndNo) const  ;
	/**
	 * ReadAtGoEND:		inStartNo �` ALL �܂œǂݍ���
	 *
	 * @param outRPack	�ǂݍ��񂾓��e���ۑ������o�b�t�@.
	 * @param inStartNo �J�n�n�_
	 * @return void 
	 */
	bool YDAT::ReadAtGoEND(RPack * outRPack,unsigned int inStartNo) const  ;

	/**
	 * ReadByte:		inStartByte �` inEndByte �܂œǂݍ���
	 *
	 * @param outRPack		�ǂݍ��񂾓��e���ۑ������o�b�t�@.
	 * @param inStartByte	�J�n�o�C�g
	 * @param inEndByte		�I���o�C�g
	 * @return bool 
	 */
	bool YDAT::ReadByte(RPack * outRPack,DWORD inStartByte,DWORD inEndByte) const  ;
	/**
	 * ReadByteGoEND:		inStartByte �` ALL �܂œǂݍ���
	 *
	 * @param outRPack		�ǂݍ��񂾓��e���ۑ������o�b�t�@.
	 * @param inStartByte	�J�n�o�C�g
	 * @return bool 
	 */
	bool YDAT::ReadByteGoEND(RPack * outRPack,int inStartByte) const  ;

	/**
	 * Append:				��������
	 *
	 * @param	inRPack		�������ރf�[�^
	 */
	void Append(const RPack * inRPack) throw(RIOCannotWriteException);
	/**
	 * Append:				��������(Admin ���_���E�l�ʒu�܂�)
	 *
	 * @param	inRPack		�������ރf�[�^
	 */
	void AppendAdmin(const RPack * inRPack) throw(RIOCannotWriteException);
	/**
	 * Overwrite:				�I�[�o�[���C�h
	 *
	 * @param inDeleteNo 
	 * @param inOverwrite 
	 * @return void 
	 */
	void Overwrite(int inDeleteNo , const RPack * inOverwrite) throw(RIOCannotWriteException);

	/**
	 * setSubject:			Subject �̃Z�b�g
	 *						�V�K�������݂̂Ƃ��ɌĂԂׂ�.
	 *
	 * @param inSubject		�ǉ�����T�u�W�F�N�g
	 * @return void 
	 */
	void setSubject(const string inSubject)
	{
		this->Subject = inSubject;
	}

	//�Ō�̏������݃i���o�[�̎擾.
	int getLastNo() const
	{
		return this->No;
	}

	time_t getLastModified() const
	{
		return this->LastModified;
	}
	string getSubject() const
	{
		return this->Subject;
	}
	DATNUMBER getDatNumber() const
	{
		return this->DatNumber;
	}
	int getDatSize() const
	{
		return this->Index[No];
	}

	// aaaaaaa (10) �`���Ŏ擾����
	string getSubjectAndLastNo() const
	{
		return this->getSubject() + " (" + num2str(this->getLastNo() ) + ")" ;
	}

	static void test();
private:
	/**
	 * IndexLoad:		INDEX�t�@�C���̓ǂݍ���.
	 *
	 * @return void 
	 */
	void YDAT::IndexLoad(const string inBoardName);
	/**
	 * DATLoad:		DAT�t�@�C���̓ǂݍ���.
	 *
	 * @return void 
	 */
	void YDAT::DATLoad(const string inBoardName);
	/**
	 * IndexLoad:		INDEX�t�@�C���̏�������
	 *
	 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
	 * @return void 
	 */
	void YDAT::IndexSave(const string inBoardName) const  throw(RIOCannotOpenException);
	/**
	 * DATLoad:		DAT�t�@�C���̏�������
	 *
	 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
	 * @return void 
	 */
	void YDAT::DATSave(const string inBoardName) const throw(RIOCannotOpenException);

private:
	//DAT��Number .
	DATNUMBER	DatNumber;
	//���݂̏������ݐ�
	int					No;
	//���̔ԍ��̏������݂����o�C�g�ڂɃ��[�h���ꂽ�����߂�e�[�u��.
//	DWORD				Index[MAX_NUMBER_FULL];
	DWORD				*Index;
	RMmap				IndexMM;

	//���ۂ̏�������.
//	char				Dat[MAX_DAT_SIZE];
	char				*Dat;
	RMmap				DatMM;
	//�ŏI�������ݎ���.
	DATNUMBER			LastModified;
	//�T�u�W�F�N�g
	string				Subject;


	//���b�N
	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_)
