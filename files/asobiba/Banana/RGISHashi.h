// RGISHashi.h: RGISHashi �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISHASHI_H__ACBCA570_322F_479A_9395_39CF225FCBF3__INCLUDED_)
#define AFX_RGISHASHI_H__ACBCA570_322F_479A_9395_39CF225FCBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RDrawObject.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"


struct HashiData
{
	unsigned short			NameIndex;
	bool					Use;
};


class RGISHashi  
{
public:
	RGISHashi();
	virtual ~RGISHashi();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();

	void SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException);
	
	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const HashiData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	static void test();
private:

private:
	unsigned int			Count;
	HashiData*	Points;

};

#endif // !defined(AFX_RGISHASHI_H__ACBCA570_322F_479A_9395_39CF225FCBF3__INCLUDED_)
