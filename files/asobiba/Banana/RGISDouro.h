// RGISDouro.h: RGISDouro �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISDOURO_H__44FF28FB_A504_43D7_8630_5DBA5DFB30D1__INCLUDED_)
#define AFX_RGISDOURO_H__44FF28FB_A504_43D7_8630_5DBA5DFB30D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISHashi.h"
#include "RGISTonneru.h"
#include "RGISJoin.h"
#include "RGISNode.h"
#include "RGISGyousei.h"

struct DouroData
{
	GISType		JT;
	GISType		YU;
	GISType		SB;
	GISType		FI;
	unsigned short			TR;
	unsigned short			HR;
	unsigned short			NameIndex;
	GISINTArray	CV;
	RGISNodeIntArray	Node;
	//�ȉ�����!
	bool					Use;
};


class RGISDouro  
{
public:
	RGISDouro();
	virtual ~RGISDouro();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();
	void SmallPack(const string & inDir,const RGISSlp * inSlp,
			  const RGISNameDatabase* inNameData,
			  const RGISGyousei * inGyousei,const RGISNode * inNode,
			  const RGISHashi * inHashi,const RGISTonneru * inTonneru)  const throw(RException);

	JoinTempFormatScript getPackInfo() const;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const DouroData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw) const;

	static void test();

private:
	unsigned int			Count;
	DouroData*	Points;
};

#endif // !defined(AFX_RGISDOURO_H__44FF28FB_A504_43D7_8630_5DBA5DFB30D1__INCLUDED_)
