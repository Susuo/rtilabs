// RfileFactory1.h: RfileFactory �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEFACTORY_H__D8A82C28_EFA6_4E90_AA5B_25D1E86D3355__INCLUDED_)
#define AFX_RFILEFACTORY_H__D8A82C28_EFA6_4E90_AA5B_25D1E86D3355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rfile.h"
#include "RfileLocalReport.h"
#include "RfileLocalBigIcon.h"
#include "RfilePreviewOne.h"
#include "RfileArcBigIcon.h"
#include "RfileNetResourceBigIcon.h"

class RfileFactory  
{
private:
	RfileFactory();
public:
	virtual ~RfileFactory();
public:
	//Rfile �N���X�̍쐬.
	//�T�|�[�g���Ă��Ȃ� inType ��\������� RNotSupportException ����������B
	static Rfile* New(__RFILETYPE inRfileType) throw()
	{
		Rfile* p = NULL;
		switch(inRfileType)
		{
		case __RFILETYPE_LOCALFILE_REPORT:
			p = new RfileLocalReport();
			return p;
		case __RFILETYPE_LOCALFILE_BIGICON:
			p = new RfileLocalBigIcon();
			return p;
		case __RFILETYPE_PREVIEWONE:
			p = new RfilePreviewOne();
			return p;
		case __RFILETYPE_ARC_BIGICON:
			p = new RfileArcBigIcon();
			return p;
		case __RFILETYPE_SMBLOGON_BIGICON:
			p = new RfileNetResourceBigIcon();
			return p;
		}
		throw RNotSupportException("RfileFactory::�w�肳�ꂽ�t�@�C���^�C�v %d �̓T�|�[�g������?",inRfileType);
		return NULL;
	}

	// inSrc �𕡐�����
	//�ł��Ȃ��ꍇ�� RNotSupportException
	static Rfile* Clone(const Rfile* inSrc) throw()
	{
		Rfile* p = NULL;
		switch( inSrc->getType() )
		{
		case __RFILETYPE_LOCALFILE_REPORT:
			p = new RfileLocalReport( *((const RfileLocalReport*)inSrc) );
			return p;
		case __RFILETYPE_LOCALFILE_BIGICON:
			p = new RfileLocalBigIcon( *((const RfileLocalBigIcon*)inSrc) );
			return p;
		case __RFILETYPE_PREVIEWONE:
			p = new RfilePreviewOne( *((const RfilePreviewOne*)inSrc) );
			return p;
		case __RFILETYPE_ARC_BIGICON:
			p = new RfileArcBigIcon( *((const RfileArcBigIcon*)inSrc) );
			return p;
		}
		throw RNotSupportException("RfileFactory::�w�肳�ꂽ�t�@�C���^�C�v %d �͕����ł��܂��񂪉���?",inSrc->getType() );
	}

};

#endif // !defined(AFX_RFILEFACTORY_H__D8A82C28_EFA6_4E90_AA5B_25D1E86D3355__INCLUDED_)
