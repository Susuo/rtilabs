// RGISKawa.h: RGISKawa �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISKawa_H__6CD1EA16_601E_4FFA_9E79_D50D821ED240__INCLUDED_)
#define AFX_RGISKawa_H__6CD1EA16_601E_4FFA_9E79_D50D821ED240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"


struct KawaData
{
	GISType		SB;
	GISType		FI;
	unsigned short			NameIndex;
	GISINTArray	CV;
	//�ȉ�����!

	bool		Use;
};


class RGISKawa  
{
public:
	RGISKawa();
	virtual ~RGISKawa();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();
	void SmallPack(const string & inDir,const RGISSlp * inSlp,
							  const RGISNameDatabase* inNameData)  const throw(RException);
	JoinTempFormatScript getPackInfo() const;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const KawaData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void RGISKawa::Draw(const RGISDraw * inDraw) const;

	static void test();

private:
	unsigned int			Count;
	KawaData*	Points;

};

#endif // !defined(AFX_RGISKawa_H__6CD1EA16_601E_4FFA_9E79_D50D821ED240__INCLUDED_)
