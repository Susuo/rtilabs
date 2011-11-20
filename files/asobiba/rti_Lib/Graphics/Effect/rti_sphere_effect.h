#include "StartWithExeFile.h"
#ifdef SPHERE_LIFE

#ifndef ___SPHERE_EFFECTH
#define ___SPHERE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X
#include <.\rti_define.h>

// �g���l�������̈ʒu
#define TUNNEL_CENTER   -1  // ��ʒ���

// �g���l���̑傫��
#define TUNNEL_DEFAULT_SIZE -1 // �f�t�H���g

// �g���l������
enum TUNNEL_CENTRAL
{
    TUNNEL_NORMAL,   // ���� ( �g����c�݂��Ȃ� )
    TUNNEL_WIDE_X,   // ���E�Ɋg��
    TUNNEL_WIDE_Y,   // �㉺�Ɋg��
    TUNNEL_TO_RIGHT, // �E�ɘc��
    TUNNEL_TO_LEFT,  // ���ɘc��
};

// �g���l���`��
enum TUNNEL_SHAPE
{
    TUNNEL_TYPE_CIRCLE, // �~
    TUNNEL_TYPE_BOX1,   // �l�p
    TUNNEL_TYPE_BOX2,   // �H�`
    TUNNEL_TYPE_CROSS,  // �\��
    TUNNEL_TYPE_WALL,   // ���E�̕�
    TUNNEL_TYPE_GROUND, // �㉺�̒n��
    TUNNEL_TYPE_FLASH,  // �t���b�V������
};

// �g���l���Ȑ�
enum TUNNEL_CURVE
{
    TUNNEL_LINE_NORMAL, // ���� ( ���� )
    TUNNEL_LINE_X,      // �w�Ȑ�
    TUNNEL_LINE_Y,      // �x�Ȑ�
    TUNNEL_LINE_BOX1,   // �l�p�Ȑ�
    TUNNEL_LINE_BOX2,   // �H�`�Ȑ�
    TUNNEL_LINE_CROSS,  // �\���Ȑ�
};

class TSphereEffect
{
private:
    void Init();
    BMD m_OutBmd; //�o��
    BMD m_bmd;
    int TunnelDisplayY,TunnelDisplayX;
	unsigned char TunnelSpritePow2W , TunnelSpritePow2H;
	int TunnelSpritePow2MaskW , TunnelSpritePow2MaskH;
    XY *m_PointChangeTable;
public:
    TSphereEffect();
    ~TSphereEffect();
    bool Draw(int ux,int uy , int Rotate , int Dist);
    bool Set(BMD bmd,BMD OutBmd);
    bool CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx=TUNNEL_CENTER,int Cy=TUNNEL_CENTER,
						  int Size=TUNNEL_DEFAULT_SIZE,TUNNEL_CENTRAL Center=TUNNEL_NORMAL,
						  TUNNEL_SHAPE Type=TUNNEL_TYPE_CIRCLE,TUNNEL_CURVE Line=TUNNEL_LINE_NORMAL);
};

#endif


#endif  //LIFE END
