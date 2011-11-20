// RGISSlm.h: RGISSlm �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_)
#define AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISKeiidoData.h"

//�����A�ܓx�A�o�x�������ł���킵�Ă������Ǝv�����ǁA
//�l���ł������� ulong �ł����܂�Ȃ��̂ŁA�k�߂��݂܂��B
//�܂��A���{�����\���ł���΂��Ƃ͂ǂ��ł������̂ŁA���������Ȃ���?(w
const __int64  GLOBAL_X_SCAL	=	5000000000;
const __int64  GLOBAL_Y_SCAL	=	 800000000;


class RGISMathZantei
{
public:
	RGISMathZantei()
	{
		this->Scale = 0;
	}
	void Create(int inScale)
	{
		ASSERT(inScale >= 100);
		this->Scale = inScale;
	}

	//25000��ԃf�[�^�n�}�̍��W�n�� 
	//ulong �Ɏ��܂�悤�� /100 ���� 10cm �̐��x�ɗ��Ƃ��܂��B
	//�i�r�P�[�V�����Ȃ̂ŕʂɖ��Ȃ��͂�
	UPOINT ConvertScale(unsigned __int64 inX,unsigned __int64 inY) const;
	//25000 ��ԃf�[�^���o�ܓx�ɕϊ�.
	static  void Kukan25000ToKeiido(__int64 inX, __int64 inY , RGISKeiidoData * ioToukei , RGISKeiidoData * ioHokui) ;
	//��̋t�ϊ�.
	static  UPOINT RevKukan25000ToKeiido(const RGISKeiidoData * inToukei ,const RGISKeiidoData * inHokui) ;
	static  unsigned long ConvertDisplayScalX(unsigned long ioX) 
	{
		return (ioX - 0) ;
	}
	static unsigned  long ConvertDisplayScalY(unsigned long ioY) 
	{
		return (0xffffffff - (((ioY) - 0)  ));
	}

private:
	int		Scale;
};

class RGISSlm  
{
public:
	RGISSlm();
	virtual ~RGISSlm();

	void Create(const string & inFilename,int inScale) throw(RException);

	//�O���[�o�����W�n�ɕϊ����܂��B
	UPOINT ConvertGlobal(long x , long y) const
	{
		RGISMathZantei zantei;
		zantei.Create(this->Scale);

		return zantei.ConvertScale(X + x , Y + y);
	}
	//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
	bool IsHit(const URECT * inDisplay) const;

	int getKey() const
	{
		return Key;
	}


	//25000��ԃf�[�^�n�}�̍��W�n���� UTM �ɕϊ����܂��B
	//�܂��A���̂Ƃ� ulong �Ɏ��܂�悤�� /100 ���� 10cm �̐��x�ɗ��Ƃ��܂��B
	//�i�r�P�[�V�����Ȃ̂ŕʂɖ��Ȃ��͂�
//	static UPOINT RGISSlm::Convert25000ToUTM(unsigned __int64 inX,unsigned __int64 inY);

	static void test();


private:
	//�t�@�C�����͕K�� 01234.slm �̂悤�Ȑ����Ȃ�̂ł�����擾.
	int ParseKey(const string & inFilename) const;
private:
	//���y�n���@�̎����W
	__int64	X;
	__int64	Y;
	//�f�B�X�v���C�ɕ\������Ƃ��̍��W�ɕϊ���������.
	int		Key;

	int		Scale;
};

#endif // !defined(AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_)
