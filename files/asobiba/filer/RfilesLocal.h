// RfilesLocal.h: RfilesLocal �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_)
#define AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RNotSupportException.h"
#include "RTableEasy.h"

#include "RfileFactory.h"
#include "RfileLocalBigIcon.h"
#include "RfileLocalReport.h"


//���[�J���f�B�X�N�t�@�C���ꗗ�̎擾.
//final
class RfilesLocal : public Rfiles  
{
private:
	//��ׂ܂����
	RfilesLocal() {}
public:
	//���������ł��Ȃ������ꍇ�� RNotSupportException �𓊂���.
	RfilesLocal(__RFILETYPE inRfileType,const string inPath) ;
	//final
	~RfilesLocal();

	//�ꗗ�擾
	virtual void ls();
	//�ꗗ�擾
	virtual void ls(string inFilter);
	//�폜
	virtual void rm(string inFilename);
	//���l�[��
	virtual void rename(string inSrcFilename,string inDestFilename);
	//��ނ�Ԃ�.
	virtual __RFILETYPE		getRfileType()	const;
	//�p�X�̎擾
	virtual string getPath() const ;
	//�p�X�̐ݒ�
	virtual void setPath(const string inPath)  ;

	//RTable���쐬���܂��B
	//ls ���g���O�ɌĂ�ł��������B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw();

	//�e�X�g.
	static void test();
private:
	//Rtable �ŕێ����Ă��� Rfile �̃^�C�v
	__RFILETYPE		RfileType;
	//�p�X.
	string			Path;

};

#endif // !defined(AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_)
