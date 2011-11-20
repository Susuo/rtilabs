#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_INLINEH
#define GRAPHICS_INLINEH

#include <.\Graphics\rti_graphics.h>
#include <.\Comm\rti_Comm.h>

_GRAPHICS_BEGIN //namespace begin

//�߂�l�Ȃ��̌x����ق点��. (eax �Œ��ڂɕԂ�)
#pragma warning(disable: 4035)

/*****************************************************************/
//�����ɓ_������
/*****************************************************************/
inline void PSet(BMD bmd1,int rx1,int ry1,long rgb)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    _asm{
		mov edi,bmd1
        mov eax,ry1
		mov ebx,[edi+BMD_W_POSSITION]	//w
		mov esi,[edi+BMD_BM_POSSITION]	//bm
        mul ebx
        add eax,rx1 ;(rx1+ry1*bmd1->w)

        mov edi,eax ;*3 �� X<< 2 - X �Ōv�Z����
        shl edi,2
        sub edi,eax

        add edi,esi   ;�]���J�n�A�h���X
        mov eax,[edi]
        mov ebx,rgb        ;���܂������ʂ�
        and eax,0xff000000
        and ebx,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�

        or  eax,ebx
        mov [edi],eax
    }
	CommDraw->GraphicsUnLock(bmd1);
}
/*****************************************************************/
//�_�C���N�g�ɂ����ɓ_������
/*****************************************************************/
inline void DirectPSet(BYTE *data1,long rgb)
{
    _asm{
		mov edi,data1
        mov eax,[edi]
        mov ebx,rgb        ;���܂������ʂ�
        and eax,0xff000000
        and ebx,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�

        or  eax,ebx
        mov [edi],eax
    }
}

/*****************************************************************/
//�_�C���N�g�ɂ����ɓ_������
/*****************************************************************/
inline void DirectPSet(BYTE *data1,BYTE *data2)
{
    _asm{
		mov edi,data1
		mov esi,data2
        mov ebx,[esi]
        mov eax,[edi]
        and ebx,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
        and eax,0xff000000

        or  eax,ebx
        mov [edi],eax
    }
}

/*****************************************************************/
//�_�C���N�g�ɂ����ɓ_������ & �F�␳
/*****************************************************************/
inline void DirectPSet(BYTE *data1,long rgb,int AddRGB)
{
    _asm{
		mov edi , data1		//�f�[�^�������ݐ�
		mov esi , rgb			//���̐F
		mov ecx , AddRGB		//�␳�f�[�^.
		mov edx , [edi]		//edx �Ɍ��X�̃f�[�^�̓ǂݍ���(24Bit�Ȃ̂� 3 �o�C�g�P�ʃA�N�Z�X�̂���.)
		//eax ebx �́A���[�N.
		// edx �ɂ��߂���ł����܂�.

		//R�̏���
		mov eax , esi		//��
		mov ebx , ecx		//�␳
		and edx , 0xff000000 //�����̂Ƃ���� 3 �o�C�g��(RGB)�m��.

        and eax , 0x00ff0000  //���̃f�[�^�̐؂���
        and ebx , 0x00ff0000  //�␳�̃f�[�^�̐؂���
        add eax , ebx         //����(�␳)
        test eax, 0xff00ffff //�I�[�o�[�␳.
        jz  Non_R_Over
            mov eax , 0x00ff0000  //255 �z���Ă���̂ŉ����߂�.
    Non_R_Over:
        or   edx , eax        //��������.

		//G�̏���
		mov eax , esi		//��
		mov ebx , ecx		//�␳

        and eax , 0x0000ff00  //���̃f�[�^�̐؂���
        and ebx , 0x0000ff00  //�␳�̃f�[�^�̐؂���
        add eax , ebx         //����(�␳)
        test eax, 0xffff00ff //�I�[�o�[�␳.
        jz  Non_G_Over
            mov eax , 0x0000ff00  //255 �z���Ă���̂ŉ����߂�.
    Non_G_Over:
        or   edx , eax        //��������.

		//B�̏���
		//�Ō�Ȃ̂ŁA�䒠�� esi(��)�� ecx(�␳)�����̂܂܎g��.
        and esi , 0x000000ff  //���̃f�[�^�̐؂���
        and ecx , 0x000000ff  //�␳�̃f�[�^�̐؂���
        add esi , ecx         //����(�␳)
        test esi, 0xffffff00 //�I�[�o�[�␳.
        jz  Non_B_Over
            mov esi , 0x000000ff  //255 �z���Ă���̂ŉ����߂�.
    Non_B_Over:
        or   edx , esi        //��������.
        //�������āA�쐬���� edx �������߂�.
        mov [edi],edx
    }
}

/*****************************************************************/
//�_�C���N�g�ɂ����ɓ_������ & �����ɐF�␳��������.
/*****************************************************************/
inline void DirectPSet(BYTE *data1,BYTE *data2,int AddRGB)
{
    _asm{
        //�]�����A�]����A�h���X�����W�X�^�[��
		mov edi,data1
		mov esi,data2
        //�F�␳�f�[�^�̓ǂݍ���
        mov ecx,AddRGB
        //�]�����ƁA�]���悩��ǂݍ��� (�]���悩��ǂݍ��ނ̂́A3�o�C�g�P�ʂ�����.)
        mov ebx,[esi]
        mov eax,[edi]
        //ebx �Ɍ��̃f�[�^���͂����Ă܂�.
        //ecx �ɂ́A�F�␳�f�[�^�������Ă܂�.
        //eax �́A�␳�����f�[�^�����߂���ł����܂�.
        //esi edx �́A���[�N�G���A.

        //R�̏���.
        mov esi , ebx //���X�̃f�[�^������.
        mov edx , ecx //�␳�f�[�^������.
        and eax , 0xff000000 //�����̂Ƃ���� 3 �o�C�g��(RGB)�m��.
        
        and esi,0x00ff0000  //���̃f�[�^�̐؂���
        and edx,0x00ff0000  //�␳�̃f�[�^�̐؂���
        add esi,edx         //����(�␳)
        test esi,0xff00ffff //�I�[�o�[�␳.
        jz  Non_R_Over
            mov esi,0x00ff0000  //255 �z���Ă���̂ŉ����߂�.
    Non_R_Over:
        or   eax,esi        //��������.
        //G�̏���.
        mov esi , ebx //���X�̃f�[�^������.
        mov edx , ecx //�␳�f�[�^������.
        
        and esi,0x0000ff00  //���̃f�[�^�̐؂���
        and edx,0x0000ff00  //�␳�̃f�[�^�̐؂���
        add esi,edx         //����(�␳)
        test esi,0xffff00ff //�I�[�o�[�␳.
        jz  Non_G_Over
            mov esi,0x0000ff00  //255 �z���Ă���̂ŉ����߂�.
    Non_G_Over:
        or   eax,esi        //��������.
        //B�̏���.
            //���ꂪ�Ō�Ȃ̂ő䒠�́@��(ebx) �␳�f�[�^(ecx)�𒼐ڎg��.
        and ebx,0x000000ff  //���̃f�[�^�̐؂���
        and ecx,0x000000ff  //�␳�̃f�[�^�̐؂���
        add ecx,ebx         //����(�␳)
        test ecx,0xffffff00 //�I�[�o�[�␳.
        jz  Non_B_Over
            mov ecx,0x000000ff  //255 �z���Ă���̂ŉ����߂�.
    Non_B_Over:
        or   eax,ecx        //��������.
        //�������āA�쐬���� eax �������߂�.
        mov [edi],eax
    }
}

/*****************************************************************/
//�����̐F�𓾂�
/*****************************************************************/
inline long PGet(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
    //���낭���@����ǂ��v���O�����ł��߂�Ȃ�������
    _asm{
		mov eax,bmd1 
        mov ecx,ry1
		mov ebx,rx1
		mov esi,[eax + BMD_W_POSSITION]	// w �̈ʒu
		mov edi,[eax + BMD_H_POSSITION]	// h �̈ʒu
		mov edx,[eax + BMD_BM_POSSITION]	// bm �̈ʒu

		mov eax,0			//�߂�l�Ƃ��Ċm��.
		//�ȈՃN���b�s���O
		cmp esi,ebx	//w�`�F�b�N
		jb	NO_GET
		cmp edi,ecx	//h�`���b�N
		jb	NO_GET
		test ecx,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET
		test ebx,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET

        imul esi,ecx	//esi = ry1 * w
        add esi,ebx ;(rx1+ry1*bmd1->w)

        mov edi,esi ;*3 �� X<< 2 - X �Ōv�Z����
        shl edi,2
        sub edi,esi

        add edi,edx   ;�]���J�n�A�h���X
        mov eax,[edi]
        and eax,0x00ffffff
	NO_GET:
    }
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

/*****************************************************************/
//�_�C���N�g�ɂ����̐F�𓾂�
/*****************************************************************/
inline long DirectPGet(const BYTE *data1)
{
//	long w;
    _asm{
		mov	edi,data1

        mov eax,[edi]
        and eax,0x00ffffff
//        mov w,eax
    }
//    return w;
}

/*****************************************************************/
//���̒n�_�̃f�[�^�A�h���X���擾
/*****************************************************************/
inline BYTE* SeekBmAddress(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
	_asm
	{
		mov eax,bmd1 
		mov esi,rx1
        mov edi,ry1
		mov ecx,[eax + BMD_W_POSSITION]	// w �̈ʒu		
		mov ebx,[eax + BMD_H_POSSITION]	// h �̈ʒu		
		mov edx,[eax + BMD_BM_POSSITION]	// bm �̈ʒu

		//�ȈՃN���b�s���O
		cmp ecx,esi	//w�`�F�b�N
		jb	NO_GET
		cmp ebx,edi	//h�`���b�N
		jb	NO_GET
		test esi,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET
		test edi,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET

		imul	edi,ecx		//esi = ry1 * w
		add		edi,esi		//(rx1+ry1*bmd1->w)

		mov		esi,edi		//*3 �� X<< 2 - X �Ōv�Z����
		shl		esi,2
		sub		esi,edi

        add esi,edx   ;�]���J�n�A�h���X
        mov eax,esi
	NO_GET:
	}
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

/*****************************************************************/
//���̒n�_�̃f�[�^�A�h���X���擾Sp
/*****************************************************************/
inline BYTE* SeekBmAddressSp(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
	_asm
	{
		mov eax,bmd1 
		mov esi,rx1
        mov edi,ry1
		mov ecx,[eax + BMD_W_POSSITION]	// w �̈ʒu		
		mov ebx,[eax + BMD_H_POSSITION]	// h �̈ʒu		
		mov edx,[eax + BMD_BM_POSSITION]	// bm �̈ʒu

		//�ȈՃN���b�s���O
		cmp ecx,esi	//w�`�F�b�N
		jb	NO_GET
		cmp ebx,edi	//h�`���b�N
		jb	NO_GET
		test esi,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET
		mov ecx,[eax + BMD_POW2W_POSSITION]	// Pow2W �̈ʒu
		test edi,0x80000000	//�}�C�i�X�`�F�b�N(�@��ˑ�)
		jnz	NO_GET

		shl		edi,cl
//		imul	edi,ecx		//esi = ry1 * w
		add		edi,esi		//(rx1+ry1*bmd1->w)

		mov		esi,edi		//*3 �� X<< 2 - X �Ōv�Z����
		shl		esi,2
		sub		esi,edi

        add esi,edx   ;�]���J�n�A�h���X
        mov eax,esi
	NO_GET:
	}
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

inline void AddColor(DWORD* ioRGB , DWORD inAddRGB)
{
	int theR,theG,theB;
	int theR2,theG2,theB2;

	theR = ((*ioRGB)& 0x00ff0000) >> 16;
	theG = ((*ioRGB)& 0x0000ff00) >>  8;
	theB = ((*ioRGB)& 0x000000ff) ;

	theR2 = ((inAddRGB) >> 16) & 0xff;
	theG2 = ((inAddRGB) >>  8) & 0xff;
	theB2 = ((inAddRGB)      ) & 0xff;

	theR += theR2;	theG += theG2;	theB += theB2;
	
	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	*ioRGB = (theR << 16) + (theG << 8) + (theB);
}

//�߂�l�Ȃ��̌x��������ׂ点��.
#pragma warning(error: 4035)

_GRAPHICS_END   //namespace end


#endif


#endif //LIFE END
