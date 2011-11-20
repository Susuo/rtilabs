// RGISSmallChimei.h: RGISSmallChimei �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLCHIMEI_H__409B7924_2E78_4760_A031_BCB6CEE92DCC__INCLUDED_)
#define AFX_RGISSMALLCHIMEI_H__409B7924_2E78_4760_A031_BCB6CEE92DCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"
#include "RGISSmallNameDatabase.h"
#include "RGISSmallNameDatabase.h"

struct SmallChimeiDataOptimizMainData
{
	unsigned char			SR;
	unsigned char			LoopCount;
};
struct SmallChimeiDataOptimizUniqData
{
	unsigned short			NameIndex;
	SPOINT					xy;
};


class RGISSmallChimei  
{
	friend class RGISAllTest;	//�e�X�g�N���X�������̒���������܂킹��悤��.
public:
	RGISSmallChimei();
	virtual ~RGISSmallChimei();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw , const RGISSmallNameDatabase * inName);

private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	//�f�[�^�̓ǂݍ���.
	void RGISSmallChimei::readDataMain(RStdioMemory * ioFile, SmallChimeiDataOptimizMainData * outData) const;
	//�f�[�^�̓ǂݍ���.
	void RGISSmallChimei::readDataUniq(RStdioMemory * ioFile, SmallChimeiDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLCHIMEI_H__409B7924_2E78_4760_A031_BCB6CEE92DCC__INCLUDED_)
