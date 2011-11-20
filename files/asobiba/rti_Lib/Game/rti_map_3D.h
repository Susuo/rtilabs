#include "StartWithExeFile.h"
#ifdef THE3D_MAP

#ifndef ___MAP3DH
#define ___MAP3DH

#include <.\Game\rti_map_rotate.h>

class TMap3D : public TMapRotate
{
    int GRx1,GRy1,GRx2,GRy2,GRx3,GRy3,GRx4,GRy4;	//BmdDummyMap �̒��� �]���J�n�̈�̊e���_
	BMD BmdSky;			//��̃e�N�X�`��
	BMD BmdSkyGradation;//��̉��̕��̃O���f�[�V����
	BMD AddGradation;	//�n�ʂƋ�̋��ڂ����߂�O���f�[�V����(���Z�A���t�@�u�����h������.)
	BMD BmdDummyMap;	//�ꎞ�I�Ƀ}�b�v(2D)���G�������Ƃ���.
	int SkyMoveAngle;	//1�x�������т̋�̃e�N�X�`���̑���
    int X,Y,Z;
	unsigned char Angle;	//��]�p�x.
	int YMax;			//�ō����x
public:
    TMap3D();
	~TMap3D();
	BMD GetDummyMap(){return BmdDummyMap;};		//�_�~�[�̈�̎擾.
    int GetX(){return X;};
    int GetY(){return Y;};
    int GetZ(){return Z;};
	unsigned char GetAngle(){return Angle;};	//�A���O���̎擾
	virtual void SetScreen(int x,int y);	//�X�N���[���T�C�Y��`
    //Map3D �����ϊ�
        // -128 : -Y = X : A      A �����߂�.  -Y * X / -128
        int GetGx1(int X,int Y){    return (-Y)*X/(-128) + X;   };
        //  128 : Y = X : A      A �����߂�.  Y * X / 128
        int GetGx2(int X,int Y){    return (Y)*X/(128)   + X;   };
        //  128 : (Y/3) = X : A      A �����߂�.  (Y/3) * X / 128
        int GetGx3(int X,int Y){    return (Y/3)*X/(128) + X;   };
        //  -128 : (-Y/3) = X : A      A �����߂�.  (-Y/3) * X / -128
        int GetGx4(int X,int Y){    return (-Y/3)*X/(-128) + X;   };
        // 120 : (Y*2) = Z : A      A �����߂�.  (Y*2) * Z / 120
        int GetGy1(int Z,int Y){    return (Y*2) * Z / 120 + Z;   };
        // 120 : (Y*2) = Z : A      A �����߂�.  (Y*2) * Z / 120 (Gy2 �Ɠ���)
        int GetGy2(int Z,int Y){    return (Y*2) * Z / 120 + Z;   };
        // 0 : 0 = Z : A             ������ 0 ����� (^^;;;
        int GetGy3(int Z,int Y){    return 0 + Z;   };
        // 0 : 0 = Z : A             ������ 0 ����� (^^;;; (Gy4 �Ɠ���)
        int GetGy4(int Z,int Y){    return 0 + Z;   };
	//�e��ړ�
	virtual void UpProc(int p){		Y-=p;	if (Y < 0 ) Y = 0;	};
	virtual void DownProc(int p){	Y+=p;	if (Y > YMax ) Y = YMax;	};
	virtual void LeftProc(unsigned char p){	Angle-=p;	};
	virtual void RightProc(unsigned char p){Angle+=p;	};
	virtual void AfterProc(int p)
	{
		    unsigned char RR = 255- Angle;
	        X -= ANGLE_MOVEX(RR,p);
	        Z -= ANGLE_MOVEY(RR,p);
		    MapHosei(&X,&Z);      //�␳
	};
	virtual void BeforProc(int p)
	{
		    unsigned char RR = 255- Angle;
	        X += ANGLE_MOVEX(RR,p);
	        Z += ANGLE_MOVEY(RR,p);
		    MapHosei(&X,&Z);      //�␳
	};
    //2����(�}�b�v)�ɕ��̕\��.
    void DrawObject2D(GraphicsProc Proc,BMD bmd,int x,int y,int w,int h,int ox,int oy)
    {
        Proc(bmd,x,y,w,h,BmdDummyMap,ox,oy);
    };
    //3�������̕\��	
    bool DrawObject(BMD bmd,int x,int y,int w,int h,int ox,int oy,int oz,BMD Sp);
	//�e��e�N�X�`���̏���.
	void UsingTexture(BMD _BmdSky,BMD _BmdSkyGradation,BMD _AddGradation);
	//�}�b�v���G�悵�܂�(�~���X�N���[�� 3D)
	virtual void Draw(BMD bmd){	BeforDraw();	AfterDraw(bmd);	}
	//�O����	BmdDummyMap ��2D �Ƃ��ă}�b�v��`���܂�.
	void BeforDraw();
	//�㏈��	BmdDummyMap �� 2D �Ƃ��āA�`�����}�b�v�� 3D�ɂ��܂�.
	void AfterDraw(BMD bmd);
};

#endif

#endif //LIFE END
