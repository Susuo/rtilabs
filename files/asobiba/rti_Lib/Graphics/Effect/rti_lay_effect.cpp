/*****************************************************************/
/*****************************************************************/
//�����̂悤��??? ������
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef LAY_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_lay_effect.h>

TLayEffect::TLayEffect()
{
}

TLayEffect::~TLayEffect()
{
}

void TLayEffect::Set(BMD _bmd1,int _x1,int _y1,int _w1,int _h1,
                    BMD _bmd2,int _x2,int _y2,unsigned char _kind,int _speed)
{
    X1 = _x1;
    Y1 = _y1;
    W1 = _w1;
    H1 = _h1;
    X2 = _x2;
    Y2 = _y2; 
    Bmd1 = _bmd1;
    Bmd2 = _bmd2;
    Kind = _kind;
    Speed = _speed;

    Possition = 0;
}

void TLayEffect::Draw()
{
    switch(Kind)
    {
    case LAY_LEFT:
    case LAY_RIGHT:
        if (!PickDrawLR() ) return ;
        break;
    case LAY_UP:
    case LAY_DOWN:
        if (!PickDrawUD() ) return ;
        break;
    }
    Possition+=Speed;
}


bool TLayEffect::PickDrawLR()
{
    BYTE *data1,*data2;
    int x1,y1,x2,y2;
    int sw,sh,ew,eh;
    int AddPitch1,AddPitch2;

    int rx1,ry1;
    rx1 = ry1 = 0;
	if (!EasyClipingEx(Bmd1,&rx1,&ry1,&W1,&H1,
						Bmd2->w,Bmd2->h,X2,Y2,
						&sw,&sh,&ew,&eh) )			return false;
    switch(Kind)
    {
    case LAY_LEFT:
        x1 = X1 + Possition;
        y1 = Y1;
        x2 = 0;
        y2 = Y2;
        ew = X2 + Possition;
        if (Bmd1->w <= x1 ) return false;
        break;
    case LAY_RIGHT:
        x1 = X1 + Possition;
        y1 = Y1;
        x2 = X2 + Possition;
        y2 = Y2;
        ew = Bmd2->w - x2;
        ::Draw(Bmd1,X1,Y1,Possition,H1,Bmd2,X2,Y2);
        if (Bmd1->w <= x1 ) return false;
        break;
    }
	if ( !CommDraw->GraphicsLock(Bmd1) )	return false;
	if ( !CommDraw->GraphicsLock(Bmd2) )	return false;
	data1 = SeekData(Bmd1,x1,y1, sw,sh,&AddPitch1);
	data2 = SeekData(Bmd2,x2,y2, sw,sh,&AddPitch2);

    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            mov eax,[edi] ;�������ɂ��������܂�
            and eax,0x00ffffff  //�s�v�ȕ����̂���ʂ�
            jz  NON_TRANCE      //�[���������獕�Ȃ̂Ŗ���
            
                push esi
                mov ecx,ew
            LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��
                
                    mov ebx,[esi]       //�]������擾
                    and ebx,0xff000000  //����������Ƃ�����N���A����
                    or  ebx,eax         //����

                    mov [esi],ebx       //����������.
                    add esi,3
                dec ecx
                jnz  LOOP2       ;LOOP2�I�� ECX���
            pop esi
        NON_TRANCE:

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(Bmd2);
	CommDraw->GraphicsUnLock(Bmd1);
    return true;
}

bool TLayEffect::PickDrawUD()
{
    BYTE *data1,*data2;
    int x1,y1,x2,y2;
    int sw,sh,ew,eh;
    int AddPitch1,AddPitch2;

    int rx1,ry1;
    rx1 = ry1 = 0;
	if (!EasyClipingEx(Bmd1,&rx1,&ry1,&W1,&H1,
						Bmd2->w,Bmd2->h,X2,Y2,
						&sw,&sh,&ew,&eh) )			return false;
    switch(Kind)
    {
    case LAY_UP:
        x1 = X1;
        y1 = Y1 + Possition;
        x2 = Y2;
        y2 = 0;
        eh = Y2 + Possition;
        if (Bmd1->h <= y1 ) return false;
        break;
    case LAY_DOWN:
        x1 = X1;
        y1 = Y1 + Possition;
        x2 = X2;
        y2 = Y2 + Possition;
        eh = Bmd2->h - y2;
        ::Draw(Bmd1,X1,Y1,W1,Possition,Bmd2,X2,Y2);
        if (Bmd1->h <= y1 ) return false;
        break;
    }
	if ( !CommDraw->GraphicsLock(Bmd1) )	return false;
	if ( !CommDraw->GraphicsLock(Bmd2) )	return false;
	data1 = SeekData(Bmd1,x1,y1, sw,sh,&AddPitch1);
	data2 = SeekData(Bmd2,x2,y2, sw,sh,&AddPitch2);

    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
           push edi
           push esi
           mov ecx,ew
            LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��
                    mov eax,[edi] ;�������ɂ��������܂�
                    and eax,0x00ffffff  //�s�v�ȕ����̂���ʂ�
                    jz  NON_TRANCE      //�[���������獕�Ȃ̂Ŗ���
                
                    mov ebx,[esi]       //�]������擾
                    and ebx,0xff000000  //����������Ƃ�����N���A����
                    or  ebx,eax         //����

                    mov [esi],ebx       //����������.
                NON_TRANCE:
                    add edi,3
                    add esi,3
                dec ecx
                jnz  LOOP2       ;LOOP2�I�� ECX���
            pop esi
            pop edi

            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(Bmd2);
	CommDraw->GraphicsUnLock(Bmd1);
    return true;
}

#endif  //LIFE END
