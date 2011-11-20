// SolvableFileLZH.h: SolvableFileLZH �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_)
#define AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "SolvableFile.h"
#include "RfilesLocal.h"

#include "RfileLocalReport.h"
#include "RfileArcBigIcon.h"
#include "RfilesLzh.h"
#include "RfilesWithCache.h"


class SolvableFileLZH : public SolvableFile  
{
public:
	SolvableFileLZH()
	{
	}
	virtual ~SolvableFileLZH()
	{
	}

	//���̃p�X���������Ă݂�
	//�����ł����ꍇ�͉����������ʂ̃p�X�� �����o�� setPath �ŃZ�b�g���邱��.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ �ȂǂƂȂ�
	//�����ł����ꍇ�� true �����ł��Ȃ������ꍇ�� false
	virtual bool DoSolvable(string inPath) ;
	//��ނ�Ԃ�.
	virtual __RFILETYPE getType() const
	{
		return __RFILETYPE_LOCALFILE_REPORT;
	}
	//���̃f�B�X�v���C�̓T�|�[�g���Ă܂���?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const
	{
		switch(inType)
		{
		case __DISPLAYTYPE_LISTVIEW_REPORT:			//�ڍ�
		case __DISPLAYTYPE_LISTVIEW_BIGICON:		//�傫�ȃA�C�R��
			return true;
		}
		return false;
	}
	//�ł���������f�B�X�v���C�́A���ł���?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		//return __DISPLAYTYPE_LISTVIEW_REPORT;
		return __DISPLAYTYPE_LISTVIEW_BIGICON;
	}
	//�ł���������f�B�X�v���C�ł̃J�����������Ă�������
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_ICON , "" , 0),
			RFixedColumn(__FILECOLUMN_TYPE_NAME , "�t�@�C����" , 0.50),
			RFixedColumn(__FILECOLUMN_TYPE_EXT , "���" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_SIZE , "�T�C�Y" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_DATE , "�X�V����" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_ATTRIB , "����",0.5),
			RFixedColumn(__FILECOLUMN_TYPE_END , "" , 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//�t�@�C���擾
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const throw()
	{
		switch(inType)
		{
//		case __DISPLAYTYPE_LISTVIEW_REPORT:
//			return new RfilesLzh<RfileLzhReport>( getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			{
				ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
				acff->ArchiveType = __ARCHIVETYPE_LZH;
				return new RfilesWithCache(acff, __RFILETYPE_ARC_BIGICON , getPath() );
			}
			//return new RfilesLzh( __ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON , getPath() );
		}
		throw RNotSupportException("�w�肳�ꂽ�f�B�X�v���C %d �́A�T�|�[�g�O�ł�" , inType);
		return NULL;
	}

	static void Test();

};

#endif // !defined(AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_)
