// RGIS.h: RGIS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS_H__44D1F4BD_041D_4F31_8D57_9A27057E1759__INCLUDED_)
#define AFX_RGIS_H__44D1F4BD_041D_4F31_8D57_9A27057E1759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "RGIS25000.h"
#include "RGIS2500.h"
#include "RGISAddressMng.h"
#include "RGISDraw.h"
#include "RGIS2500Info.h"



class RGIS  
{
public:
	RGIS();
	virtual ~RGIS();

	void SmallPack(const string inPath,const string inTo,int inKey,int inScale);

	//�����X�N���v�g�̎擾.
	static int getJoinScript(JoinTempFormatScript * ioScripts , int inSize) ;
	//�����X�N���v�g�̎擾.
	static int getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) ;

	bool IsHit(const URECT *inDisplay)
	{
		return Gis25000.Slm.IsHit(inDisplay);
	}

	int getKey() const
	{
		return Key;
	}
private:
	//Key�𕶎���Ƃ��ĕԂ��܂�. Key�͕K�� 5���̕�����ɂȂ�.
	string KeyToString(int inKey) const;

	RGIS25000			Gis25000;
	RGIS2500			Gis2500;
	RGISAddressMng		AddressMng;
	RGISNameDatabase	NameDatabase;

	int				Key;

	MessageGIS2500Info	Gis2500Info;

};

#endif // !defined(AFX_RGIS_H__44D1F4BD_041D_4F31_8D57_9A27057E1759__INCLUDED_)
