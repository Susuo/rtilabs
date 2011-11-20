
#include <./rti_define.h>
#include <./Graphics/rti_graphics.h>
#include <windows.h>

#ifndef ___RTI_GRAPHICS_TRIANGLE_DRAWH
#define ___RTI_GRAPHICS_TRIANGLE_DRAWH


#define __TRIANGLE_DRAW_LEFT	0
#define __TRIANGLE_DRAW_RIGHT	1

	//�x�^
    struct _XYZP
    {
		Fixed x,y,z;
    };
	//�e�N�X�`��
    struct _XYZTP
    {
		Fixed x,y,z;
        short tx,ty;        //�e�N�X�`��	
    };
	//�x�^ �O���[.
    struct _XYZCTP
    {
		Fixed x,y,z;
        short cr,cg,cb;     //�F
    };
	//�e�N�X�`�� �O���[.
    struct _XYZCTGP
    {
		Fixed x,y,z;
        short cr,cg,cb;     //�F
        short tx,ty;        //�e�N�X�`��
    };

	//�x�^�ɕK�v�Ȃ��̑��̃f�[�^.
	struct _BetaData
	{
		int		rgb;
		BMD		OutBmd;
		LPZBUFFER ZBuffer;
	};

	//�e�N�X�`���ɕK�v�Ȃ��̑��̃f�[�^.
	struct _TextureData
	{
		BMD		OutBmd;
		BMD		TextureBmd;
		LPZBUFFER ZBuffer;
	};


	//�x�^
    struct _WorkAreaP
    {
        Fixed StepX,StepZ;
    };
	//�e�N�X�`���[.
    struct _WorkAreaTP
    {
        Fixed StepX,StepZ,StepTX,StepTY;
    };
	//�x�^�@�O���[.
    struct _WorkAreaGP
    {
        Fixed StepX,StepZ,StepCR,StepCG,StepCB;
    };
	//�x�^�@�e�N�X�`��.
    struct _WorkAreaTGP
    {
        Fixed StepX,StepZ,StepCr,StepCG,StepCB,StepTX,StepTY;
    };

	struct _NN_WorkAreaP
	{
		int			Y;
		_WorkAreaP	Data[2]; // L R
	};


//�P���l�p�`�G��.
void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData);


#endif