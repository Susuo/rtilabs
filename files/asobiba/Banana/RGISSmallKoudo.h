// RGISSmallKoudo.h: RGISSmallKoudo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLKOUDO_H__64951FB0_C1D4_4A71_8A91_CDEC65DDF919__INCLUDED_)
#define AFX_RGISSMALLKOUDO_H__64951FB0_C1D4_4A71_8A91_CDEC65DDF919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"

struct SmallKoudoData
{
	unsigned char			Level;
	RGISSmallType			CV;
};

class RGISSmallKoudo  
{
	friend class RGISAllTest;	//�e�X�g�N���X�������̒���������܂킹��悤��.
public:
	RGISSmallKoudo();
	virtual ~RGISSmallKoudo();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,char* ioBuffer) throw(RException);
	//��/�_�̕`��
	void Draw1(const RGISSmallDraw *inDraw);
	//�h��Ԃ�
	void Draw2(const RGISSmallDraw *inDraw);

	void Delete();
private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	COLORREF selectSize(unsigned char inShubetsu) const;

private:
	SmallKoudoData*		Points;
	int					Count;

};

#endif // !defined(AFX_RGISSMALLKOUDO_H__64951FB0_C1D4_4A71_8A91_CDEC65DDF919__INCLUDED_)
