#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_RESIZEH
#define GRAPHICS_RESIZEH
/*****************************************************************/
//���R�ϊ�
/*****************************************************************/

#include <.\Graphics\rti_graphics.h>

    struct _XYT
    {
        int x,y;        //���W
        int tx,ty;      //�e�N�X�`��
    };

//���R�ϊ� ���C����
    //���ڌĂяo���Ă����\�ł� (^^;;
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY);
//���R�l�p�`�n�b�`���O.
     void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb);

    //�w�肵���e�N�X�`��(�l�p�`)�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4)
     {
        _XYT XY[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    XY[0].tx = rx1;    XY[0].ty = ry1;
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    XY[1].tx = rx1+rw1;XY[1].ty = ry1;
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    XY[2].tx = rx1+rw1;XY[2].ty = ry1+rh1;
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    XY[3].tx = rx1;    XY[3].ty = ry1+rh1;
        //���C������
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
     //�w�肳�ꂽ���G�Ȍ`�̃e�N�X�`�������R�ϊ����ĊG�悷��.
     inline void ReSizeDrawEx(const BMD bmd1,
                      int rx1_1,int ry1_1,
                      int rx1_2,int ry1_2,int rx1_3,int ry1_3,
                      int rx1_4,int ry1_4,
                      BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4)
     {
        _XYT XY[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    XY[0].tx = rx1_1;   XY[0].ty = ry1_1;
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    XY[1].tx = rx1_2;   XY[1].ty = ry1_2;
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    XY[2].tx = rx1_3;   XY[2].ty = ry1_3;
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    XY[3].tx = rx1_4;   XY[3].ty = ry1_4;
        //���C������
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
    //�w�肵���e�N�X�`��(�l�p�`)�����R�ϊ����ĊG�悷��(������ReSizeDrawMain�Ăяo��)
     inline void ReSizeBox(BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4,int Rgb)
     {
        _XYT XY[4];    //�ϊ��������W���i�[. (rx2 , ry2�V���[�Y)

        //�ϊ��������W���i�[����. (rx2 , ry2�V���[�Y)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    
        //���C������
        ReSizeBoxMain(bmd2,XY,Rgb);
     }

#endif

#endif //LIFE END
