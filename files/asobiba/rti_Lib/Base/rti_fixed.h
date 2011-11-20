#ifndef ___FIXEDH
#define ___FIXEDH

/****************************************************************************/
//�Œ菬���_(16:16)  �����ăN���X���Ȃ��̂͂Ȃ� ? ���[��A asm �ň����₷�����邽�߂���(��)
/****************************************************************************/
#include <math.h>

//�߂�l�Ȃ��̌x����ق点��. (eax �Œ��ڂɕԂ�)
#pragma warning(disable: 4035)

//�Œ菬���_    ���� 16 : ���� 16 �̌Œ菬���_�ǂ���
typedef long Fixed;

//�}�X
#define fxsin(x) (FixedLTBSin256[((unsigned char)(x))])
#define fxcos(x) (FixedLTBCos256[((unsigned char)(x))])
#define fxtan(x) (FixedDiv(fxsin(x),fxcos(x)))
#define fxpow(x,y) (FixedPow(x,y))
#define fxatan(x) (FixedAtan(x))
#define fxatan2(x,y) (FixedAtan( FixedDiv(x,y) ))
#define fxhypot(x,y) (fast_hypot(x,y) )
//�p�C�̌Œ菬���_
#define PAI_FIXED (205887) 
//�ő�l
#define FIXED_HIGH_VALUE	(0x7fffffff)
//�ŏ��l
#define FIXED_LOW_VALUE		(0x8000001)

//�ϊ�
#define Int2Fixed(x) (( (long)(x))<<16 )    //�������Œ菬���_�ɕϊ�
#define Fixed2Int(x) ((x)>>16 )             //�Œ菬���_�𐳐��ɕϊ�
#define Float2Fixed(x) ((long)((x)*65536.0) )   //�������Œ�ɕϊ�
#define Fixed2Float(x) ((double)((x)/65536.0) ) //�Œ�𕂓��ɕϊ�
#define FixedRound(x) (x+0x8000)                //�l�̌ܓ�
#define FixedInt(x) (x&0xffff0000)              //�����_�؂�̂�
//�Œ菬���_�̒Z�k�`...??
#define IF(x)   Int2Fixed(x)
#define FI(x)   Fixed2Int(x)

//�|���Z�Ɗ���Z.
//***************************************************
//�Œ菬���_(<<16)���m�̊|���Z
//***************************************************
inline Fixed FixedMul(Fixed const num1, Fixed const num2)
{
		__asm
		{
			mov eax, num1
			mov edx, num2
			imul edx
			shrd eax, edx, 16
		}
}

//***************************************************
//�Œ菬���_(<<16)���m�̂��Z
//***************************************************
inline Fixed FixedDiv(Fixed const num1, Fixed const num2)
{
	register Fixed result;
	try
	{
		__asm
		{
            mov edx,num1
            mov ebx,num2
            xor eax,eax
            sar edx,16

            shrd eax,edx,16
            idiv ebx
			mov result,eax
		}
	}
	catch(...)
	{
		return 0;
	}
	return result;
}


//***************************************************
//�A�[�N�^���W�F���g(�Œ菬���_�o�[�W����) �Q�l:�A���S���Y�����T
//***************************************************
#define ATAN_N  24  /* �{���Q�� */
inline Fixed FixedAtan(Fixed x)  
{
	int i, sgn;
    Fixed PowX;
	Fixed a;

	if      (x >  IF(1) ) {  sgn =  1;  x = FixedDiv(IF(1) , x);  }
	else if (x < IF(-1) ) {  sgn = -1;  x = FixedDiv(IF(1) , x);  }
	else                sgn =  0;
	a = 0;
    PowX = FixedMul(x,x);
	for (i = ATAN_N; i >= 1; i--)
    {
//		a = (i * i * x * x) / (2 * i + 1 + a);
        a = FixedDiv(  (i * i * PowX)  , ((i*2) + IF(1) + a) );
    }
//	if (sgn > 0) return  PI / 2 - x / (1 + a);
//	if (sgn < 0) return -PI / 2 - x / (1 + a);
//	/* else */   return           x / (1 + a);
	if (sgn > 0) return  (PAI_FIXED /2) - FixedDiv(x , IF(1) + a);
	if (sgn < 0) return -(PAI_FIXED /2) - FixedDiv(x , IF(1) + a);
	/* else */   return           FixedDiv(x , IF(1) + a);

}

//***************************************************
//�ׂ���
//***************************************************
inline Fixed FixedPow(Fixed a,Fixed b)
{
	return Float2Fixed (  pow(Fixed2Float(a),Fixed2Float(b) )  );
//	return IF(1);
}

//***************************************************
//2��
//***************************************************
inline Fixed FixedPow2(Fixed a)
{
	return FixedMul(a,a);
//	return IF(1);
}


//�߂�l�Ȃ��̌x��������ׂ点��.
#pragma warning(error: 4035)

#endif
