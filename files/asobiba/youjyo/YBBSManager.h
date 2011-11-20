// YBBSManager.h: YBBSManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_)
#define AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YBBS.h"

#include "RNotFoundException.h"
#include "RIOCannotOpenException.h"
#include "YThreadWithTls.h"


class YBBSManager  
{
private:
	YBBSManager();
public:
	virtual ~YBBSManager();


	//�ǂݍ���
	void Read(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,
		const YRequestPath* inRequestPath, RStreamUtil * outBuffer ) throw(RNotFoundException,RIOCannotOpenException);
	
	//��������
	void Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,
		POST& ioPost, RStreamUtil * outBuffer ) throw(RNotFoundException,RIOCannotOpenException);

	/**
	 * SystemWrite:				�V�X�e������̃��b�Z�[�W
	 *
	 * @param outShareBuffer	TLS�o�b�t�@.
	 * @param ioPost			�������ޓ��e
	 * @return void 
	 */
	void SystemWrite(char* outShareBuffer,POST& ioPost) throw(RNotFoundException,RIOCannotOpenException);
	//�폜
	void Aborn(const string inTargetIta ,DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete)
		throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	//�폜�˗����ꂽ�������݂̂�΂��P����}�X�N����.
	void MaskReWrite(const string inTargetIta  ,DATNUMBER inDatNumber ,const string & password ,
					int inDeleteSize ,const int * inDelete ,
					int inMaskSize   ,const string *inMask ) 
		throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	//����̔̓���̃X���b�h�̃^�C�g���̎擾.
	const string getSubject(const string inBoardName , DATNUMBER inDatNumber) throw(RNotFoundException);


	static YBBSManager*	getInstance();

	/////����p�X���b�h�ɂ����s
	//�폜�l�^�X�N.
	void KillerTask(char* outShareBuffer);
	//DAT�K�y�[�W�R���N�^
	void GCTask();

	/////////////////�Ǘ��җp����
	//�����o��.
	void Dump();
	///���ׂĂ̏����f�B�X�N�Ƀt���b�V������.
	void Flush();
	//BBS���ēǂݍ���
	void ReloadBBS() throw(RException);

private:
	//BBS�����ׂĔj������.
	void ClearBBS();
	/**
	 * getBBS:					�w�肳�ꂽ�ɑ΂���BBS���擾.
	 *
	 * @param inTargetIta 
	 * @return YBBS* 
	 */
	YBBS* getBBS(const string &inTargetIta) throw(RNotFoundException);

private:
	typedef map<string,YBBS*>	BBSS;
	BBSS	AllBBS;
};

#endif // !defined(AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_)
