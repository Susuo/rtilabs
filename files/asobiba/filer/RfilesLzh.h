// RfilesLzh.h: RfilesLzh �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_)
#define AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableOnlyOne.h"
#include "RfileArcBigIcon.h"
#include "RArchiveLha.h"
#include "RArchiveFactory.h"
#include "RfileFactory.h"
#include "RTableEasy.h"
#include "WindowsMain.h"
#include "VirtualPath.h"


//LZH�t�@�C��
class RfilesLzh : public Rfiles  
{
public:

	friend class ArcCacheFileFnctional;

	//�T�|�[�g���Ă��Ȃ���� RNotSupportException
	//!�������ł��Ȃ������� RDoNotInitaliseException
	//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
	RfilesLzh(__ARCHIVETYPE inArchiveType,__RFILETYPE inRfileType,const string inPath) throw();
	//final
	~RfilesLzh();

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
	//RTable���쐬���܂��B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	RTable* CreateTable(__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() ;

private:
	//Rtable �ŕێ����Ă��� Rfile �̃^�C�v
	__RFILETYPE		RfileType;
	//�p�X.
	string			Path;
	//���k�𓚃G���W��.
	RArchive*		ArchiveEngine;
};

//RfilesWithCache �Ŏg�p����t�@���N�^
//RfilesLzh�ɑ΂��� friend class
class ArcCacheFileFnctional : public CacheFileFnctional
{
public:

	//Rfile �̍쐬.
	virtual Rfiles* New(__RFILETYPE inRfileType,const string inSrcPath) const throw()
	{
		return new RfilesLzh(ArchiveType , inRfileType , inSrcPath );
	}

	//�\�[�X�p�X�̎擾
	//c:\test3\test.zip::/�V�����t�H���_/  ---> c:\test3\test.zip
	virtual string getSrcPath(const string inPath) const
	{
		VirtualPath vp(inPath);
		return vp.getNativePath();
	}

	//���΃p�X�̎擾
	//c:\test3\test.zip::\�V�����t�H���_\  ---> �V�����t�H���_\  
	virtual string getRelativePath(const string inPath) const
	{
		VirtualPath vp(inPath);
		return vp.getLastPath();
	}

	//���� inTargetFile ��ǉ����āE�́E�j��!!�ł���?
	virtual bool IsAppend(const string inSrcPath , const Rfile* inTargetFile) const
	{
		const string name = inTargetFile->getVars(__FILECOLUMN_TYPE_NAME);
		const int hitPos = name.find(inSrcPath);
		return (hitPos < name.length() );
	}

	//RTable �̍쐬.
	virtual RTable* ProxyCreateTable( const Rfiles* inSrcFiles,__FILECOLUMN_TYPE inKey,bool IsAscending) const
	{
		return ((RfilesLzh*)inSrcFiles)->CreateTable(inKey,IsAscending);
	}

	//�A�[�J�C���@�^�C�v.
	__ARCHIVETYPE	ArchiveType;
};


#endif // !defined(AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_)
