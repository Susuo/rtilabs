// RGISSmallTatemono.h: RGISSmallTatemono �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLTATEMONO_H__08D023FA_6B3A_4AFD_93E0_D1D631CD8AFB__INCLUDED_)
#define AFX_RGISSMALLTATEMONO_H__08D023FA_6B3A_4AFD_93E0_D1D631CD8AFB__INCLUDED_

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
#include "RGISSmallFindObject.h"

struct SmallTatemonoDataOptimizMainData
{
	unsigned short			NameIndex;
	unsigned char			LoopCount;
};
struct SmallTatemonoDataOptimizUniqData
{
	unsigned char			CVCount;
	const char*				CV;
};


class RGISSmallTatemono  
{
public:
	RGISSmallTatemono();
	virtual ~RGISSmallTatemono();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//��/�_�̕`��
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//������̕`��
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);

	//�w����.
	void FindEki(const LPOINT *inBlockStart ,FindPool * outFindData , 
		const string &inFindName , unsigned short inFindNameHash, const RGISSmallNameDatabase * inNameDB ) const;

private:
	//�f�[�^�̓ǂݍ���.
	void readDataMain(RStdioMemory * ioFile, SmallTatemonoDataOptimizMainData * outData) const;
	//�f�[�^�̓ǂݍ���.
	void readDataUniq(RStdioMemory * ioFile, SmallTatemonoDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLTATEMONO_H__08D023FA_6B3A_4AFD_93E0_D1D631CD8AFB__INCLUDED_)
