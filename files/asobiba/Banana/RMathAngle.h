// RMathAngle.h: RMathAngle �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_)
#define AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//1�~ = 256�x �ōr�炷�ƈ�����Ă� 0 �ɂȂ�̂ň��S.
typedef unsigned char RAngle ;
#define RevAngle(x) (0xff- (x) )

//360�x�`���� 256�x�`���ɕϊ�.
#define ConvertAngle360To256(x) ((x) * 256 / 360)


//���b�N�A�b�v�e�[�u����p���� sin , cos
class RMathAngle  
{
private:
	RMathAngle();	//singleton
public:
	~RMathAngle();

	static const RMathAngle* getMathAngle();

	double Sin(RAngle inDeg) const
	{
		return SinTable[inDeg];
	}
	double Cos(RAngle inDeg) const
	{
		return CosTable[inDeg];
	}
private:
	double SinTable[256];
	double CosTable[256];
};
//�Œ菬�����b�N�A�b�v�e�[�u����p���� sin , cos
#define MY_FIXED_BIT	10	//�Œ菬���̏����Ă�̕����̃r�b�g��
typedef long	RFIXED;

#define ItoF(x)	((x) << MY_FIXED_BIT)
#define FtoI(x)	((x) >> MY_FIXED_BIT)

class RMathAngleFixed
{
private:
	RMathAngleFixed();			//singleton
public:
	~RMathAngleFixed();

	static const RMathAngleFixed* getMathAngleFixed();

	RFIXED Sin(RAngle inDeg) const
	{
		return SinTable[inDeg];
	}
	RFIXED Cos(RAngle inDeg) const
	{
		return CosTable[inDeg];
	}
private:
	RFIXED SinTable[256];
	RFIXED CosTable[256];
};

extern int FastHypot(int a,int b);

#endif // !defined(AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_)
