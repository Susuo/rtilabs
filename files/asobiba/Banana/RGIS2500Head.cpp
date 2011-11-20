// RGIS2500Head.cpp: RGIS2500Head �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGIS2500Head.h"
#include "RGISSlm.h"
#include "Sec2Utm.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGIS2500Head::RGIS2500Head()
{
	this->Scale = 0;
}

RGIS2500Head::~RGIS2500Head()
{

}

void RGIS2500Head::Create(const char * inStr, int inScale) throw(RException)
{
	//FH
	//�}��
	//�t�@�C���� 
	//���R�[�h����
	//�v�f�̑���
	//���W�n�t���O
	//���W�n�ԍ�
	//���l���敪
	//�}�s�����p�̂w���W
	//�}�s�����p�̂x���W
	//�}�s�E��p�̂w���W
	//�}�s�E��p�̂x���W
	//�}�s����p�̂w���W
	//�}�s����p�̂x���W
	//�}�s�E���p�̂w���W
	//�}�s�E���p�̂x���W

	int c = 
		sscanf(inStr , "FH,%128[^,],%256[^,],%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
			Zumai , Filename , &Record , &Count, &ZahyoFlg , &ZahyoNumber , &Kubun ,
			&LowerLeft.x , &LowerLeft.y ,
			&UpperRight.x , &UpperRight.y ,
			&UpperLeft.x , &UpperLeft.y ,
			&LowerRight.x , &LowerRight.y );
	if ( c != 15 )
	{
		throw RException(string("") + "�w�b�_�̃p�[�X�Ɏ��s���܂��� ���ꂾ���̐������p�[�X�Œ��܂���ł���:" + num2str(c));
	}
	if ( ZahyoNumber <= 0 || ZahyoNumber > 19)
	{
		throw RException(string("") + "�w�b�_�̃p�[�X�Ɏ��s���܂��� ���W�n�� 1 �` 19 �͈̔͂łȂ���΂����܂���");
	}
	if ( ZahyoFlg !=2 )
	{
		throw RException(string("") + "�w�b�_�̃p�[�X�Ɏ��s���܂��� ���W�n�� 2 �łȂ���΂Ȃ�܂���");
	}

	this->Scale = inScale;
}


#include "RGISMath.h"

UPOINT RGIS2500Head::convertXY(double x,double y) const
{
	ASSERT( this->Scale >= 100);

	DoublePoint dxy;
	RGISMath math;
	dxy = math.JMG2DMS(	(x + LowerLeft.x) ,
						(y + LowerLeft.y) ,
						ZahyoNumber,
						&RGISMath::WGS84
				);
	return LtoU( math.KeiidoTo( dxy.x , dxy.y ,this->Scale) );
}


void RGIS2500Head::test()
{
	//�e�X�g�͔j��.
}

