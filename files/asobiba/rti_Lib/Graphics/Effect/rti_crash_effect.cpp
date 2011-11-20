/*****************************************************************/
/*****************************************************************/
//�@�h�炷 �������Ȃ�`
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef CRASH_LIFE

#define CRASH_RETURN_X  0x01
#define CRASH_RETURN_Y  0x02

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_crash_effect.h>

/******************************************************************/
//�R���X�g���N�^
/******************************************************************/
TCrashEffect::TCrashEffect()
{
}

/******************************************************************/
//�f�X�g���N�^
/******************************************************************/
TCrashEffect::~TCrashEffect()
{
}

/******************************************************************/
//�Z�b�g
/******************************************************************/
void TCrashEffect::Set(BMD _bmd,unsigned int _speedx,unsigned int _speedy,
                        unsigned int _shiftx,unsigned int _shifty)
{
    OutBmd = _bmd;
    Y = X = 0;
    Flg = 0;
    //�}�C�i�X�����ւ̃V�t�g���������Ă��Ȃ��̂Ń}�C�i�X�ɂ���.
    ShiftX = - (int)_shiftx;
    ShiftY = - (int)_shifty;
	Speedx = - (int)_speedx;
	Speedy = - (int)_speedy;

}

/******************************************************************/
//�N���b�V�� (false �������炨���܂�)
/******************************************************************/
bool TCrashEffect::Draw()
{
    bool ret = true;

    if ( CrashMove(&X,&Speedx,ShiftX,CRASH_RETURN_X) &
         CrashMove(&Y,&Speedy,ShiftY,CRASH_RETURN_Y)   ) ret = false;

    //���炵�ĊG��
    MoveDraw(OutBmd,X,Y);
    return ret;
}

/******************************************************************/
//�N���b�V���ړ�
/******************************************************************/
bool TCrashEffect::CrashMove(int *_P,int *_Speed,
                             int _Shift,unsigned char ReturnFlg)
{
    //�ړ�
    *_P += *_Speed;
    if (Flg & ReturnFlg)
    {
        //�߂肾��[��
        if (*_P >= 0)
        {
            //X�̕␳
            *_P = 0;
            return true;
        }
    }
    else
    {
        //�s������[��
        if (*_P <= _Shift)
        {   //�w�肵���ʒu����������.
            //���]
            *_Speed = - *_Speed;
            //�߂�ɓ���
            Flg |= ReturnFlg;
            //X�̕␳
            *_P = _Shift;
        }
    }
    return false;
}

/*****************************************************************/
//�G���w�肵���ʒu�Ɉړ������� �c��͋󔒂Ŗ��߂�.
/*****************************************************************/
void TCrashEffect::MoveDraw(BMD bmd1,int rx2,int ry2)
{

    int w = bmd1->w;
    int h = bmd1->h;
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    //�g���A�G�Y�A�}�C�i�X�����̗h��ɂ����Ή����Ă܂��� m(__)m
    if (rx2 > 0 ) rx2 = 0;
    if (ry2 > 0 ) ry2 = 0;

    int sw,sh,ew,eh,rx1,ry1,rw1,rh1;
    rx1 = ry1 = 0;    rw1 = bmd1->w; rh1 = bmd1->h;
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;

	CommDraw->GraphicsLock(bmd1);
	CommDraw->GraphicsLock(bmd2);
	data1 = SeekData(bmd1,0,0, sw,sh,&AddPitch1);
	data2 = SeekData(bmd1,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        //�ł��邾���A dword �P�ʂœ]�����āA
        //�[���� byte �P�ʂœ]�����邽�߂̌v�Z.
        mov eax,ew    //3*(ew)  X*3 = X<<2 -X
        mov ebx,eax
        shl eax,2
        sub eax,ebx   //eax�������ł�

        //���炵�����Ƃɂ��]���̃T�C�Y�����߂�
        mov ebx,AddPitch1   
        sub ebx,eax         //����ŁA ebx �ɗ]���̃T�C�Y���͂���܂���.

        //�G�� DWORD BYTE �ɕ�������.
        mov ecx,eax   //���Z�Ƃ��܂肪�K�v�Ȃ̂�.. 
        shr ecx,2     //eax / 4 �̂���. DWORD
        and eax,3     //eax % 4 �̂���. BYTE

        //���炵�����c�������̂悢�]�����������ɂ�[
        //���W�X�^�[������Ȃ��̂� EAX �̏�ʂ��؂��(^^;;;; << ���Ԃˁ[
        mov edx,ebx
        shr ebx,2     //ebx / 4 �̂���. DWORD
        and edx,3     //ebx % 4 �̂���. BYTE
        shl edx,16      //�����܂ňړ�����
        or  eax,edx     //eax �̏�ʂ����p����.
        //-----------EAX----------------
        //���:�]����BYTE | ���� �G��BYTE

        mov edx,eh
        mov esi,data1 ;�]����
        mov edi,data2 ;�]����
        cld
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push esi
            push ecx
            push eax
            //�G���܂Ƃ߂ē]�����܂�
            rep movsd       //DOWRD �]��
            mov ecx,eax
            and ecx,0x0000ffff  //��ʂ��}�X�N
            rep movsb       //BYTE �]��
            //�]���������܂�.
            mov ecx,eax
            shr ecx,16  //eax �̏�ʂɗ]���̃o�C�g�T�C�Y������̂�.
            mov eax,0   //eax ���N���A
            rep stosb       //�s���ɂ�� ���BYTE�]��.
            mov ecx,ebx
            rep stosd       //DWORD �]��.

            pop eax     //eax���A
            pop ecx
            pop esi

            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1

        //���̋󔒂𖄂߂܂�.
        mov ecx,h       //�G�̍���
        sub ecx,eh      //���]�����������߂܂�.
        imul ecx,AddPitch1  // ����̒��������߂܂�.
        mov ebx,ecx   //���Z�Ƃ��܂肪�K�v�Ȃ̂�.. 
        shr ecx,2     //ecx / 4 �̂���.
        and ebx,3     //ecx % 4 �̂���.

        mov eax,0
        rep stosd       //DWORD �]��

        mov ecx,ebx
        rep stosd       //BYTE �]��
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}


#endif  //LIFE END
