// RGISSlp.h: RGISSlp �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_)
#define AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISSlm.h"
#include "GISType.h"

//#define CLIPX(x)	((x-83260000)/4096)
//#define CLIPY(x)	(500-((x-547900000)/4096))
#define CLIPX(x)	(((x)-83260000)/1024)
#define CLIPY(x)	(500-(((x)-747900000)/1024))


class RGISSlp  
{
public:
	RGISSlp();
	virtual ~RGISSlp();

	void Create(const string & inFilename,const RGISSlm * inSlm) throw(RException);
	//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
	void ConvertDisplayScal() throw(RException);

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	UPOINT getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);

		return Points[p];
	}
	//�ő�ŏ��̏ꏊ�̎擾.
	LRECT getRect() const;

	bool TestCV(GISINTArray* inArray);


	static void test();
private:
	unsigned int			Count;
	UPOINT	*	Points;

};

#endif // !defined(AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_)
