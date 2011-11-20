// RGISSmallKoudo2.h: RGISSmallKoudo2 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLKOUDO2_H__FE3A70B8_1D2E_402A_9B3F_97618EDF0745__INCLUDED_)
#define AFX_RGISSMALLKOUDO2_H__FE3A70B8_1D2E_402A_9B3F_97618EDF0745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"

class RGISSmallKoudo2
{
	friend class RGISAllTest;	//�e�X�g�N���X�������̒���������܂킹��悤��.
public:
	RGISSmallKoudo2();
	virtual ~RGISSmallKoudo2();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//��/�_�̕`��
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//�h��Ԃ�
	void Draw2(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
private:

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLKOUDO2_H__FE3A70B8_1D2E_402A_9B3F_97618EDF0745__INCLUDED_)
