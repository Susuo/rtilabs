// SolvablePathLocal.h: SolvablePathLocal �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_)
#define AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_

class SolvablePathLocal;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "SolvablePath.h"
#include "RfilesLocal.h"

#include "RfileLocalReport.h"
#include "RfileLocalBigIcon.h"

class SolvablePathLocal : public SolvablePath 
{
public:
	SolvablePathLocal(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathLocal()
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
		case __DISPLAYTYPE_LISTVIEW_REPORT:
			return new RfilesLocal(__RFILETYPE_LOCALFILE_REPORT, getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			return new RfilesLocal(__RFILETYPE_LOCALFILE_BIGICON, getPath() );
		}
		throw RNotSupportException("�w�肳�ꂽ�f�B�X�v���C %d �́A�T�|�[�g�O�ł�" , inType);
		return NULL;
	}
	//��f�B���N�g�����オ��
	virtual string UpDirectory() const ;

	static void Test();
};

#endif // !defined(AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_)
