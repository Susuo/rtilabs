#ifndef __RTI_DEFINE_H
#define __RTI_DEFINE_H

#include <.\Base\rti_glookup.h>

typedef struct BMP_DATA_TABLE* BMD;
struct BMP_DATA_TABLE{
	int w,h;
	BYTE *bm ;
	int  Pow2W;				// W �� 2 �ׂ̂�����������A������ ���̐����������.
    void* Tag;
};

//3����
struct XYZ
{
    short x,y,z;
};

//3�����Œ菬��
struct XYZFixed
{
    Fixed x,y,z;
};

//3������]�p�x
struct XYZChar
{
    unsigned char x,y,z;
};

//2����
struct XY
{
	short x,y;
};
//2�����Œ�
struct XYFixed
{
	Fixed x,y;
};

//RGB
struct _RGB
{
	unsigned char b,g,r;
};

typedef unsigned short Angle;			//��]�p�x(1024�x).
typedef unsigned short Angle1024;		//��]�p�x(1024�x).
#define __ANGLE_REV(x) (1023-(__ANGLE(x)))	//�t�ɂ���.
#define __ANGLE(x) ((x)&0x03ff)				//�p�x�␳.

#ifdef _DEBUG
//ASSERT �̃p�N��
	#define __RTI_CHECKER(f) \
		do \
		{ \
			if (!(f) ) \
			{ \
				MSG msg;	\
				BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);	\
				if (bQuit)	PostQuitMessage(msg.wParam);	\
				__asm { int 3 }; \
			} \
		} while (0) 
#else
	#define __RTI_CHECKER(f) /*     */
#endif

#endif
