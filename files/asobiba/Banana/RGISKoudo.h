// RGISKoudo.h: RGISKoudo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISKOUDO_H__F6F7D50A_FE80_49C6_A25F_40F46F801242__INCLUDED_)
#define AFX_RGISKOUDO_H__F6F7D50A_FE80_49C6_A25F_40F46F801242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"


struct KoudoData
{
	unsigned short Level;
	GISINTArray	CV;
};


class RGISKoudo
{
public:
	RGISKoudo();
	virtual ~RGISKoudo();

	void Create(const string & inSlpFilename,const string & inMHFilename,const RGISSlm * inMeshSlm) throw(RException);
	void Delete();
	void SmallPack(const string & inDir)  const throw(RException);
	JoinTempFormatScript getPackInfo() const;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const KoudoData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void RGISKoudo::Draw(const RGISDraw * inDraw) const;

	static void test();
private:
	//�W�������x���ɕϊ�.
	unsigned char selectKoudoLevel(int inKM);
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	KoudoData*	Points;

	RGISSlp		MeshSlp;

};

#endif // !defined(AFX_RGISKOUDO_H__F6F7D50A_FE80_49C6_A25F_40F46F801242__INCLUDED_)
