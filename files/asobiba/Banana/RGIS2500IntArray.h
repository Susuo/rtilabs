// RGIS2500IntArray.h: RGIS2500IntArray �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_)
#define AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"
#include "RGIS2500Head.h"

//�A�[�N�̍ő�l �A�[�N�́A 4096 �ȏ�̗v�f��ێ����Ȃ�
const int MAX_ARC_BUFFER	=	4096;
//�A�[�N�̈�v�f�̒����̍ő�l
const int MAX_ARC_STRING_BUFFER	=	256;

//�z���S���̍ő�l �A�[�N�́A 4096 �ȏ�̗v�f��ێ����Ȃ�
const int MAX_PRG_BUFFER	=	4096;
//�z���S���̈�v�f�̒����̍ő�l
const int MAX_PRG_STRING_BUFFER	=	256;

class RGIS2500PointArray  
{
public:
	RGIS2500PointArray();
	virtual ~RGIS2500PointArray();

	void Create(int inMax , const char**  inStrArray , const RGIS2500Head* inHead) throw(RParseException);

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	UPOINT getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
	void ConvertDisplayScal(int inShift) throw(RException);

private:
	unsigned int	Count;
	UPOINT*			Points;
};

class RGIS2500IntArray  
{
public:
	RGIS2500IntArray();
	virtual ~RGIS2500IntArray();

	void Create(int inMax , const char ** inStrArray) throw(RParseException);

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	int getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}

private:
	unsigned int	Count;
	int*			Points;
};

#endif // !defined(AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_)
