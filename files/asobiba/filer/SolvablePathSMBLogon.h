// SolvablePathSMBLogon.h: SolvablePathSMBLogon �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_)
#define AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "SolvablePath.h"
#include "RfilesSMBLogon.h"


class SolvablePathSMBLogon : public SolvablePath  
{
public:
	SolvablePathSMBLogon(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathSMBLogon()
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
		return __RFILETYPE_SMBLOGON_BIGICON;
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
			RFixedColumn(__FILECOLUMN_TYPE_COMMENT , "�R�����g" , 0.50),
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
			return new RfilesSMBLogon(__RFILETYPE_SMBLOGON_REPORT, getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			return new RfilesSMBLogon(__RFILETYPE_SMBLOGON_BIGICON, getPath() );
		}
		throw RNotSupportException("�w�肳�ꂽ�f�B�X�v���C %d �́A�T�|�[�g�O�ł�" , inType);
		return NULL;
	}
	//��f�B���N�g�����オ��
	virtual string UpDirectory() const ;

	static void Test();

};

#endif // !defined(AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_)
