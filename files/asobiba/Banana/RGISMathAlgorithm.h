// RGISMathAlgorithm.h: RGISMathAlgorithm �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_)
#define AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//���̃N���X�́A
//http://homepage3.nifty.com/Nowral/
//�̃\�[�X�R�[�h���Q�l�ɏ�����Ă��܂��B
//
//�Q�l�ɂ����\�[�X�� GPL �Ȃ̂ŁA
//���̃\�[�X�R�[�h�� GPL �ɂȂ�܂��B

class RGISMathAlgorithm  
{
public:
	RGISMathAlgorithm(double a , double f);
	virtual ~RGISMathAlgorithm();
public:
	// Gauss-Klueger
	DoublePoint RGISMathAlgorithm::ll2xy(double b,double dl) const;
	// �ԓ�����ܓxB�܂ł̎q�ߐ��ʒ�
	double RGISMathAlgorithm::lom(double b) const;
	//???
	double RGISMathAlgorithm::eta2(double b) const;
	//�K�ѐ��ȗ����a
	double RGISMathAlgorithm::bouyuu(double eta2) const;

	// �ȉ~�̍��W -> �������W
	Double3Point RGISMathAlgorithm::llh2xyz(double b,double l,double h) const;
	// �������W -> �ȉ~�̍��W
	Double3Point RGISMathAlgorithm::xyz2llh(double x,double y,double z) const;

double getED2() const
{
	return this->ED2;
}
double getCAD() const
{
	return this->CAD;
}

private:
	double PI;
	double RD;

	double A;
	double F;

	// ��1���S��
	double E2;
	// ��2���S��
	double ED2;

	double CAD;
	double CBD;
	double CCD;
	double CDD;
	double CED;

};

#endif // !defined(AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_)
