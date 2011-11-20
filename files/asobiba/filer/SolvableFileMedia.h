// SolvableFileMedia.h: SolvableFileMedia �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILEMEDIA_H__8F0A2E5F_DBC4_4680_922D_FDD410DFDB24__INCLUDED_)
#define AFX_SOLVABLEFILEMEDIA_H__8F0A2E5F_DBC4_4680_922D_FDD410DFDB24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SolvableFile.h"
#include "RfilesPreviewOne.h"

class SolvableFileMedia : public SolvableFile  
{
public:
	SolvableFileMedia()
	{
	}
	virtual ~SolvableFileMedia()
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
		return __RFILETYPE_PREVIEWONE;
	}
	//���̃f�B�X�v���C�̓T�|�[�g���Ă܂���?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const
	{
		switch(inType)
		{
		case __DISPLAYTYPE_PREVIEW_MEDIA:			//���f�B�A.
			return true;
		}
		return false;
	}
	//�ł���������f�B�X�v���C�́A���ł���?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		return __DISPLAYTYPE_PREVIEW_MEDIA;
	}
	//�ł���������f�B�X�v���C�ł̃J�����������Ă�������
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_END , "" , 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//�t�@�C���擾
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const throw()
	{
		switch(inType)
		{
		case __DISPLAYTYPE_PREVIEW_MEDIA:
			return new RfilesPreviewOne( getPath() );
		}
		throw RNotSupportException("�w�肳�ꂽ�f�B�X�v���C %d �́A�T�|�[�g�O�ł�" , inType);
		return NULL;
	}


};

#endif // !defined(AFX_SOLVABLEFILEMEDIA_H__8F0A2E5F_DBC4_4680_922D_FDD410DFDB24__INCLUDED_)
