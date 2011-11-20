// RGISSmallIntelligentCache.h: RGISSmallIntelligentCache �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_)
#define AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallFileRead.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallParent.h"
#include "RZlib.h"
#include "RGISAllTest.h"
#include "RGISSmallPenFile.h"

//�L���b�V������n��f�[�^�̐�
const int  GIS_SMALL_CACHE_MAX	=	10;


class RGISSmallIntelligentCache  
{
	friend class RGISAllTest;	//�e�X�g�N���X�������̒���������܂킹��悤��.
public:
	RGISSmallIntelligentCache();
	virtual ~RGISSmallIntelligentCache();

	//�쐬
	void Create(const string & inFilename , const string & inUserDataPath) throw(RException);

	void New(RDrawObject  * ioDraw  ,const RGISSmallDrawEffect* inEffect);

	//�w�肳�ꂽ�u���b�N��ǂݍ���
	const RGISSmallParent* Load(unsigned long inBlock);
	//�w�肳�ꂽ�u���b�N���L���b�V���Ƃ͊֌W�Ȃ��ǂݍ���(�L���b�V�����Q�Ƃ��邱�Ƃ͂��Ȃ�)
	bool LoadEx(unsigned long inBlock , RGISSmallParentImp* ioParentImp);

	void Find(RGISSmallHeaderImp* ioFindHeader);

	//���[�U�[�f�[�^�̕ҏW.
	void UserDataModify(const LPOINT * inPoint , const int * inDeleteNumber , int inDeleteCount ,
					    const char * inData , int inSize,int inCount ) throw(RException);
	//���[�U�[�f�[�^�̌���.
	bool UserDataFind(const LRECT * inSearchArea , RGISSmallUserDataRead * outUserData ) throw(RException);


private:
	//�L���b�V���̔j��.
	void Destroy();
	//���͈̔͂Ɋ܂܂�܂���?
	bool isHit(const LRECT * inHitBox , unsigned long inBlock) const;
	//Hit�����Ɏg���₷���悤�ɁA���炩���ߌv�Z���Ă����܂��B
	LRECT ConvertHitBox(const LRECT * inDisplay) const;
	//�L���b�V���Ɏw�肵���L�[�����邩�ǂ�������.
	bool findCache(unsigned long inKey);
	//�u���b�N����f�B�X�v���C���W�ɕϊ�.
	RECT BlockToDisplayRect(const RDrawObject *inDraw ,unsigned long inBlock , const RGISSmallDrawEffect* inEffect) const;
	//�w�肵���u���b�N��ǂݍ��� ��������
	RGISSmallParent* RGISSmallIntelligentCache::LoadPrivate(unsigned long inBlock);

private:
	RZlib	Zlib;

	//�f�[�^
	RGISSmallParent*	Cache[GIS_SMALL_CACHE_MAX];
	//�}�b�v�t�@�C��.
	RGISSmallFileRead	File;
};

#endif // !defined(AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_)
