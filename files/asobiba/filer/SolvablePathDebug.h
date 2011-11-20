// SolvablePathDebug.h: SolvablePathDebug �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_)
#define AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvablePathDebug;

#include "comm.h"


#include "SolvablePath.h"

class SolvablePathDebug : public SolvablePath  
{
public:
	SolvablePathDebug(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathDebug()
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
		case __DISPLAYTYPE_DEBUG:
			return true;
		}
		return false;
	}
	//�ł���������f�B�X�v���C�́A���ł���?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		return __DISPLAYTYPE_DEBUG;
	}
	//�ł���������f�B�X�v���C�ł̃J�����������Ă�������
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_NAME , "�t�@�C����" , 0),
			RFixedColumn(__FILECOLUMN_TYPE_EXT , "���", 0),
			RFixedColumn(__FILECOLUMN_TYPE_END , "", 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//�Ή����� Rfiles �̍쐬.
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const
	{
		return NULL;
	}
	//��f�B���N�g�����オ��
	virtual string UpDirectory() const 
	{
		return "";
	}


};

#endif // !defined(AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_)
