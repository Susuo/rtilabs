// RfilesSMBLogon.h: RfilesSMBLogon �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESSMBLOGON_H__3D127AC5_ABB0_414B_89FE_DD5E4E6DF8AB__INCLUDED_)
#define AFX_RFILESSMBLOGON_H__3D127AC5_ABB0_414B_89FE_DD5E4E6DF8AB__INCLUDED_

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

class RfilesSMBLogon : public Rfiles  
{
private:
	//��ׂ܂����
	RfilesSMBLogon() {}
public:
	//���������ł��Ȃ������ꍇ�� RNotSupportException �𓊂���.
	RfilesSMBLogon(__RFILETYPE inRfileType,const string inPath) ;
	//final
	~RfilesSMBLogon();

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

#endif // !defined(AFX_RFILESSMBLOGON_H__3D127AC5_ABB0_414B_89FE_DD5E4E6DF8AB__INCLUDED_)
