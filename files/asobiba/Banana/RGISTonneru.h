// RGISTonneru.h: RGISTonneru �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISTONNERU_H__964FC1FA_A415_4BE3_9883_1E1845B81078__INCLUDED_)
#define AFX_RGISTONNERU_H__964FC1FA_A415_4BE3_9883_1E1845B81078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RDrawObject.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"


struct TonneruData
{
	unsigned short			NameIndex;

	bool					Use;
};

class RGISTonneru  
{
public:
	RGISTonneru();
	virtual ~RGISTonneru();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();

	void SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException);
	
	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const TonneruData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	static void test();
private:

private:
	unsigned int			Count;
	TonneruData*	Points;

};

#endif // !defined(AFX_RGISTONNERU_H__964FC1FA_A415_4BE3_9883_1E1845B81078__INCLUDED_)
