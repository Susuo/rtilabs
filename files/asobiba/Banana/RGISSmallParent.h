// RGISSmallParent.h: RGISSmallParent �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_)
#define AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISSmallDouro.h"
#include "RGISSmallTetudo.h"
#include "RGISSmallChimei.h"
#include "RGISSmallKawa.h"
#include "RGISSmallKoukyou.h"
#include "RGISSmallSuiikiKai.h"
#include "RGISSmallDouro.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallNameDatabase.h"
#include "RGISSmallAddress.h"
#include "RGISSmallKoudo2.h"
#include "RGISSmallUserData.h"
#include "RGISSmallParentImp.h"
#include "RGISSmallFindObject.h"
#include "RGISSmallTatemono.h"
#include "RGISSmallZyouti.h"
#include "RGISAllTest.h"

class RGISSmallParent  : public RGISSmallParentImp
{
	friend class RGISAllTest;	//�e�X�g�N���X�������̒���������܂킹��悤��.
public:
	RGISSmallParent();
	virtual ~RGISSmallParent();

	//�V���{���̉���
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//�f�[�^�̃Z�b�g
	virtual void setData(unsigned long inBlock ,char* ioBuffer , char * ioUserDataBuffer) ;
	//���[�U�[�f�[�^�̃����[�h.
	virtual void UserDataReload(char* ioUserDataBuffer) ;

	//��������������O���[�v�����߂�.
	unsigned long getBlock() const
	{
		return Block;
	}
	//�`��
	void Draw(RGISSmallDraw *inDraw ,const RECT & inClip);

	//�w������
	void FindEki(FindPool * outFindData , const string &inName , unsigned short inNameHash) const
	{
		Tetudo.FindEki(&BlockStart , outFindData , inName , inNameHash , &NameData);
	}
	//�Z������
	void FindAddress(FindPool * outFindData , const string &inName , int inBanchi) const
	{
		Address.FindAddress(&BlockStart , outFindData , inName , inBanchi , &AddressName);
	}
	//���[�U�f�[�^����
	int FindUserData(RGISSmallUserDataRead * outFindData ,int inDistance ,  const LRECT * inSeartchArea) const
	{
		return UserData.FindUserData(&BlockStart , outFindData ,inDistance, inSeartchArea);
	}

private:

private:
	unsigned long		Block;
	//�n�}�f�[�^.
	char*				Data;
	//���[�U��`�f�[�^.
	char*				UserDataBuffer;

	LPOINT				BlockStart;

	RGISSmallDouro		Douro;
	RGISSmallTetudo		Tetudo;
	RGISSmallChimei		Chimei;
	RGISSmallKawa		Kawa;
	RGISSmallKoukyou	Koukyou;
	RGISSmallSuiikiKai	SuiikiKai;
	RGISSmallNameDatabase	NameData;
	RGISSmallAddress		Address;
	RGISSmallNameDatabase	AddressName;
	RGISSmallKoudo2			Koudo;
	RGISSmallUserData		UserData;

	RGISSmallTatemono		Tatemono;
	RGISSmallZyouti			Zyouti;
};

#endif // !defined(AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_)
