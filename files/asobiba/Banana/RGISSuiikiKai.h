// RGISSuiikiKai.h: RGISSuiikiKai �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSUIIKIKAI_H__DF7FE4A9_6A94_4259_97F8_24B792D38CE4__INCLUDED_)
#define AFX_RGISSUIIKIKAI_H__DF7FE4A9_6A94_4259_97F8_24B792D38CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"


struct SuiikiKaiData
{
	GISType		SR;
	GISINTArray	CV;
	//�ȉ�����!
	bool					Use;
};


class RGISSuiikiKai
{
public:
	RGISSuiikiKai();
	virtual ~RGISSuiikiKai();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();
	void SmallPack(const string & inDir,const RGISSlp * inSlp,
						  const RGISNameDatabase* inNameData)  const throw(RException);
	JoinTempFormatScript getPackInfo() const;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const SuiikiKaiData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void RGISSuiikiKai::Draw(const RGISDraw * inDraw) const;

	static void test();
private:
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	SuiikiKaiData*	Points;

};
#endif // !defined(AFX_RGISSUIIKIKAI_H__DF7FE4A9_6A94_4259_97F8_24B792D38CE4__INCLUDED_)
