// RGISKoudo2.h: RGISKoudo2 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISKOUDO2_H__B5D61B54_68F3_4424_9FB0_9B8613E11C67__INCLUDED_)
#define AFX_RGISKOUDO2_H__B5D61B54_68F3_4424_9FB0_9B8613E11C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"


struct KoudoData2
{
	unsigned short	Level;
	int				Number;
	bool			Use;
};


class RGISKoudo2
{
public:
	RGISKoudo2();
	virtual ~RGISKoudo2();

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
	const KoudoData2* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw) const;

	static void test();
private:
	//�W�������x���ɕϊ�.
	unsigned char selectKoudoLevel(int inKM);
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	KoudoData2*	Points;

	RGISSlp		MeshSlp;

};

#endif // !defined(AFX_RGISKOUDO2_H__B5D61B54_68F3_4424_9FB0_9B8613E11C67__INCLUDED_)
