/*****************************************************************/
/*****************************************************************/
// RTI's  Graphics Lib ... (C) Copylight 1999 ... 
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Comm\rti_Comm.h>
#include <math.h>

_GRAPHICS_BEGIN //namespace begin

/*****************************************************************/
//�ȈՃN���b�s���O
/*****************************************************************/
bool EasyClipingEx(BMD bmd,int *rx1,int *ry1,int *rw1,int *rh1,
				 int rw2,int rh2,int rx2,int ry2,
				 int *sw,int *sh,int *ew,int *eh)
{
    //�ȈՏI���ʒu�N���b�s���O
/*
    //�}�C�i�X������������␳.
    if(*rx1 < 0)
    {
        *rw1 += *rx1;  // *rx1 �̓}�C�i�X.
        *rx1 = 0;
    }
    //�}�C�i�X������������␳.
    if(*ry1 < 0)
    {
        *rh1 += *ry1;  // *ry1 �̓}�C�i�X.
        *ry1 = 0;
    }
    //�摜�̉������傫����΁@�v
    if(*rx1 >= bmd->w)  return false;
    //�摜�̍������傫����΁@�v
    if(*ry1 >= bmd->h)  return false;
    
    int p;
    p = rw2 - rx2 ;
    if (p < rw1)    *ew = p;
    else            *ew = rw1;
    if (rx2 < 0 ) *sw = abs(rx2) , *ew = *ew - *sw;
    else          *sw = 0;

    p = rh2 - ry2 ;
    if (p < rh1)    *eh = p;
    else            *eh = rh1;
    if (ry2 < 0 ) *sh = abs(ry2) , *eh = *eh - *sh;
    else          *sh = 0;

    if (*ew <= 0 || *eh <= 0 ) return false;
	return true;
*/
    bool r = false;
    _asm
    {
		mov			edi,bmd
       //�}�C�i�X������������␳.
	   mov         eax,dword ptr [rx1]
	   mov         ecx,dword ptr [rw1]
	   mov		   ebx,dword ptr [eax]	//rx1�̒��g
	   mov         edx,dword ptr [ecx]	//rw1�̒��g
  	   cmp         ebx,0
	   jge         NONE_RX1_HOSEI
			add			edx,ebx	//*rw1 += *rx1;  // *rx1 �̓}�C�i�X.
			mov			ebx,0	//*rx1 = 0;
			mov			[ecx],edx
			mov			[eax],ebx
	NONE_RX1_HOSEI:
		//�摜�̉������傫����΁@�v
		//if(*rx1 >= bmd->w)  return false;
		mov ecx,[edi+BMD_W_POSSITION]	//�@��ˑ�!!	bmd->w
		cmp	ebx,ecx
		jge	END_PROC	//�v

       //�}�C�i�X������������␳.
	   mov         eax,dword ptr [ry1]
	   mov         ecx,dword ptr [rh1]
	   mov		   ebx,dword ptr [eax]	//ry1�̒��g
	   mov         edx,dword ptr [ecx]	//rh1�̒��g
  	   cmp         ebx,0
	   jge         NONE_RY1_HOSEI
			add			edx,ebx	//*rh1 += *ry1;  // *ry1 �̓}�C�i�X.
			mov			ebx,0	//*ry1 = 0;
			mov			[ecx],edx
			mov			[eax],ebx
	NONE_RY1_HOSEI:
		//�摜�̉������傫����΁@�v
		//if(*rx1 >= bmd->h)  return false;
		mov ecx,[edi+BMD_H_POSSITION]	//�@��ˑ�!!	bmd->h
		cmp	ebx,ecx
		jge	END_PROC	//�v


        //���C�h�� ew
        mov eax,rw2 //eax �� rw2
        mov ebx,rx2 //ebx �� rx2
        mov ecx,rw1 // ecx �� rw1 �ɂ���.
		mov ecx,[ecx]	//rw1 �� �|�C���^�Ȃ̂Ŏ��ԉ�����
        sub eax,ebx // rw2 - rx2

        mov edi,ecx // edi�� *ew �̂��� �� rw1������
        cmp eax,ecx //p �� rw1 �̔�r
        jge W_NEXT1
            mov edi,eax //*ew �� p ������
    W_NEXT1:

        //���C�h�� sw
        mov esi,0 // esi�� *sw �̂��� �� 0������
        test ebx,0x80000000 // rx2 ���}�C�i�X ? 
        jz  W_NEXT2        //�}�C�i�X����Ȃ��Ȃ� 0 �ł悢�̂Ŕ�����
        neg  ebx            //rx2 �̐�Βl(�����ɂ���)
            mov esi,ebx         //*sw �ɓ����
            sub edi,ebx         //*ew = *ew - *sw
    W_NEXT2:
        //�������ɏ����߂�
        mov eax,ew
        mov ebx,sw
        mov dword ptr [eax],edi
        mov dword ptr [ebx],esi
            mov edx,edi  // �Ō�̔�r�p�� ew ��ۑ�(edx�͎g���Ă��Ȃ��̂�)

        //�w�C�g�� eh
        mov eax,rh2 //eax �� rh2
        mov ebx,ry2 //ebx �� ry2
        mov ecx,rh1 // ecx �� rh1 �ɂ���.
		mov ecx,[ecx]	//rh1 �� �|�C���^�Ȃ̂Ŏ��ԉ�����
        sub eax,ebx // rh2 - ry2

        mov edi,ecx // edi�� *eh �̂��� �� rh1������
        cmp eax,ecx //p �� rh1 �̔�r
        jge H_NEXT1
            mov edi,eax //*eh �� p ������
    H_NEXT1:

        //�w�C�g�� sh
        mov esi,0 // esi�� *sh �̂��� �� 0������
        test ebx,0x80000000 // ry2 ���}�C�i�X ? 
        jz  H_NEXT2        //�}�C�i�X����Ȃ��Ȃ� 0 �ł悢�̂Ŕ�����
        neg  ebx            //ry2 �̐�Βl(�����ɂ���)
            mov esi,ebx         //*sh �ɓ����
            sub edi,ebx         //*eh = *eh - *sh
    H_NEXT2:
        //�������ɏ����߂�
        mov eax,eh
        mov ebx,sh
        mov dword ptr [eax],edi
        mov dword ptr [ebx],esi

        //�G�悵�Ă����̂��Ȃ�...
	    //if (*ew <= 0 || *eh <= 0 ) return false;
        cmp edx,0
        jle END_PROC
        cmp edi,0
        jle END_PROC
            //�����������}
            mov r,1
    END_PROC:
    }
    return r;            
}


/*****************************************************************/
//�f�[�^�̓�����
/*****************************************************************/
BYTE *SeekData(BMD bmd,int rx,int ry,
			  int sw,int sh,int *lpAddPitch)
{
    *lpAddPitch = (bmd->w + bmd->w + bmd->w);
	return (bmd->bm+(rx+sw)*3+(ry+sh)* (*lpAddPitch) );

}




static void linerInterpolate(BMD bmd,Fixed x,Fixed y,int *rgb);
static BYTE linePol(int c1,int c2,int c3,int c4,Fixed xr,Fixed yr);



//���̃O���t�B�b�N�X�� sizex sizey �ɐ؂�􂢂��Ƃ��̌�
//�ƁAX�̐��AY�̐��� CX,CY�ɕԂ�
int GraphicsCutTile(BMD bmd,int sizex,int sizey,short *cx,short *cy)
{
	int t,TableSize;
    t = bmd->w / sizex;
    if (bmd->w % sizex) t ++;
    TableSize = (*cx) = t;

    t = bmd->h / sizey;
    if (bmd->h % sizey) t ++;
    TableSize *= (*cy = t);
	return TableSize;
}

/*****************************************************************/
//�g��k���@���`�⊮
/*****************************************************************/
void BigDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int rw2,int rh2)
{
    int x,y;
    Fixed fx,fy;
    Fixed sfx,sfy;
    int ew,eh;
    int rgb;

	if (rw2 != 0)
	{
		sfx = FixedDiv( IF(rw1) , IF(rw2) ) ; //�䗦�v�Z
	}
	else
	{
		sfx = 0;
	}
	if (rh2 != 0)
	{
	    sfy = FixedDiv( IF(rh1) , IF(rh2) ) ;
	}
	else
	{
		sfy = 0;
	}

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;

    ew = rw2-1;
    eh = rh2-1;
    for (y = 0 ,fy = ry1+1 ; y < eh-1 ;y ++ , fy += sfy)
    {
        for (x = 0 ,fx = rx1+1; x < ew-1 ;x ++ , fx += sfx)
        {
            //�擾
            linerInterpolate(bmd1,fx,fy,&rgb ); 
            //�]��
            PSet(bmd2,x+rx2,y+ry2,rgb);
        }
    }

	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

void linerInterpolate(BMD bmd,Fixed x,Fixed y,int *rgb)
{
    int px,py;
    Fixed xrate,yrate;
    BYTE r,g,b;
    long col1,col2,col3,col4;

    px =(int)Fixed2Int(x);
    py =(int)Fixed2Int(y);

    xrate = x-Int2Fixed(px);
    yrate = y-Int2Fixed(py);

    //�F�擾
    col1 = PGet(bmd,px+1,py-1);
    col2 = PGet(bmd,px+1,py+1);
    col3 = PGet(bmd,px-1,py-1);
    col4 = PGet(bmd,px-1,py+1);

    g = linePol( ((int)col1 & 0x000000ff) ,
                 ((int)col2 & 0x000000ff) ,
                 ((int)col3 & 0x000000ff) ,
                 ((int)col4 & 0x000000ff) ,
                 (xrate),(yrate));
    r = linePol( ((int)((col1 & 0x0000ff00) >> 8) ) ,
                 ((int)((col2 & 0x0000ff00) >> 8) ) ,
                 ((int)((col3 & 0x0000ff00) >> 8) ) ,
                 ((int)((col4 & 0x0000ff00) >> 8) ) ,
                 (xrate),(yrate) );
    b = linePol( ((int)((col1 & 0x00ff0000) >> 16) ) ,
                 ((int)((col2 & 0x00ff0000) >> 16) ) ,
                 ((int)((col3 & 0x00ff0000) >> 16) ) ,
                 ((int)((col4 & 0x00ff0000) >> 16) ) ,
                 (xrate),(yrate) );
    //�Ƃ肠���� (^^;
    *rgb = RTIRGB(r,g,b);
}


BYTE linePol(int c1,int c2,int c3,int c4,Fixed xr,Fixed yr)
{
    Fixed d,e,f;
    d = ( (IF(1)-(xr)) * c1) + (xr * c3);
    e = ( (IF(1)-(xr)) * c2) + (xr * c4);
    f = FixedMul(d,(Int2Fixed(1)-yr) )+ FixedMul(e,yr);
    return (BYTE)( Fixed2Int(FixedRound(f)) );
}


/*****************************************************************/
//�ڂ���
/*****************************************************************/
void ShadeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    _asm{
		dec ew ;������͂��炩���ߌ��Z���Ă���
		dec eh
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            push edx  //������Ɣq�� LOOP2�̒��Ŏg���̂�..
            
            mov ecx,ew
//            xor bx,bx    ;���ō�Ɨp�Ɏg���܂� LOOP2�̂��Ƃ�
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

//                mov edi,data1 ;�]����
//                mov esi,data2 ;�]����
				push ecx         ;������Ɣq��

				//��
				mov edx,edi
				sub edx,AddPitch1
                mov eax,[edx] ;�������ɂ��������܂�
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax
				//��
				mov edx,edi
				add edx,AddPitch1
                mov eax,[edx] ;�������ɂ��������܂�
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax
				//��
				mov edx,edi
				sub edx,3
                mov eax,[edx] ;�������ɂ��������܂�
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax
				//�E
				mov edx,edi
				sub edx,3
                mov eax,[edx] ;�������ɂ��������܂�
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax

                //0x000000ff�ɂ��Čv�Z
				//eax�͉��Z�悤�ł�
				pop eax ;�E�̗v�f
				pop ebx ;�E�̗v�f
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
				pop ebx ;�E�̗v�f
				pop edx ;�E�̗v�f  ;���̓�͂��Ƃō���
                sub esp,16    ;4*4�o�C�g(pop pop)�߂��Ă���
                and ebx,0x000000ff
                and edx,0x000000ff
                add ebx,edx
				add eax,ebx		   ;����
                shr eax,2          ;���ς��o��
                and eax,0x000000ff ;�O�̂��ߍăN���b�s���O
                mov ecx,eax        ;ecx�͍��v�悤�ɔq��

                //0x0000ff00�ɂ��Čv�Z
				//eax�͉��Z�悤�ł�
				pop eax ;�E�̗v�f
				pop ebx ;�E�̗v�f
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
				pop ebx ;�E�̗v�f
				pop edx ;�E�̗v�f  ;���̓�͂��Ƃō���
                sub esp,16    ;4*4�o�C�g(pop pop)�߂��Ă���
                and ebx,0x0000ff00
                and edx,0x0000ff00
                add ebx,edx		   ;����
				add eax,ebx		   ;����
                shr eax,2          ;���ς��o��
                and eax,0x0000ff00 ;�O�̂��ߍăN���b�s���O
                or  ecx,eax

                //0x00ff0000�ɂ��Čv�Z
				//eax�͉��Z�悤�ł�
				pop eax ;�E�̗v�f
				pop ebx ;�E�̗v�f
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
				pop ebx ;�E�̗v�f
				pop edx ;�E�̗v�f  ;���̓�͂��Ƃō���
                and ebx,0x00ff0000
                and edx,0x00ff0000
                add ebx,edx		   ;����
				add eax,ebx		   ;����
                shr eax,2          ;���ς��o��
                and eax,0x00ff0000 ;�O�̂��ߍăN���b�s���O
                or  ecx,eax

                mov ebx,[esi]      ;���܂������ʂ�
                and ebx,0xff000000
                or  ebx,ecx

                mov [esi],ebx

                add edi,3
                add esi,3

				pop ecx         ;������Ɣq�؂�߂�
            dec ecx
            cmp ecx,1
            ja  LOOP2       ;LOOP2�I�� ECX���

//            pop data2 ;�f�[�^���A
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y���v�f���v���X
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        cmp edx,1
        ja  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 1 ; y < eh-1 ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 1 ; x < ew-1 ;x ++ )
        {
            GetPoint = data1 - AddPitch1; //��
			B=  *(GetPoint);
			G=  *(GetPoint +1);
			R=  *(GetPoint +2);
            GetPoint = data1 + AddPitch1; //��
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
            GetPoint = data1 - 3;  //��
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
            GetPoint = data1 + 3; //�E
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
			
			R = R >> 2;
			G = G >> 2;
			B = B >> 2;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
*/
}

/*****************************************************************/
//������
/*****************************************************************/
void Blend50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            push edx  //������Ɣq�� LOOP2�̒��Ŏg���̂�..
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
                mov ebx,[esi]      ;
                test eax,0x00ffffff
                jz   NON_DRAW //�����͖���
				push ebx      //�������A�N�Z�X�͂Ȃ�ׂ��C��

                push eax
                push ebx

                //0x000000ff�ɂ��Čv�Z
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
                shr eax,1	;1�o�C�g�V�t�g / 2
                and eax,0x000000ff ;�O�̂��ߍăN���b�s���O
                mov edx,eax

                //0x0000ff00�ɂ��Čv�Z
                pop ebx
                pop eax
                sub esp,8    ;4*2�o�C�g(pop pop)�߂��Ă���
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
                shr eax,1   ;1�o�C�g�V�t�g / 2
                and eax,0x0000ff00 ;�O�̂��ߍăN���b�s���O
                or edx,eax

                //0x00ff0000�ɂ��Čv�Z
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
                shr eax,1   ;1�o�C�g�V�t�g /2 
                and eax,0x00ff0000 ;�O�̂��ߍăN���b�s���O
                or edx,eax

				pop ebx;���܂������ʂ�
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx
NON_DRAW:

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop edx
            pop esi
            pop edi
			//�摜�����̃��C����
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
            /*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
			B += *(data2) ;
            G += *(data2 +1) ;
			R += *(data2 +2) ;
            R = R >> 1;
            G = G >> 1;
            B = B >> 1;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
            */

}

/*****************************************************************/
//���ȏ�܂��͈ȉ�����
/*****************************************************************/
void BlendDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend)
{
    int x,y;

    Fixed frlend2,fglend2,fblend2;
    fblend2 = Int2Fixed(1) - fblend;
    fglend2 = Int2Fixed(1) - fglend;
    frlend2 = Int2Fixed(1) - frlend;

	int AddPitch1,AddPitch2;
    int R,G,B;
    int R2,G2,B2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);

			B2 = *(data2) ;
            G2 = *(data2 +1) ;
			R2 = *(data2 +2) ;
            B= (BYTE)Fixed2Int( ( fblend2*R2) + ( fblend*R) ) ;
            G= (BYTE)Fixed2Int( ( fglend2*G2) + ( fglend*G) ) ;
            R= (BYTE)Fixed2Int( ( frlend2*B2) + ( frlend*B) ) ;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//�O���f�[�V�����u�����h
/*****************************************************************/
void BlendGradetionDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
						BMD bmd2,int rx2,int ry2,
						Fixed fblend,Fixed fglend,Fixed frlend,
						unsigned char mode,int start,int size)
{
    int x,y;

    Fixed frlend2,fglend2,fblend2;
    Fixed frlend3,fglend3,fblend3;
    fblend2 = Int2Fixed(1) - fblend;
    fglend2 = Int2Fixed(1) - fglend;
    frlend2 = Int2Fixed(1) - frlend;

	int Gr ;
	int AddPitch1,AddPitch2;
    int R,G,B;
    int R2,G2,B2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

	frlend3 = frlend2;
	fglend3 = fglend2;
	fblend3 = fblend2;
    for (y = 0 ; y < eh ;y ++ )
    {
		frlend2 = frlend3;
		fglend2 = fglend3;
		fblend2 = fblend3;
		Gr = 0;
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);

			B2 = *(data2) ;
            G2 = *(data2 +1) ;
			R2 = *(data2 +2) ;
			if (x > start)
			{
				if (++Gr > size)
				{
					Gr = 0;
					frlend2 -= (2048);
					fglend2 -= (2048);
					fblend2 -= (2048);
					if (frlend2 < 0) frlend2 = 0;
					if (fglend2 < 0) fglend2 = 0;
					if (fblend2 < 0) fblend2 = 0;
				}
			}
		    frlend = Int2Fixed(1) - frlend2;
			fglend = Int2Fixed(1) - fglend2;
			fblend = Int2Fixed(1) - fblend2;

            B= (BYTE)Fixed2Int( (fblend2*R2) + (fblend*R) ) ;
            G= (BYTE)Fixed2Int( (fglend2*G2) + (fglend*G) ) ;
            R= (BYTE)Fixed2Int( (frlend2*B2) + (frlend*B) ) ;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//�����x����
/*****************************************************************/
void Bright50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*

    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
//            push data1   ;�f�[�^�Ҕ�
//            push data2
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

//                mov edi,data1 ;�]����
//                mov esi,data2 ;�]����
                
                mov eax,[edi] ;�������ɂ��������܂�
                push eax

                //0x000000ff�ɂ��Čv�Z
                and eax,0x000000ff
                shr eax,1
                and eax,0x000000ff ;�O�̂��ߍăN���b�s���O
                mov ebx,eax

                //0x0000ff00�ɂ��Čv�Z
                pop eax
                sub esp,4    ;4*1�o�C�g(pop pop)�߂��Ă���
                and eax,0x0000ff00
                shr eax,1   ;1�o�C�g�V�t�g
                and eax,0x0000ff00 ;�O�̂��ߍăN���b�s���O
                or ebx,eax

                //0x00ff0000�ɂ��Čv�Z
                pop eax
                and eax,0x00ff0000
                shr eax,1   ;1�o�C�g�V�t�g
                and eax,0x00ff0000 ;�O�̂��ߍăN���b�s���O
                or ebx,eax

                mov eax,[esi]      ;���܂������ʂ�
                and eax,0xff000000
                or  eax,ebx

                mov [esi],eax

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

//            pop data2 ;�f�[�^���A
//            pop data1
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y���v�f���v���X
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
            /*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
			B += *(data2) ;
            G += *(data2 +1) ;
			R += *(data2 +2) ;
            R = R >> 1;
            G = G >> 1;
            B = B >> 1;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
            */

}


/*****************************************************************/
//���x����
/*****************************************************************/
void BrightDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend)
{
    int x,y;

	int AddPitch1,AddPitch2;
    int R,G,B;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
            B= (fblend*R);
            G= (fglend*G);
            R= (frlend*B);

			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//���Z�A���t�@�u�����h �������̃V�t�g�����̎g����(����)
/*****************************************************************/
void AddDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
//            push data1   ;�f�[�^�Ҕ�
//            push data2
            push edi
            push esi
            push edx  //������Ɣq�� LOOP2�̒��Ŏg���̂�..
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

//                mov edi,data1 ;�]����
//                mov esi,data2 ;�]����
                
                mov eax,[edi] ;�������ɂ��������܂�
                mov ebx,[esi]      ;
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                and ebx,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax
                push ebx

                //0x000000ff�ɂ��Čv�Z
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
				cmp eax,0x000000ff
				jbe	None_Hosei1
					mov	eax,0x000000ff	//�␳
			None_Hosei1:
                mov edx,eax

                //0x0000ff00�ɂ��Čv�Z
                pop ebx
                pop eax
                sub esp,8    ;4*2�o�C�g(pop pop)�߂��Ă���
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
				cmp eax,0x0000ff00
				jbe	None_Hosei2
					mov	eax,0x0000ff00	//�␳
			None_Hosei2:
                or edx,eax

                //0x00ff0000�ɂ��Čv�Z
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
				cmp eax,0x00ff0000
				jbe	None_Hosei3
					mov	eax,0x00ff0000	//�␳
			None_Hosei3:
                or edx,eax

                mov ebx,[esi]      ;���܂������ʂ�
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

//            pop data2 ;�f�[�^���A
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y���v�f���v���X
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//���Z�A���t�@�u�����h
/*****************************************************************/
void SubDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,data2 ;�]����
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
//            push data1   ;�f�[�^�Ҕ�
//            push data2
            push edi
            push esi
            push edx  //������Ɣq�� LOOP2�̒��Ŏg���̂�..
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

//                mov edi,data1 ;�]����
//                mov esi,data2 ;�]����
                
                mov eax,[edi] ;�������ɂ��������܂�
                mov ebx,[esi]      ;
                and eax,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                and ebx,0x00ffffff ;�s�v�ȕ�����؂藎�Ƃ�
                push eax
                push ebx

                //0x000000ff�ɂ��Čv�Z
                and eax,0x000000ff
                and ebx,0x000000ff
                sub eax,ebx
                //�}�C�i�X�ɂ����񂾂Ƃ��̕␳
                jns None_Hosei1
                    mov     eax,0
            None_Hosei1:
                mov edx,eax

                //0x0000ff00�ɂ��Čv�Z
                pop ebx
                pop eax
                sub esp,8    ;4*2�o�C�g(pop pop)�߂��Ă���
                and eax,0x0000ff00
                and ebx,0x0000ff00
                sub eax,ebx
                //�}�C�i�X�ɂ����񂾂Ƃ��̕␳
                jns None_Hosei2
                    mov     eax,0
            None_Hosei2:
                or edx,eax

                //0x00ff0000�ɂ��Čv�Z
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                sub eax,ebx
                //�}�C�i�X�ɂ����񂾂Ƃ��̕␳
                jns None_Hosei3
                    mov     eax,0
            None_Hosei3:
                or edx,eax

                mov ebx,[esi]      ;���܂������ʂ�
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

//            pop data2 ;�f�[�^���A
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y���v�f���v���X
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//BitBlt �� AND ���� (^^;;  �ʉߏ����͂Ȃ��ł�.
/*****************************************************************/
void AndDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov edx,eh
        mov edi,data1 ;�]��
        mov esi,data2 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
                mov ebx,[esi]      ;�]���悩��Q�b�g

                or eax,0xff000000 //�]����ɂ��� ���8�r�b�g���󂳂Ȃ��悤��
                and eax,ebx       // And ����
                mov [esi],eax     //��������

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//BitBlt �� OR(PAINT) ���� (^^;;  �ʉߏ����͂Ȃ��ł�.
/*****************************************************************/
void OrDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov edx,eh
        mov edi,data1 ;�]��
        mov esi,data2 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
                mov ebx,[esi]      ;�]���悩��Q�b�g

                and eax,0x00ffffff //�]����ɂ��� ���8�r�b�g���󂳂Ȃ��悤��
                or eax,ebx       // Or ����
                mov [esi],eax     //��������

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//�ʉߖ����̊G��
/*****************************************************************/
void DirectDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int y;
//    int copysize;
//    int copysizemod;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;

 
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/

//    copysize = 3*(ew) ;
//    copysizemod = copysize % 4;
//    copysize = copysize / 4;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    for (int y = eh ; y >= 0 ;y -- )
    {
        //�ŋ��̍�����!? ��
        memcpy( (data2),(data1),3*(ew) );
		data1+=AddPitch1;
		data2+=AddPitch2;
    }
*/
  

    _asm{
        //�ł��邾���A dword �P�ʂœ]�����āA
        //�[���� byte �P�ʂœ]�����邽�߂̌v�Z.
        mov eax,ew    //3*(ew)  X*3 = X<<2 -X
        shl eax,2
        sub eax,ew   //eax�������ł�

        mov ecx,eax   //���Z�Ƃ��܂肪�K�v�Ȃ̂�.. 
        shr ecx,2     //eax / 4 �̂���.
        and eax,3     //eax % 4 �̂���.

        mov edx,eh
        mov esi,data1 ;�]����
        mov edi,data2 ;�]����
        cld
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            push ecx
            //�܂Ƃ߂ē]�����܂�
            rep movsd
            mov ecx,eax
            rep movsb

            pop ecx
            pop esi
            pop edi
            add esi,AddPitch1
            add edi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}




/*****************************************************************/
//���]�G��
/*****************************************************************/
void RevDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1)
{
//    int x,y;

	int AddPitch1;
//    BYTE *backupPoint;
	BYTE *data1;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    p = bmd1->w - rx1 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx1 < 0 ) sw = abs(rx1) , ew = ew - sw;
    else          sw = 0;

    p = bmd1->h - ry1 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry1 < 0 ) sh = abs(ry1) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
/*
    AddPitch1 = (bmd1->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
*/

    _asm{
        mov edx,eh
        mov edi,data1 ;�]����
        mov esi,AddPitch1 ;esi�����܂��Ă���̂Ŏg�� (^^;;
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi   ;�f�[�^�Ҕ�
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��


                mov eax,[edi] ;�������ɂ��������܂�
				mov ebx,eax   ;�Ҕ�����[��
				not eax       ;���]
                and eax,0x00ffffff ;�[�����Ă��Ƃ͓]�����Ȃ��Ă��悢
                and ebx,0xff000000
                or  ebx,eax
                mov [edi],ebx

                add edi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop edi ;�f�[�^���A

//            add edi,AddPitch1 ;Y���v�f���v���X
            add edi,esi ;Y���v�f���v���X
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y < eh ;y ++ )
    {
        backupPoint = data1; //�ۑ�
        for (x = 0 ; x < ew ;x ++ )
        {
            *(data1) =  ~(*(data1)); //���]�G��
            *(data1+1) =  ~(*(data1+1)); //���]�G��
            *(data1+2) =  ~(*(data1+2)); //���]�G��
            data1 += 3;
        }
        data1 = backupPoint; //���A
        data1 += AddPitch1;
    }
*/
}



/*****************************************************************/
//�ʉߐF����ŊG��
/*****************************************************************/
void Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;�]��
        mov esi,data2 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
//            push data1   ;�f�[�^�Ҕ�
//            push data2
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

//                mov edi,data1 ;�]��

                mov eax,[edi] ;�������ɂ��������܂�

                and eax,0x00ffffff ;�[�����Ă��Ƃ͓]�����Ȃ��Ă��悢
                jz NON_PROC        ;and�̓t���O���ω�����̂�cmp���� 

                ;�����ɂ����Ƃ������Ƃ͓����F�ł͂Ȃ��̂œ]��
//                mov esi,data2 ;�]��

                mov ebx,[esi]      ;���܂������ʂ�
                and ebx,0xff000000
                or  ebx,eax

                mov [esi],ebx

            NON_PROC: ;�����F�������肵���炱���ɂ��邩��

//              add data1,3
//              add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop esi
            pop edi
//            pop data2 ;�f�[�^���A
//            pop data1

//            mov ebx,AddPitch1 ;Y���v�f���v���X
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
            if (   ( ((DWORD)*((DWORD*)(data1))) & 0xffffff00 )  )
            {
//				*(data2) = *(data1);
//				*(data2+1) = *(data1+1);
//				*(data2+2) = *(data1+2);
				memcpy(data2,data1,3);
            }
            data1+=3;
            data2+=3;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
*/
}

/*****************************************************************/
//�N���A
/*****************************************************************/
void Cls(BMD bmd,BYTE clear)
{
	if ( !CommDraw->GraphicsLock(bmd) )	return;
	BYTE *dest = bmd->bm;
	int count = (bmd->w * bmd->h);
	count = count + count + count;	//count = (bmd->w * bmd->h) * 3 �̊|���Z�͑����Z��.

	int c = clear;
	_asm
	{
		mov edi,dest
		mov edx,count
		//eax �� c c c c �Ɣz�u���Ă����܂�.
		mov ebx,c
		mov eax,ebx	//�����͈ړ��œ]���˂�
		shl ebx,8	//4���炵��
		or  eax,ebx //����
		shl ebx,8	//�ȉ�����
		or  eax,ebx
		shl ebx,8
		or  eax,ebx
		//������ eax �� c c c c �ƂȂ��Ă���͂��ł�.

		//�ł́A�]���̎n�܂�ł�.
		cld	//�������]��
		mov ecx,edi	//4 �Ŋ���؂�邩�ǂ������ׂ܂�.
		and ecx,3	// dest �̃A�h���X & 3 
		sub edx,ecx	//�]������o�C�g�����炻�̕�������
		js	END_OF_RTN
		
		rep stosb
		//���݂� 4 �Ŋ���؂��A�h���X�ɂ��܂�.
		//��������Acount / 4 �̒l�� DWORD �P�ʂœ]�����܂�.
		mov ecx,edx
		mov ebx,ecx
		shr ecx,2	// / 2 �̂��Ƃł�.
		and ebx,3	// & 3 �̂��Ƃł�.
		rep stosd	// DWORD �]��
		mov ecx,ebx
		rep stosb	//BYTE�]��
	END_OF_RTN:
	}
 
	CommDraw->GraphicsUnLock(bmd);
}

/*****************************************************************/
//����`��
/*****************************************************************/
void Line(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb)
{
	int AddPicth;
	BYTE *data1;
    int w,h;

    w = bmd1->w;
    h = bmd1->h;
    //�N���b�s���O
    if (rx1 < 0) rx1 = 0;
    if (ry1 < 0) rx1 = 0;
    if (rx1 > w) rx1 = w;
    if (ry1 > h) rx1 = h;
    if (rx2 < 0) rx2 = 0;
    if (ry2 < 0) rx2 = 0;
    if (rx2 > w) rx2 = w;
    if (ry2 > h) rx2 = h;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    data1 = bmd1->bm;
    AddPicth = bmd1->w * 3;

    int dx,dy ,sx,sy,x,y;
    int i,e;

    dx = abs(rx2 - rx1);                   // �ω��� X
    dy = abs(ry2 - ry1);                   // �ω��� Y
    sx = (rx2 - rx1) >= 0 ? 1 : -1;        // ��:1 ��:-1 ��:0
    sy = (ry2 - ry1) >= 0 ? 1 : -1;        // ��:1 ��:-1 ��:0
    
    x = rx1;
    y = ry1;

    if (dy < dx)
    {                    // �_���ɂ��Ȃ���
        //X ���� �� ��������ꍇ
        e = -dx;
        for( i = 0 ; i < dx ; i++)
        {
            //PSet (x * tx, y * ty), QBColor(c)
            PSet(bmd1  , x , y , rgb );
            x = x + sx;                 // X ����
            e = e + 2 * dy;             // DDA �ɂ����
            if (e >= 0) 
            {
                y = y + sy ;            // Y����
                e = e - 2 * dx;         // DDA �ɂ����
            }
        }
    }
    else
    {
        // Y ���� �� ��������ꍇ
        e = -dy;
        for( i = 0 ; i < dy ; i ++)
        {
            //PSet (x * tx, y * ty), QBColor(c)
            PSet(bmd1  , x , y , rgb );
            y = y + sy;                  // Y ����
            e = e + 2 * dx;              // DDA �ɂ����
            if (e >= 0) 
            {
                x = x + sx ;             // X ����
                e = e - 2 * dy;          // DDA �ɂ����
            }
        }
    }

	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//�l�p��`��
/*****************************************************************/
void Box(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb)
{
    int AddPitch1;
	BYTE *backupdata;
	BYTE *data1;
    int w,h;
    int ew,eh;
    int x,y;

    w = bmd1->w;
    h = bmd1->h;
    rx1--; ry2 ++;
    //�N���b�s���O
    if (rx1 < 0) rx1 = 0;
    if (ry1 < 0) ry1 = 0;
    if (rx1 > w) rx1 = w;
    if (ry1 > h) ry1 = h;
    if (rx2 < 0) rx2 = rx1;
    if (ry2 < 0) ry2 = ry1;
    if (rx2 > w) rx2 = w;
    if (ry2 > h) ry2 = h;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    AddPitch1 = (w * 3);
	data1= (bmd1->bm+(rx1*3)+(ry1*AddPitch1) );
//    rgb = (rgb << 8); //���炩���߃V�t�g���Ă���
    ew = abs(rx2 - rx1); eh = abs(ry2 - ry1);

    for (y = 0 ; y < eh ; y ++)
    {
        backupdata = data1;
        for (x = 0 ; x < ew ; x ++)
        { 
//            PSet(bmd1,x+rx1,y+ry1,rgb);
			DirectPSet(data1,rgb);
            data1+=3;
        }
        data1 = backupdata;
        data1 += AddPitch1;
    }
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//�~��`��
/*****************************************************************/
void Circle(BMD bmd1,int rx1,int ry1,int rr1,long rgb)
{

    int r;
    int x,y;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    for (r = 0 ; r < 1024 ; r++)
    {
        x = (int)(LTBSin1024[r]*rr1 +rx1);
        y = (int)(LTBCos1024[r]*rr1 +ry1);
        //�N���b�s���O (^^;;
        if (x >= 0 && x <= bmd1->w && y >= 0 && y <= bmd1->h)
        {
            PSet(bmd1,x,y,rgb);
        }
    }
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//���E�㉺���]
/*****************************************************************/
void MillorDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,bool lr,bool ud)
{
    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;
    int countX; // X ��1�i�񂾂Ƃ��̃A�h���X�̈ړ���
    int lr_millor_start_end_x;//���E���]�̏ꍇ�͎n�܂�͒[����

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    rh1 --; rw1 --;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/

    if (lr)
    {
        countX = -3 ;
        lr_millor_start_end_x = AddPitch1-3;
    }
    else
    {
        countX = 3;
        lr_millor_start_end_x = 0; 
    }
    if (ud)
    { //�㉺���]�́A�摜�A�h���X���ŉ��w��Y�Ɉړ������Ă��炷�[����
        data1 += eh * AddPitch1; 
        //�㉺���]�̂��߂ɕ������]
        AddPitch1 = -AddPitch1;
    }

    for (y = 0 ; y <= eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        data1 += lr_millor_start_end_x;
        for (x = 0 ; x <= ew ;x ++ )
        {
			*(data2) = *(data1);
            *(data2 +1) = *(data1+1);
			*(data2 +2) = *(data1+2);
            data1 += countX ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//�w�肵���͈͂Ƀ��U�C�N��������
/*****************************************************************/
void MosaicDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,int mw,int mh)
{
//    int x,y;
//    int wx,wy;
    int mwskip,mhskip;

	int AddPitch1;
//    BYTE *backupPoint;
//    BYTE *mbackupPoint;
//    BYTE *workbackupPoint;
	BYTE *data1;
//    BYTE R,G,B;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
/*
    //�ȈՏI���ʒu�N���b�s���O
    int sw,sh,ew,eh,p;
    rh1 --; rw1 --;
    p = bmd1->w - rx1 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx1 < 0 ) sw = abs(rx1) , ew = ew - sw;
    else          sw = 0;

    p = bmd1->h - ry1 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry1 < 0 ) sh = abs(ry1) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
/*
    AddPitch1 = (bmd1->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
*/
    mwskip = mw * 3;
    mhskip = mh * AddPitch1;
    int mwstep ,mhstep;


    _asm{
        mov edx,0
        mov edi,data1 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
//            push data1   ;�f�[�^�Ҕ�
            push edi   ;�f�[�^�Ҕ�
            
            mov ecx,0
            xor bx,bx    ;���ō�Ɨp�Ɏg���܂� LOOP2�̂��Ƃ�
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                ;�͈̓I�[�o�[�͋����Ȃ�
//                if ( (mw + x) >= ew)     mwstep = (mw+x)-ew;
//                else                     mwstep = mw;
                mov eax,mw
                add eax,ecx  ;ecx �� x�̂��Ƃł�
                cmp eax,ew
                jb ELSE1
                    sub eax,ew
                    jmp ENDIF1
                ELSE1:
                    mov eax,mw
            ENDIF1:
                mov mwstep,eax

                ;��������2��if���ł�
//                if ( (mh + y) >= eh)     mhstep = (mh+y)-eh;
//                else                     mhstep = mh;
                mov eax,mh
                add eax,edx  ;edx �� y�̂��Ƃł�
                cmp eax,eh
                jb ELSE2
                    sub eax,eh
                    jmp ENDIF2
                ELSE2:
                    mov eax,mh
            ENDIF2:
                mov mhstep,eax

                ;�͈̓I�[�o�[�`�F�b�N�I��

//                mov edi,data1 ;�]����
                mov eax,[edi] ;�������ɂ��������܂�
                and eax,0x00ffffff ;�s�v�ȃf�[�^�̐؂藎�Ƃ�

                ;���[�v�J�E���^�[�̂�����Ɣq��
                push edx
                push ecx
//                push data1
                push edi


                //eax �̒��g�������܂���܂�
                //�������[�v�X�^�[�g
                mov edx,0
                LOOP3:              ;���[�v�J�E���^�� edx ���g�p��
//                    push data1   ;�f�[�^�Ҕ�
                    push edi   ;�f�[�^�Ҕ�
            
                    mov ecx,0
                    LOOP4:           ;���[�v�J�E���^�� ecx ���g�p��

                        ;�ȑO�ɗp�ӂ��� eax�̓��e�������܂���
//                        mov edi,data1 ;�]��
                        mov ebx,[edi]      ;���܂������ʂ�
                        and ebx,0xff000000
                        or  ebx,eax
                        mov [edi],ebx

//                        add data1,3
                        add edi,3
                    inc ecx
                    cmp ecx,mwstep
                    jb  LOOP4       ;LOOP2�I�� ECX���

//                    pop data1         ;�f�[�^�𕜋A
                    pop edi         ;�f�[�^�𕜋A

//                    mov ebx,AddPitch1 ;Y���v�f���v���X
//                    add data1,ebx
                    add edi,AddPitch1
                inc edx             ;LOOP1�I�� EDX���
                cmp edx,mhstep
                jb  LOOP3
                ;�������[�v�I��
                ;���[�v�J�E���^�[�̂�����Ɣq�؂�߂�

//                pop data1
                pop edi
                pop ecx
                pop edx

//                mov eax,mwskip
//                add data1,eax
                add edi,mwskip
            add ecx,mw
            cmp ecx,ew
            jb  LOOP2       ;LOOP2�I�� ECX���

//            pop data1 ;�f�[�^���A
            pop edi ;�f�[�^���A

//            mov eax,mh
//            mov ebx,mhskip ;Y���v�f���v���X
//            add data1,ebx
            add edi,mhskip
        add edx,mh             ;LOOP1�I�� EDX���
        cmp edx,eh
        jb  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y <= eh ;y +=mh )
    {
        backupPoint = data1; //�ۑ�
        for (x = 0 ; x <= ew ;x +=mw )
        {
            R =*(data1);
            G =*(data1+1);
            B =*(data1+2);
            mbackupPoint = data1; //�ۑ�
            if ( (mw + x) >= ew)     mwstep = (mw+x)-ew;
            else                     mwstep = mw;
            if ( (mh + y) >= eh)     mhstep = (mh+y)-eh;
            else                     mhstep = mh;
            //���U�C�N����������
            for (wy = 0 ; wy < mhstep ;wy ++ )
            {
                workbackupPoint = data1; //�ۑ�
                for (wx = 0 ; wx < mwstep ;wx ++ )
                {
                    *(data1) =   R; //
                    *(data1+1) = G; //
                    *(data1+2) = B; //
                    data1 += 3;
                }
                data1 = workbackupPoint; //���A
                data1 += AddPitch1;
            }

            data1 = mbackupPoint; //���A
            data1 += mwskip ;
        }
        data1 = backupPoint; //���A
        data1 += mhskip;
    }
*/
}






/*****************************************************************/
//�t�B���^ �w�肵���͈͂Ƀt�B���^��������
/*****************************************************************/
void FilterDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      int R,int G,int B)
{
	int AddPitch1;
	BYTE *data1;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);

    int RAdd,GAdd,BAdd;
    RAdd = (R<<16);
    GAdd = (G<<8 );
    BAdd = (B    );

    _asm{

        mov edx,eh
        mov edi,data1 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
                mov esi,eax //esi ���䒠
                and eax,0xff000000 //��������eax �ɂǂ�ǂ񑫂�����ł���.

                //R�̏���
                mov ebx,esi
                and ebx,0x00ff0000
                add ebx,RAdd
                js R_END_STEP      //�}�C�i�X�ɂ����ł���̂łȂɂ����Ȃ�(������0�Ȃ̂� 0 ���킴�킴�������K�v�Ȃ�)

                test ebx,0xff00ffff
                jz R_WRITE_STEP      //�������ݏ���
                    mov ebx,0x00ff0000 //�ۂ߂�
            R_WRITE_STEP:
                or eax,ebx // R �̏�������
            R_END_STEP: //R�̏����̏I��

                //G�̏���
                mov ebx,esi
                and ebx,0x0000ff00
                add ebx,GAdd
                js G_END_STEP      //�}�C�i�X�ɂ����ł���̂łȂɂ����Ȃ�(������0�Ȃ̂� 0 ���킴�킴�������K�v�Ȃ�)

                test ebx,0xffff00ff
                jz G_WRITE_STEP      //�������ݏ���
                    mov ebx,0x0000ff00 //�ۂ߂�
            G_WRITE_STEP:
                or eax,ebx // G �̏�������
            G_END_STEP: //R�̏����̏I��

                //B�̏���
                mov ebx,esi
                and ebx,0x000000ff
                add ebx,BAdd
                js B_END_STEP      //�}�C�i�X�ɂ����ł���̂łȂɂ����Ȃ�(������0�Ȃ̂� 0 ���킴�킴�������K�v�Ȃ�)

                test ebx,0xffffff00
                jz B_WRITE_STEP      //�������ݏ���
                    mov ebx,0x000000ff //�ۂ߂�
            B_WRITE_STEP:
                or eax,ebx // G �̏�������
            B_END_STEP: //R�̏����̏I��

                //�������ɏ����߂�
                mov [edi],eax

                add edi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop edi
            add edi,AddPitch1
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//�V�t�g�G�� ���V�t�g�̗v�f�ł�. (EL �͐Ԃ����ǂ� )
// �܂��A S �́A 0 �` 31 �̒l���Ƃ�܂�.
// ����ȏ�͂���
/*****************************************************************/
void ShiftDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int S)
{

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //�ȈՃN���b�s���O
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov ebx,eh     // �����ł����Z���g�����߂� ebx �ɂ��Ă���
                       //����킵�����A��邵�Ă���
        mov edi,data1 ;�]��
        mov esi,data2 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
                and eax,0x000000ff //�s�v�ȕ������E��
                jz END_STEP        //���͖���

                sub eax,S     //�V�t�g�l�����Z����.
                js  END_STEP       //���̐��������疳��

                cmp eax,ebx  //eax - eh(�̎c��)
                ja  END_STEP // > ��������@����

                mul AddPitch2
                add eax,esi     // eax �̕������炷

                mov edx,[eax]      //�G�敔�����擾.
                and edx,0x00ffffff //�s�v�ȕ����𗎂Ƃ�

                mov eax,[esi]      //�������ޕ������擾
                and eax,0xff000000 
                or  eax,edx        //����ʂ�����
                mov [esi],eax      //�G��

            END_STEP:

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec ebx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//��]
/*****************************************************************/
void RotateDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,Angle seeta)
{
    int x,y;
    int rx,ry;
	int tx,ty;
    int min_x,max_x,min_y,max_y;
	Fixed Sin,Cos;	//�T�C���R�T�C���̃e���|�������ɂ�[
	Fixed RSin,RCos;//�T�C���R�T�C���̃e���|�������ɂ�[

	int AddPitch1,AddPitch2;
	BYTE *BackupPoint2;
	BYTE *data1,*data2;

	//�T�C�� �R�T�C�����擾!!
	Sin = FixedLTBSin1024[ __ANGLE(seeta) ];
	Cos = FixedLTBCos1024[ __ANGLE(seeta) ];
	//�t�T�C�� �t�R�T�C�����擾!!
	RSin = FixedLTBSin1024[__ANGLE_REV(seeta)];
	RCos = FixedLTBCos1024[__ANGLE_REV(seeta)];
    //�e�N�X�`���̊e���_�� jiku �𒆐S�ɉ�]�����܂���.
	//����ɂ���āA�ő�A�Œ�̓]���͈͂��擾���܂���.
	/*
		min
		------------------------
        |                      |
        |                      |
        |          jiku        |
        |                      |
        |                      |
        |                      |
		------------------------
								max
    */
	//����
    tx = x = rx1 - jikux;
    ty = y = ry1 - jikuy;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    min_x = max_x = rx ;
    min_y = max_y = ry ;
	//�E��
    tx = x + rw1;
    ty = y ;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
	//����
    tx = x ;
    ty = y + rh1;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
	//�E��
    tx = x + rw1;
    ty = y + rh1;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
    //min ���珉�߂ā@max �܂Ńf�[�^��]�����܂�.
    //�ł��A���̑O�Ɂ@�󂯎�鑤�̊J�n�A�h���X�����߂܂�.
    //min max �V���[�Y�͎����W�ł�.
    //�����������(�قǂ�ǁ@�}�C�i�X�Ȃ̂ň����Z������)�]���J�n�ꏊ���Ƃ�.
    x = rx2 + min_x;    
    y = ry2 + min_y;    
	max_x ++ ; max_y++;

	//�N���b�s���O
	if (x < 0)
	{		x = 0;		min_x = -rx2;	}
	//�N���b�s���O
	if (y < 0)
	{		y = 0;		min_y = -ry2;	}
	//�N���b�s���O
	if (x+(max_x - min_x) > bmd2->w)
	{		max_x = bmd2->w - x + min_x - 1;	}
	if (y+(max_y - min_y) > bmd2->h)
	{		max_y = bmd2->h - y + min_y - 1;	}
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	//�]���J�n�A�h���X�擾
	data1 = SeekData(bmd1,rx1,ry1, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,x,y, 0,0,&AddPitch2);

	//�f�o�b�O�p(�]���͈͂𔒂��{�b�N�X�œh��Ԃ�)
//	Box(Sprite,rx2 + min_x,ry2 + min_y,rx2 + max_x,ry2 + max_y,0xffffff);
	Fixed Xx,Xy;
	Fixed Yx,Yy;
	//�����l�����߂�.
	Yx = (min_x * RCos)-(min_y * RSin);
	Yy = (min_x * RSin)+(min_y * RCos);
	//�e�N�X�`�������f���ĉ�ʂɐ����ɓ\��t���܂�.
	//�������邱�Ƃɂ���āA�t�������ттттттЂттттттт��ƁA����܂�.
	//�Ȋw�̏����ł�.(�Ӗ��Ȃ�)
	/*

		Texter             Screen
	|---------------|      |-----------------------|
    |  -^           |      |   ------------------> |
    | |             |  ->  |   ------------------> |
    |-              |      |   ------------------> |
    |---------------|      |                       |
     �΂߂ɉ��f            |                       |
                           -------------------------
						   �܂������G��
    */
	//�]���J�n
    for (y = min_y ; y <= max_y ;y ++ )
    {
		Xx = Yx;
		Xy = Yy;
        BackupPoint2 = data2;
        for (x = min_x ; x <= max_x ;x ++ )
        {
			//��].
			rx = FI(Xx) +jikux;
			ry = FI(Xy) +jikuy;
    		//�N���b�s���O����
			if (rx>=0 && rx<rw1 && ry>=0 && ry<rh1 )
            {
		    //�]��
				DirectPSet(data2 , data1+(rx+rx+rx)+ry*AddPitch1);
            }
			Xx += RCos;
			Xy += RSin;
            data2 += 3 ;
        }
		Yx -= RSin;
		Yy += RCos;
        data2 = BackupPoint2 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//3D��]
/*****************************************************************/
void Rotate3DDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,int jikuz,
					  unsigned char seetax,unsigned char seetay,unsigned char seetaz)
{
}

/*****************************************************************/
//�O���C�X�P�[���ŊG��
/*****************************************************************/
void GrayScaleDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;

 
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
	/*
	*	RGB to YIQ �J���[	(C MAGAZIN 2000-2���� �A���S���Y�����{���)
	*
	*  Y     (0.299   0.587   0.114)(R)
	*  I  =  (0.596  -0.274  -0.322)(G)
	*  Q     (0.211  -0.522   0.322)(B)
	*
	*  �ŁA�O���[�X�P�[���ɂ���ɂ́A Y ���������߂�΂悢�����Ȃ̂�(��)
	*  Y = 0.299 * R + 0.587 * B + 0.114 * G �����܂�.
	*  ���̂Ƃ��A���������_���ł�Ƃ���Ȃ̂ŁA�S�̂� * 256 �{���܂�.
	*  Y = (77 * R + 28 * B  + 151 * G ) / 256 
	*  �Ō�� 256 �́A>> 8 �ő�p����Ƃ���(��)  
	*  ���̌v�Z���ɂ���� Y ���������߂āA�O���[�X�P�[���ɂ��܂�.
	*/
    _asm{
        mov edx,eh
        mov edi,data1 ;�]��
        mov esi,data2 ;�]��
    LOOP1:              ;���[�v�J�E���^�� edx ���g�p��
            push edi
            push esi
			push edx	;���[�v�J�E���^�[ edx ��������Ɣq�؂��܂�.      
            mov ecx,ew
        LOOP2:           ;���[�v�J�E���^�� ecx ���g�p��

                mov eax,[edi] ;�������ɂ��������܂�
				//���ݎ��R�Ɏg����̂́A eax ebx edx �̓�����ł�. (�������ɂ�[)
				// eax �́A(��)�I���W�i���̕ۑ�.
				// ebx �́A���[�N�G���A.
				// edx �́A�v�Z����(Y�v�f)�����߂Ă��܂�.

				//B �����߂܂�.
				mov ebx,eax				//�Ƃ肠�����Aebx (�v�Z�̈�Ɉړ�.)
				and ebx,0x000000ff		//�s�v�ȕ�����؂藎�Ƃ��܂�.
				imul edx,ebx,28				//B�̌v�Z.//edx �ɂǂ�ǂ�v�f�����߂Ă����܂�.

				//G�����߂܂�.
				mov ebx,eax				//�Ƃ肠�����Aebx (�v�Z�̈�Ɉړ�.)
				and ebx,0x0000ff00		//�s�v�ȕ�����؂藎�Ƃ��܂�.
				shr ebx,8
				imul ebx,151			//G�̌v�Z.
				add  edx,ebx			//edx �ɂǂ�ǂ�v�f�����߂Ă����܂�.
				//R�����߂܂�.
				mov ebx,eax				//�Ƃ肠�����Aebx (�v�Z�̈�Ɉړ�.)
				and ebx,0x00ff0000		//�s�v�ȕ�����؂藎�Ƃ��܂�.
				shr ebx,16
				imul ebx,77				//R�̌v�Z.
				add  edx,ebx			//edx �ɂǂ�ǂ�v�f�����߂Ă����܂�.

				shr  edx,8				//�Ō�� / 256 ���܂�.
				mov  eax,edx
				shl  eax,8
				or   edx,eax
				shl  eax,8
				or   edx,eax

                and edx,0x00ffffff ;�v��Ȃ��Ƃ��������т�.
                mov ebx,[esi]       //�悩��Q�b�g.
                and ebx,0xff000000	//��̂Ƃ���ŏ����Ă͂����Ȃ��Ƃ�����N���A.
                or  ebx,edx			//����
                mov [esi],ebx		//��������

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2�I�� ECX���

			pop edx			 ;���[�v�J�E���^�[ edx ��q�؂��Ă����̂����Ƃɖ߂��܂�.
            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

_GRAPHICS_END   //namespace end


#endif //LIFE END
