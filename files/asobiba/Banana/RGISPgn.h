// RGISPgn.h: RGISPgn �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_)
#define AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS2500IntArray.h"
#include "RGISDraw.h"
#include "RGISArc.h"
#include "RGISAtr.h"
#include "RGISNameDatabase.h"
#include "RGIS2500Info.h"

struct PgnData
{
	int	Type;						//�}�����ރR�[�h
	int No;							//�ʔԍ�
	RGIS2500IntArray	Array;		//���Y�|���S�����\������A�[�N�w�背�R�[�h
	UPOINT	Representative;			//��\�_
};

class RGISPgn  
{
public:
	RGISPgn();
	virtual ~RGISPgn();
	void Create(const string & inFilename, int inScale) throw(RException);
	void Delete();

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const PgnData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw, const RGISArc * inArc) const;

	void RGISPgn::SmallPack(const RGISArc * inArc ,const RGISAtr * inAtr , 
		const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
		RGIS2500InfoVector* gis2500infoVector);

	void RGISPgn::SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector);

	static void test();
private:
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	PgnData*	Points;
	string		Filename;

};

#endif // !defined(AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_)
