// RGISSmallHeaderImp.h: RGISSmallHeaderImp �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLHEADERIMP_H__F73FA48F_7BFE_4D64_8A6D_3B4E540A8196__INCLUDED_)
#define AFX_RGISSMALLHEADERIMP_H__F73FA48F_7BFE_4D64_8A6D_3B4E540A8196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISSmallFindObject.h"

class RGISSmallHeaderImp  
{
public:
	RGISSmallHeaderImp();
	virtual ~RGISSmallHeaderImp();

	//�V���{���̉���
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) = 0;

	//�V���{���̎擾.
	virtual unsigned char getSimbol() = 0;
};

#endif // !defined(AFX_RGISSMALLHEADERIMP_H__F73FA48F_7BFE_4D64_8A6D_3B4E540A8196__INCLUDED_)
