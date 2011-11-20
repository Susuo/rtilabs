// RGISSlmINdex.h: RGISSlmINdex �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_)
#define AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSlm.h"

typedef vector<RGISSlm*>	RGISSlms;

//���ׂĂ� slm �t�@�C����ǂݍ���.
class RGISSlmINdex  
{
public:
	RGISSlmINdex();
	virtual ~RGISSlmINdex();

	//���ׂĂ� slm �t�@�C����ǂݍ���.
	void Create(const string & inPath ,const RStringList & inTargetList , int inScale) ;
	//���ׂĔj��.
	void Destroy();

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Points.size();
	}
	//�n�_�̎擾
	const RGISSlm* getPoint(unsigned int p) const
	{
		ASSERT(p < getCount() );
		return Points[p];
	}


private:
	RGISSlms	Points;
};

#endif // !defined(AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_)
