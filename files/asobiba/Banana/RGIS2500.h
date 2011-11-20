// RGIS2500.h: RGIS2500 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_)
#define AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "RDrawObject.h"
#include "RGISDraw.h"
#include "RGISPgn.h"
#include "RGISArc.h"
#include "RGISAtr.h"
#include "RGISPnt.h"
#include "RGISJoin.h"
#include "RGISNameDatabase.h"
#include "RGIS2500Info.h"

struct RGIS2500AreaPgn
{
	RGISArc	Arc;
	RGISPgn	Pgn;
	RGISAtr Atr;
	RGISPnt Pnt;
};


struct RGIS2500OneArea
{
	RGIS2500AreaPgn			Tatemono;		//����
	RGIS2500AreaPgn			Zyouti;			//���傤��? ����/�w�Z�̕~�n...
	RGIS2500AreaPgn			Eki;			//�w
};


class RGIS2500  
{
public:
	RGIS2500();
	virtual ~RGIS2500();

	void SmallPack(const string inPath,const string inTo,const string inNumString, 
		MessageGIS2500Info * gis2500Info , RGISNameDatabase * ioNameDatabase, int inScale);
	//�����X�N���v�g�̎擾.
	int RGIS2500::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const;
	//�����X�N���v�g�̎擾.
	int RGIS2500::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const;

	int getKey() const
	{
		return Key;
	}
private:
	void Delete();
	//01100 ���̃f�B���N�g���̒��ɕ��f����ē����Ă���G���A�̐����擾���܂�.
	int getAreaCount(const string & inPath ) const throw(RNotFoundException , RException) ;
	//���ۂɓǂݍ���
	void Read(const string & inPath , int inScale) throw(RException);
	//�ЂƂ̃G���A��ǂݍ���Ō��܂��B
	void ReadInPart(RGIS2500OneArea* ioArea , const string & inPath , int inScale) throw(RException);

	//Key�𕶎���Ƃ��ĕԂ��܂�. Key�͕K�� 5���̕�����ɂȂ�.
	string KeyToString(int inKey) const;

	JoinTempFormatScript RGIS2500::PgnJoinScript(const string & inDirectoryName , unsigned char inBlockStart) const;

	int					Count;
	RGIS2500OneArea*	Points;

	int				Key;
};

#endif // !defined(AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_)
