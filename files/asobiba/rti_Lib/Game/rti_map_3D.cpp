#include "StartWithExeFile.h"
#ifdef THE3D_MAP

#include <.\Graphics\rti_bitmaplib.h>
#include <.\Game\rti_map_3d.h>
#include <.\Base\rti_key.h>
#include <.\Base\rti_debug.h>

/***************************************************************/
//�R���X�g���N�^
/***************************************************************/
TMap3D::TMap3D()
{
    X = 1000;
    Z = 750;
    Y = 50;
    Angle = 0;
	YMax = 0;
	//�ꎞ�I�Ƀ}�b�v(2D)���G�������Ƃ���.
	BmdDummyMap = NULL;
}

/***************************************************************/
//�f�X�g���N�^
/***************************************************************/
TMap3D::~TMap3D()
{
	//�ꎞ�I�Ƀ}�b�v(2D)���G�������Ƃ���̍폜
	if (BmdDummyMap != NULL)	BmpChar->Breaker(BmdDummyMap);
}

/***************************************************************/
//�X�N���[���T�C�Y��`
/***************************************************************/
void TMap3D::SetScreen(int x,int y)
{
	//��q�폜
	if (BmdDummyMap != NULL)	BmpChar->Breaker(BmdDummyMap);
	//�ꎞ�I�Ƀ}�b�v(2D)���G�������Ƃ���.
    BmdDummyMap = BmpChar->New(x,y);
	//�X�N���[���T�C�Y��`
	ScreenX = x; ScreenY = y;
	//�ő卂�x�ݒ�.	x = 512 �� Ymax = 100    	512 : 100 = x: A ������
	YMax = 100 * x / 512 ;
}

/***************************************************************/
//�e��e�N�X�`���̏���.
/***************************************************************/
void TMap3D::UsingTexture(BMD _BmdSky,BMD _BmdSkyGradation,BMD _AddGradation)
{
	BmdSky			= _BmdSky;			//��̃e�N�X�`��
	BmdSkyGradation = _BmdSkyGradation;	//��̉��̕��̃O���f�[�V����
	AddGradation	= _AddGradation;	//�n�ʂƋ�̋��ڂ����߂�O���f�[�V����(���Z�A���t�@�u�����h������.)

    //��̃e�N�X�`���� 90�x�@�ň��肷��̂ł�.
    SkyMoveAngle    = _BmdSky->w / (256/4);
}

/***************************************************************/
//�O����	BmdDummyMap ��2D �Ƃ��ă}�b�v��`���܂�.
/***************************************************************/
void TMap3D::BeforDraw()
{
    int Gx1,Gy1,Gx2,Gy2,Gx3,Gy3,Gx4,Gy4;
/*
        Gx1 = -(256/2)      - Y;
        Gy1 =  (120)        + Y*2;
        Gx2 =  (256/2)      + Y;
        Gy2 =  (120)        + Y*2;
        Gx3 =  (256/2)      + Y /3;
        Gy3 = 0             ;
        Gx4 = -(256/2)      - Y /3;
        Gy4 = 0             ;   
*/ 
        Gx1 = GetGx1(-128, Y );
        Gy1 = GetGy1( 120, Y );
        Gx2 = GetGx2( 128, Y );
        Gy2 = GetGy2( 120, Y );
        Gx3 = GetGx3( 128, Y );
        Gy3 = GetGy3(   0, Y );   
        Gx4 = GetGx4(-128, Y );
        Gy4 = GetGy4(   0, Y );   

        GRx1 = (int)ROTATEX256(Gx1,Gy1,Angle) + X;
        GRy1 = (int)ROTATEY256(Gx1,Gy1,Angle) + Z;
        GRx2 = (int)ROTATEX256(Gx2,Gy2,Angle) + X;
        GRy2 = (int)ROTATEY256(Gx2,Gy2,Angle) + Z;
        GRx3 = (int)ROTATEX256(Gx3,Gy3,Angle) + X;
        GRy3 = (int)ROTATEY256(Gx3,Gy3,Angle) + Z;
        GRx4 = (int)ROTATEX256(Gx4,Gy4,Angle) + X;
        GRy4 = (int)ROTATEY256(Gx4,Gy4,Angle) + Z;

        int minx,miny;
        minx = GRx1; miny = GRy1;
        if (GRx2 < minx) minx = GRx2;        if (GRy2 < miny) miny = GRy2;
        if (GRx3 < minx) minx = GRx3;        if (GRy3 < miny) miny = GRy3;
        if (GRx4 < minx) minx = GRx4;        if (GRy4 < miny) miny = GRy4;

        Nowx = minx;            Nowy = miny;
        MapHosei(&Nowx,&Nowy);      //�␳

        //�G��.
        TMapRotate::Draw(BmdDummyMap);

        //���ׂẮ@Rx Ry�� minx,miny ���W�ɕϊ�.
        GRx1 -= minx;        GRy1 -= miny;
        GRx2 -= minx;        GRy2 -= miny;
        GRx3 -= minx;        GRy3 -= miny;
        GRx4 -= minx;        GRy4 -= miny;

}

/***************************************************************/
//�㏈��	BmdDummyMap �� 2D �Ƃ��āA�`�����}�b�v�� 3D�ɂ��܂�.
/***************************************************************/
void TMap3D::AfterDraw(BMD bmd)
{
        //���]��
        ReSizeDraw(BmdDummyMap,GRx1,GRy1,
                      GRx2,GRy2,
                      GRx3,GRy3,
                      GRx4,GRy4,
                      bmd,
                      0,80,
                      320,80,
                      320,240,
                      0 ,240);

	    int SkyStart;
//        SkyStart = ( (255 - Map->GetAngle()) % (256 / 4) ) * SkyMoveAngle;
        SkyStart = ( (255 - Angle) &63 ) * SkyMoveAngle;

        //�󂾂��
		DirectDraw(BmdSky,0,0,BmdSky->w,BmdSky->h,bmd,SkyStart,0);
		DirectDraw(BmdSky,0,0,BmdSky->w,BmdSky->h,bmd,SkyStart - BmdSky->w,0);
        //��̉��̃O���f�[�V����
		DirectDraw(BmdSkyGradation,0,0,
            BmdSkyGradation->w,BmdSkyGradation->h,bmd,0,BmdSky->h);
        //�}�b�v�ɂ�������O���f�[�V����
		AddDraw(AddGradation,0,0,AddGradation->w,AddGradation->h,bmd,0,80);
}

bool TMap3D::DrawObject(BMD bmd,int x,int y,int w,int h,int ox,int oy,int oz,BMD Sp)
{
    int Rx1,Ry1,Rx2,Ry2,Rx3,Ry3,Rx4,Ry4;
    int Gx1,Gy1,Gx2,Gy2,Gx3,Gy3,Gx4,Gy4;
    int NEX,NEZ;

    //���_���W�ɕϊ�
    ox -= X;
    oy -= Y;
    oz -= Z;
    //���ɂ����΂����قǁA Z ���ł����Ȃ�悤�ɂ���.
    oz = -oz;
    //���_�ɂ���].
    NEX = (int)ROTATEX256(ox,oz,Angle);
    NEZ = (int)ROTATEY256(ox,oz,Angle);
    ox  =   NEX;
    oz  =   NEZ;
    oy = - oy;

    if (oz>=0)  return false;   // ���_�����
    if (oz<=-700)  return false;   // ��������̂Ŗv
    
    Gx1 = ox;
    Gy1 = oy;
    Gx2 = ox + bmd->w;
    Gy2 = oy;
    Gx3 = ox + bmd->w;
    Gy3 = oy + bmd->h;
    Gx4 = ox;
    Gy4 = oy + bmd->h;

#define APPENDER    32

    //�������e
    Gx1 = Gx1*APPENDER / oz;
    Gy1 = Gy1*APPENDER / oz;
    Gx2 = Gx2*APPENDER / oz;
    Gy2 = Gy2*APPENDER / oz;
    Gx3 = Gx3*APPENDER / oz;
    Gy3 = Gy3*APPENDER / oz;
    Gx4 = Gx4*APPENDER / oz;
    Gy4 = Gy4*APPENDER / oz;

    //�f�B�X�v���C���W�ɕϊ�
#define DISPLAY_W   (320/2)
#define DISPLAY_H   (200/2)

    Rx1 = DISPLAY_W + Gx1;    Ry1 = DISPLAY_H - Gy1;
    Rx2 = DISPLAY_W + Gx2;    Ry2 = DISPLAY_H - Gy2;
    Rx3 = DISPLAY_W + Gx3;    Ry3 = DISPLAY_H - Gy3;
    Rx4 = DISPLAY_W + Gx4;    Ry4 = DISPLAY_H - Gy4;

    //���]��
    ReSizeDraw(bmd,x,y,w,h,
                      Sp,
                      Rx1 ,Ry1,
                      Rx2 ,Ry2,
                      Rx3 ,Ry3,
                      Rx4 ,Ry4  );

    return true;
}


#endif //LIFE END
