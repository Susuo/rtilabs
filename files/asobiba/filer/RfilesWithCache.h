// RfilesWithCache.h: RfilesWithCache �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_)
#define AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RFiles.h"
#include "RCacheFS.h"

//�e�X�g.
#include "RfilesLzh.h"

class RfilesWithCache : public Rfiles  
{
private:
	//��ׂ܂����
	RfilesWithCache(){}
public:
	RfilesWithCache(CacheFileFnctional* inCff , __RFILETYPE inRfileType , const string inPath) throw();

	virtual ~RfilesWithCache();


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
	//��ނ�Ԃ�.
	virtual string		getPath()	const;
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
	__RFILETYPE			RfileType;
	//�p�X.
	string				Path;
	//�t�@���N�^
	CacheFileFnctional*	Cff;
	//�\�[�X�p�X(�����I�ɕێ����Ă��� Rfiles��ł̃p�X)
	string				SrcPath;
	//�����I�ɕێ����Ă���I���W�i���� Rfiles*
	Rfiles*				OrignalFiles;
	//�����I�ɕێ����Ă��� Rfiles �ɑ΂��Ă� MakeTable ���Ă����Ȃ��Ă͂����Ȃ��ꍇ��
	//true�ɂȂ�t���O�ł��B
	bool				MostMakeRTable;

};

#endif // !defined(AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_)
