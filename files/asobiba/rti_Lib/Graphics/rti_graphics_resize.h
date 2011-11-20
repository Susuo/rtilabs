#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_RESIZEH
#define GRAPHICS_RESIZEH
/*****************************************************************/
//���R�ϊ�
/*****************************************************************/

#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_zbuffer.h>


    struct _XYT
    {
        short x,y;        //���W
        short tx,ty;      //�e�N�X�`��
    };
    struct _XYZT
    {
        short x,y,z;        //���W
        short tx,ty;        //�e�N�X�`��
    };
    struct _XYZCT
    {
        short x,y,z;        //���W
        short cr,cg,cb;     //�F
        short tx,ty;        //�e�N�X�`��
    };

_GRAPHICS_BEGIN //namespace begin

    struct _WorkArea        //�ӂ[
    {
        Fixed StepX,StepTX,StepTY;
    };
    struct _WorkAreaZ       // ZBuffer
    {
        Fixed StepX,StepZ,StepTX,StepTY;
    };
    struct _WorkAreaG       //�O���[�V�F�[�f�B���O
    {
        Fixed StepX,StepZ,StepCR,StepCG,StepCB,StepTX,StepTY;
    };

//�ʏ�G��
    //���R�ϊ� �e�N�X�`���[�ό`
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYT *XY);
    //���R�l�p�`�n�b�`���O.
     void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb);
     void ReSizeBoxMain3(BMD bmd2,_XYT *XY,int Rgb);
//ZBuffer
    //���R�ϊ� �e�N�X�`���[�ό`
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer);
    //���R�l�p�`�n�b�`���O.
     void ReSizeBoxMain(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer);
     void ReSizeBoxMain3(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer);
//�O���[�V�F�[�f�B���Os
    //���R�ϊ� �e�N�X�`���[�ό`
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer);
    //���R�l�p�`�n�b�`���O. 
     void ReSizeBoxMain(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer);
     void ReSizeBoxMain3(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer);

     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDraw(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,XY xy1, XY xy2,XY xy3, XY xy4)
     {
        _XYT XY[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)
        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = xy1.x; XY[0].y = xy1.y; XY[0].tx = txy1.x;   XY[0].ty = txy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y; XY[1].tx = txy2.x;   XY[1].ty = txy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y; XY[2].tx = txy3.x;   XY[2].ty = txy3.y;
        XY[3].x = xy4.x; XY[3].y = xy4.y; XY[3].tx = txy4.x;   XY[3].ty = txy4.y;
        //���C������
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
    //�w�肵���F�Ŏl�p�`�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBox(BMD bmd2,XY xy1, XY xy2,XY xy3, XYZ xy4,int Rgb)
     {
        _XYT XY[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = xy1.x; XY[0].y = xy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y;
        XY[3].x = xy4.x; XY[3].y = xy4.y;
        //���C������
        ReSizeBoxMain(bmd2,XY,Rgb);
     }
     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDrawZ(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, XYZ xyz4,LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)
        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z; XYZ[3].tx = txy4.x;   XYZ[3].ty = txy4.y;
        //���C������
        ReSizeDrawMain(bmd1,bmd2,XYZ,lpZBuffer);
     }
    //�w�肵���F�Ŏl�p�`�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBoxZ(BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, XYZ xyz4,
			                      int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z;
        //���C������
        ReSizeBoxMain(bmd2,XYZ,Rgb,lpZBuffer);
     }
     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDrawG(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 XYZ xyz4,_RGB rgb4,
                 LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)
        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z; XYZ[3].tx = txy4.x;   XYZ[3].ty = txy4.y;
        XYZ[3].cr = rgb4.r;XYZ[3].cg = rgb4.g;XYZ[3].cb = rgb4.b;  
        //���C������
        ReSizeDrawMain(bmd1,bmd2,XYZ,lpZBuffer);
     }
     
    //�w�肵���F�Ŏl�p�`�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBoxG(BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 XYZ xyz4,_RGB rgb4,
                 int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z;
        XYZ[3].cr = rgb4.r;XYZ[3].cg = rgb4.g;XYZ[3].cb = rgb4.b;  
        //���C������
        ReSizeBoxMain(bmd2,XYZ,Rgb,lpZBuffer);
     }




    //��������́A�O�p�`�o�[�W�����ł�,

     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDraw3(const BMD bmd1, XY txy1, XY txy2,XY txy3,
                                            BMD bmd2,XY xy1, XY xy2,XY xy3)
     {
        _XYT XY[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = xy1.x; XY[0].y = xy1.y; XY[0].tx = txy1.x;   XY[0].ty = txy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y; XY[1].tx = txy2.x;   XY[1].ty = txy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y; XY[2].tx = txy3.x;   XY[2].ty = txy3.y;
        //���C������
        ReSizeDrawMain3(bmd1,bmd2,XY);
     }
    //�w�肵���e�N�X�`��(3�p�`)�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBox3(BMD bmd2,XY xy1, XY xy2,XY xy3,int Rgb)
     {
        _XYT XY[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = xy1.x; XY[0].y = xy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y;
        //���C������
        ReSizeBoxMain3(bmd2,XY,Rgb);
     }
     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDrawZ3(const BMD bmd1, XY txy1, XY txy2,XY txy3,
                  BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        //���C������
        ReSizeDrawMain3(bmd1,bmd2,XYZ,lpZBuffer);
     }
    //�w�肵���e�N�X�`��(3�p�`)�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBoxZ3(
           BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3,int Rgb,LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        //���C������
        ReSizeBoxMain3(bmd2,XYZ,Rgb,lpZBuffer);
     }
     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDrawG3(const BMD bmd1,XY txy1, XY txy2,XY txy3,
                 BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)
        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        //���C������
        ReSizeDrawMain3(bmd1,bmd2,XYZ,lpZBuffer);
     }
     
    //�w�肵���F�Ŏl�p�`�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBoxG3(BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[3];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        //���C������
        ReSizeBoxMain3(bmd2,XYZ,Rgb,lpZBuffer);
     }

_GRAPHICS_END   //namespace end

#endif

#endif //LIFE END
