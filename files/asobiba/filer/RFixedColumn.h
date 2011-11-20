// RFixedColumn.h: RFixedColumn �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_)
#define AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RFixedColumn ;

#include "comm.h"


enum __FILECOLUMN_TYPE
{
	__FILECOLUMN_TYPE_MARK,			//�}�[�N
	__FILECOLUMN_TYPE_ICON,			//�A�C�R��
	__FILECOLUMN_TYPE_NAME,			//���O
	__FILECOLUMN_TYPE_VIRTUALPATH,	//���z�p�X(���k�t�@�C���Ȃǂ́@���k�t�@�C���̐擪�� / �Ƃ���p�X)
	__FILECOLUMN_TYPE_EXT,			//�^�C�v(�g���q�Ɋ֘A�t�����Ă��閼�O)
	__FILECOLUMN_TYPE_SIZE,			//�T�C�Y
	__FILECOLUMN_TYPE_DATE,			//�X�V���t
	__FILECOLUMN_TYPE_ATTRIB,		//����
	__FILECOLUMN_TYPE_FREE,			//�󂫗̈�
	__FILECOLUMN_TYPE_COMMENT,		//�R�����g
	__FILECOLUMN_TYPE_ARCMODE,		//���k���[�h
	__FILECOLUMN_TYPE_RATIO,		//���k���[�g(���k��)
	__FILECOLUMN_TYPE_END,			//�ԕ�.

};


class RFixedColumn  
{
public:
	RFixedColumn()
	{
		Type = __FILECOLUMN_TYPE_END;
		Name = "";
		WidthParsent = 0;
	}
	RFixedColumn(__FILECOLUMN_TYPE inType,string inName,double inWidthParsent)
	{
		Type = inType;
		Name = inName;
		WidthParsent = inWidthParsent;
	}
	virtual ~RFixedColumn()
	{
	}

	__FILECOLUMN_TYPE getType()	const {	return Type;	}
	string getName()	const {	return Name;	}
	double getWidthParsent()	const {	return WidthParsent;	}

private:
	//���
	__FILECOLUMN_TYPE	Type ;
	//���O
	string				Name;
	//�� ���̕��� 1 �Ƃ����Ƃ��̏k�ڗ�.
	double				WidthParsent;
};

#endif // !defined(AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_)
