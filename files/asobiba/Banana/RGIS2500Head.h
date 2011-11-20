// RGIS2500Head.h: RGIS2500Head �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_)
#define AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"



class RGIS2500Head  
{
public:
	RGIS2500Head();
	virtual ~RGIS2500Head();

	void Create(const char * inStr , int inScale) throw(RException);

	unsigned int getCount() const
	{
		return Count;
	}

	UPOINT convertXY(double x,double y) const;

	static void test();

private:
	//�}��
	char Zumai[128];
	//�t�@�C���� 
	char Filename[256];
	//���R�[�h����
	int Record;
	//�v�f�̑���
	int Count;
	//���W�n�t���O
	int ZahyoFlg;
	//���W�n�ԍ�
	int ZahyoNumber;
	//���l���敪
	int Kubun;

	DoublePoint UpperLeft;
	DoublePoint LowerLeft;
	DoublePoint UpperRight;
	DoublePoint LowerRight;

	int			Scale;
};

#endif // !defined(AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_)
