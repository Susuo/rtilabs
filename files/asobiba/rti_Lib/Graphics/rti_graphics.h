#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICSH
#define GRAPHICSH

#include <.\Base\rti_glookup.h>
#include <.\rti_define.h>
#include <.\rti_namespace.h>
#include <windows.h>
#define BMD_W_POSSITION	0	//BMD �� w �����o�̈ʒu (�@��ˑ�)
#define BMD_H_POSSITION	4	//BMD �� h �����o�̈ʒu (�@��ˑ�)
#define BMD_BM_POSSITION 8	//BMD �� bm �����o�̈ʒu (�@��ˑ�)
#define BMD_POW2W_POSSITION 12//BMD �� Pow2W �����o�̈ʒu (�@��ˑ�)

#include "rti_graphics_inlines.h" 
#include "rti_graphics_resize.h" 

_GRAPHICS_BEGIN //namespace begin
    //�O���t�B�b�N�X����
	 int GraphicsCutTile(const BMD bmd,int sizex,int sizey,short *cx,short *cy);
    //�g��k��
     void BigDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int rw2,int rh2);
    //�ڂ���
	 void ShadeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //������
     void Blend50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //�w�蓧��
     void BlendDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed rblend,Fixed gblend,Fixed bblend);
    //���傶��ɓ���
	 void BlendGradetionDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
						BMD bmd2,int rx2,int ry2,
						Fixed fblend,Fixed fglend,Fixed frlend,
						unsigned char mode,int start,int size);
    //�����x����
     void Bright50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
     //�w�薾�x����
     void BrightDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend);
    //���Z�A���t�@�u�����h
     void AddDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //���Z�A���t�@�u�����h
     void SubDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //Bit And ����
     void AndDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //Bit Or ����
     void OrDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //�����F�����̍����G��
     void DirectDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //���]
     void RevDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1);
    //�����F����̊G��
     void Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);    
    //�N���A
     void Cls(BMD bmd,BYTE clear = 0x00);
    //���C����`��
     void Line(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb);
    //�l�p������
     void Box(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb);
    //�~��`��
     void Circle(BMD bmd1,int rx1,int ry1,int rr1,long rgb);
    //�㉺���E���]
     void MillorDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,bool lr,bool ud);
    //���U�C�N��������.
     void MosaicDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,int mw,int mh);
    //�w�肵�����������Z���ĕ\������.
     void FilterDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      int R,int G,int B);
    //�w�肳�ꂽ�x�̐�����������ɂ��炷.
     void ShiftDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int S);
    //��]
	 void RotateDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,Angle seeta);
    //��]3D
	 void Rotate3DDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,int jikuz,
					  unsigned char seetax,unsigned char seetay,unsigned char seetaz);
	//�O���C�X�P�[���ŊG��
	void GrayScaleDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
		                  BMD bmd2,int rx2,int ry2);
     //�ȈՃN���b�s���O
	bool EasyClipingEx(BMD bmd,int *rx1,int *ry1,int *rw1,int *rh1,
				 int rw2,int rh2,int rx2,int ry2,
				 int *sw,int *sh,int *ew,int *eh);
    //�f�[�^�̓��o��.
	 BYTE *SeekData(BMD bmd,int rx,int ry,
			  int sw,int sh,int *lpAddPitch);

_GRAPHICS_END   //namespace end

USEING_GRAPHICS //�l�[���X�y�[�X�̗��p.

#define RTIRGB(r,g,b) ((DWORD)(((BYTE)(b) | \
    ((WORD)(g) << 8)) | \
    (((DWORD)(BYTE)(r)) << 16)))

//�ėp�O���t�B�b�N�X�֐��R�[��.
// DirectDraw   Draw    Blend50Draw     ShadeDraw   Bright50Draw
//  AddDraw SubDraw AndDraw OrDraw  
typedef void (*GraphicsProc)(const BMD,int,int,int,int,BMD,int,int);

#endif


#endif //LIFE END
