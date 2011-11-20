/*
#include <.\Base\rti_glookup.h>

//����������킷.
enum Houkou
{
	TOP,	RIGHT,	BUTTON,	LEFT
};

class TBasicDefines;

class TBasicDefines
{
public:
	TBasicDefines(){X = 0; Y = 0; Sx = 0 ; Sy = 0; Angle = 0; Speed = 0;};

	Fixed X,Y;	//X Y �f�[�^
	Fixed Sx,Sy;//���x
	unsigned char Angle;	//��]�p
	Fixed Speed;			//���x

	int GetX(){return FI(X);};
	int GetY(){return FI(Y);};
	int GetSpeed(){return FI(Speed);};
	unsigned char GetAngle(){return Angle;};
	void SetX(int x){X = IF(x);};
	void SetY(int y){Y = IF(y);};
	void SetSpeed(int speed){Speed = IF(speed);};
	void SetAngle(unsigned char angle){Angle = angle;};

	//�G�@��ǂ�������.
	void PathFinder(TBasicDefines *TBO)
	{
		unsigned char Seeta;
	    Fixed x =  TBO->X - X;
		Fixed y =  TBO->Y - Y;
		Seeta = (unsigned char) DDD (atan2( FI(y) , FI(x) ) );
		Angle = Seeta;
	    Sx = FixedMul(FixedLTBCos256[ Seeta ] , Speed) ;
	    Sy = FixedMul(FixedLTBSin256[ Seeta ] , Speed) ;
	};
	//�z�[�~���O
	void Homing(TBasicDefines *TBO)
	{
		//�����ƖڕW�Ƃ̍��W�̍�����ڕW�p�����߂�
	    Fixed kdx =  TBO->X - X;
		Fixed kdy =  TBO->Y - Y;
		double rad = atan2( FI(kdy) , FI(kdx) );

		//���p�����߂�(�ڕW�p - �i�s�p)
		double subdir = rad - RRR(Angle);	//360���W�n

		//����肵�Ȃ����߂̏���(180 �ȓ��łȂ���΁A�t�p)
		if (subdir > RRR(180) )		subdir -= RRR(360);
		if (subdir < RRR(-180) )	subdir += RRR(360);

		//�ڕW�����ցA������������.(�i�s�p += ���p * ����)
		double dir = subdir * 0.05;	//���ɑ΂��Ă̊���.
		Angle += (unsigned char)DDD256(dir);

		//�i�s�p����ړ��ʂ����ߑ�������
		Sx = FixedMul(FixedLTBCos256[Angle] , Speed);
		Sy = FixedMul(FixedLTBSin256[Angle] , Speed);
	};
	void MoveRef()
	{
		X += Sx;
		Y += Sy;
	};
};

*/
