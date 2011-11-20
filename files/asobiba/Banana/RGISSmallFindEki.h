// RGISSmallFindEki.h: RGISSmallFindEki �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFINDEKI_H__C5137E86_C0C5_4F2F_B003_9E8A2DC50225__INCLUDED_)
#define AFX_RGISSMALLFINDEKI_H__C5137E86_C0C5_4F2F_B003_9E8A2DC50225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallHeaderImp.h"
#include "RGISSmallIntelligentCache.h"
#include "RGISSmallFindObject.h"


class RGISSmallFindEki : public RGISSmallHeaderImp  
{
public:
	RGISSmallFindEki();
	virtual ~RGISSmallFindEki();


	void Delete();
	void Create(const string & inName, RGISSmallIntelligentCache* ioCache) throw(RException) ;
	//�V���{���̉���
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) ;
	//�V���{���̎擾.
	virtual unsigned char getSimbol() 
	{
		return 'E';
	}

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return FoundData.size();
	}
	//�n�_�̎擾
	const RGISSmallFindObject* getPoint(unsigned int p) const
	{
		ASSERT(p < FoundData.size() );
		return FoundData[p];
	}
private:
	string	Name;
	int		NameSize;
	char NameShortCut[NAME_SHORT_CUT_SIZE];
	RGISSmallIntelligentCache* Cache;

	FindPool		FoundData;
};

#endif // !defined(AFX_RGISSMALLFINDEKI_H__C5137E86_C0C5_4F2F_B003_9E8A2DC50225__INCLUDED_)
