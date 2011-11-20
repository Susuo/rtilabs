// RGISEki.h: RGISEki �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISEKI_H__031544F8_1C13_49FE_BA90_613F73F5B6A9__INCLUDED_)
#define AFX_RGISEKI_H__031544F8_1C13_49FE_BA90_613F73F5B6A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RDrawObject.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"


struct EkiData
{
	unsigned short			NameIndex;
	bool					Use;
};


class RGISEki
{
public:
	RGISEki();
	virtual ~RGISEki();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();

	void SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException);
	
	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const EkiData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	static void test();
private:

private:
	unsigned int			Count;
	EkiData*	Points;

};
#endif // !defined(AFX_RGISEKI_H__031544F8_1C13_49FE_BA90_613F73F5B6A9__INCLUDED_)
