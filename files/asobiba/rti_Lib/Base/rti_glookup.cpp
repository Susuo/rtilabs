#include <StartWithExeFile.h>
#ifdef GLOOKUP_LIFE

#include <.\Base\rti_glookup.h>
#include <.\Base\rti_debug.h>
#include <math.h>
//***************************************************
//���b�N�A�b�v�e�[�u���Ǝg���Ǝg���ƕ֗��ȕϐ��Q
//�Œ菬���_
//�ړ��ʂ⋗���Ȃǂ����߂�藝
//�������[�`���Ȃ�
//          Thank You For C MAGAZIN 
//          �������F ��  �b����ɂ��ŐV�A���S���Y�����T
//***************************************************


double LTBSin1024[1024];
double LTBCos1024[1024];
Fixed  FixedLTBSin1024[1024];
Fixed  FixedLTBCos1024[1024];

//***************************************************
//���b�N�A�b�v�e�[�u���쐬
//***************************************************
void lookup_making()
{

    int i;
    for (i = 0 ; i < 1024 ; i ++) 
    {
        LTBSin1024[i] = sin(RRR1024(i));
        LTBCos1024[i] = cos(RRR1024(i));
        FixedLTBSin1024[i] = Float2Fixed(LTBSin1024[i] );
        FixedLTBCos1024[i] = Float2Fixed(LTBCos1024[i] );
    }
    
}

//***************************************************
//hypot�֐��̍�����
//***************************************************
double fast_hypot_double(int x,int y)
{
    double len1,len2 ;

    x = abs(x);
    y = abs(y);
    if (x >= y)
    {
        len1 = x ; len2 = y;
    }
    else
    {
        len1 = y ; len2 = x;
    }
    return 0.9604 + len1 + 0.3978 *  len2;
}

//***************************************************
//hypot�֐��̍����Ԃ�int�� �ő�
//***************************************************
unsigned long fast_hypot(int x,int y)
{
/*
    unsigned long  len1,len2 ,t;

    x = abs(x);
    y = abs(y);
    if (x >= y)
    {
        len1 = x ; len2 = y;
    }
    else
    {
        len1 = y ; len2 = x;
    }
    t = len2 + (len2 >> 1);
    return (len1 - (len1 >> 5) - (len1 >> 7) + (t >> 2) + (t >> 6) );
*/
	unsigned long vret;
	_asm{
//    x = abs(x);
	    ;��Βl�����Ƃ߂�
		mov eax,x
		test eax,0x80000000
		jz NON1
		neg eax
	NON1:

//    y = abs(y);
		;��Βl�����Ƃ߂�
		mov ebx,y
		test ebx,0x80000000
		jz NON2
		neg ebx
	NON2:
//    if (x >= y)
//    {
//        len1 = x ; len2 = y;
//    }
//    else
//    {
//        len1 = y ; len2 = x;
//    }
	// len1 = eax  len2 = ebx
		cmp eax,ebx
		jae STEP2
		xchg eax,ebx //����

	STEP2:
//    t = len2 + (len2 >> 1);
		mov edi,ebx //len2����
		shr edi,1   //len2 >> 1
		add edi,ebx //len2 +
	//edi �́A t �ł�.

//    return (len1 - (len1 >> 5) - (len1 >> 7) + (t >> 2) + (t >> 6) );
		mov esi,eax //len1 ���� ���^�[���̒l�� esi �ɍ��܂�

		mov edx,eax //(len1 >> 5)
		shr edx,5
		sub esi,edx

		mov eax,7  //(len1 >> 7) //�����Alen1�͎Q�Ƃ��Ȃ��̂Ŕj�󂷂�
		sub esi,eax

		mov edx,edi//(t >> 2)
		shr edx,2
		add esi,edx

		shr edi,6  //(t >> 6) //�����At�͎Q�Ƃ��Ȃ��̂Ŕj�󂷂�
		add esi,edi
		mov vret,esi
	}
	return vret;
}


//***************************************************
//(nx,ny)����(gx,gy)��speed �ňړ����������̈ړ���(sx,sy)�����߂�
//***************************************************
void path_finder(int nx,int ny,int gx,int gy,int *sx,int *sy,int speed)
{
    int x =  gx - nx;
    int y =  gy - ny;

	double seeta;
	seeta = atan2(y,x);
    *sx = (int) (cos( seeta )* speed) ;
    *sy = (int) (sin( seeta )* speed) ;
}
//***************************************************
//�O���̈ړ���
//***************************************************
void path_finder3D(int nx,int ny,int nz,
				   int gx,int gy,int gz,
				   int *sx,int *sy,int *sz,int speed)
{
	int x = gx - nx;
	int y = gy - ny;
	int z = gz - nz;
	int n = farst_hypot3D(x,y,z) / speed;
    if (n)
    {
	    *sx = x / n;
	    *sy = y / n;
	    *sz = z / n;
    }
    else
    {
	    *sx = x ;
	    *sy = y ;
	    *sz = z ;
    }
}

//***************************************************
//�O���̋������҂��  sqrt(lx^2 + ly ^2 + lz ^2)
//***************************************************
unsigned long farst_hypot3D(long lx,long ly,long lz)
{
	long len1,len2,len3,len,temp;
	len1 = abs(lx);
	len2 = abs(ly);
	len3 = abs(lz);

	if (len1 < len2)
		SWAP(len1,len2,temp);
	if (len2 < len3)
		SWAP(len2,len3,temp);
	if (len1 < len2)
		SWAP(len1,len2,temp);
	len = (len1 >> 1) + (len1 >> 2) + (len1 >> 3) + (len1 >> 4)
		+ (len2 >> 2) + (len2 >> 3) + (len2 >> 6) 
		+ (len3 >> 2) + (len3 >> 5) + (len3 >> 6) ;
	return len;
}


//***************************************************
//���@(nx,ny)���G(gx,gy)�Ƃ̂����蔻�� faj �͗e�F�덷
//***************************************************
bool HitCheck(int nx,int ny,int nw,int nh,
			  int gx,int gy,int gw,int gh,int faj)
{
    bool hit = false;
    _asm
    {
        //faj �����炩���߃��W�X�^��.
        mov esi,faj
        //���𒲂ׂ�
        mov eax,nx
        sub eax,gx //sa = (short)(nx - gx);
        jns NON1    //���̐��ɂ�����ł��Ȃ����`�F�b�N
	       //���̐��Ȃ̂Ő�Βl�����߂�
		neg eax     //�������](�����ɂ���)
        //���� eax (nx - gx) �̌��ʂ��A���ƌ덷�̍��v���
        //��������΁A������@�ƁA���肵�܂�.
        //�������A���������A���ƌ덷(nw + esi) �����߂āA
        //�v�Z������A eax����A���������� 1 �X�e�b�v
        //�P�`�邱�Ƃ��ł��܂� (^^;;
        sub eax,nw  //��
        sub eax,esi //���Ɍ덷�������� esi = faj
        jns  ENDPPROC //�͂���Ă܂�.
        jmp NEXT_STEP //���͍����̔���ł�.
	NON1: //nx - gx �̌��ʂ́@������������.
        //���� eax (nx - gx) �̌��ʂ��A���ƌ덷�̍��v���
        //��������΁A������@�ƁA���肵�܂�.
        //�������A���������A���ƌ덷(gw + esi) �����߂āA
        //�v�Z������A eax����A���������� 1 �X�e�b�v
        //�P�`�邱�Ƃ��ł��܂� (^^;;
        sub eax,gw  //��
        sub eax,esi //���Ɍ덷�������� esi = faj
        jns  ENDPPROC //�͂���Ă܂�.

    NEXT_STEP:
        //�����𒲂ׂ�
        mov eax,ny
        sub eax,gy //sa = (short)(ny - gy);
        jns NON2    //���̐��ɂ�����ł��Ȃ����`�F�b�N
	       //���̐��Ȃ̂Ő�Βl�����߂�
		neg eax     //�������](�����ɂ���)
        //���� eax (ny - gy) �̌��ʂ��A�c�ƌ덷�̍��v���
        //��������΁A������@�ƁA���肵�܂�.
        //�������A���������A�c�ƌ덷(nh + esi) �����߂āA
        //�v�Z������A eax����A���������� 1 �X�e�b�v
        //�P�`�邱�Ƃ��ł��܂� (^^;;
        sub eax,nh  //�c 
        sub eax,esi  //�c�Ɍ덷�������� esi = faj
        jns  ENDPPROC //�͂���Ă܂�.
        jmp TRUE_STEP //�q�b�g���Ă܂���!
	NON2: //nx - gx �̌��ʂ́@������������.
        //���� eax (ny - gy) �̌��ʂ��A�c�ƌ덷�̍��v���
        //��������΁A������@�ƁA���肵�܂�.
        //�������A���������A�c�ƌ덷(gh + esi) �����߂āA
        //�v�Z������A eax����A���������� 1 �X�e�b�v
        //�P�`�邱�Ƃ��ł��܂� (^^;;
        sub eax,gh  //�c 
        sub eax,esi  //�c�Ɍ덷�������� esi = faj
        jns  ENDPPROC //�͂���Ă܂�.

    TRUE_STEP:
        //�����ɂ�����A�q�b�g���Ă���.
        mov hit,1   //�q�b�g���Ă����!!
    ENDPPROC:
        //�����܂�
    }
    return hit;
/*
	short sa;
	sa = (short)(nx - gx);
	if (sa < 0)
	{
		if ( abs(sa) > nw + faj) return false;
	}
	else
	{
		if ( abs(sa) > gw + faj) return false;
	}

	sa = (short)(ny - gy);
	if (sa < 0)
	{
		if ( abs(sa) > nh + faj) return false;
	}
	else
	{
		if ( abs(sa) > gh + faj) return false;
	}
    return true;
*/
}
//***************************************************
//���@(nx,ny,nz)���G(gx,gy,gz)�Ƃ̂����蔻�� faj �͗e�F�덷
//***************************************************
bool HitCheck3D(int nx,int ny,int nz,
				int nw,int nh,int nd,
			    int gx,int gy,int gz,
				int gw,int gh,int gd,
				int faj)
{
	short sa;
	sa = (short)(nx - gx);
	if (sa < 0)
		if ( abs(sa) > nw + faj) return false;
	else
		if ( abs(sa) > gw + faj) return false;

	sa = (short)(ny - gy);
	if (sa < 0)
		if ( abs(sa) > nh + faj) return false;
	else
		if ( abs(sa) > gh + faj) return false;

    sa = (short)(nz - gz);
	if (sa < 0)
		if ( abs(sa) > nd + faj) return false;
	else
		if ( abs(sa) > gd + faj) return false;
	return true;
}
//***************************************************
//���@(nx,ny)���G(gx,gy)�Ƃ̂����蔻�� faj �͗e�F�덷
//***************************************************
bool HitCheckP(int nx,int ny,int gx,int gy,int faj)
{
/*
	short sa;
	sa = (short)(nx - gx);
	if (abs(sa) > faj) return false;

	sa = (short)(ny - gy);
	if (abs(sa) > faj) return false;
    return true;
*/
/*
	bool bret;
	_asm{
		mov bret,1

		mov eax,nx //sa = (short)(nx - gx);
		sub eax,gx
		;��Βl�����Ƃ߂�    //abs(sa)
		test eax,0x80000000
		jz NON1
		neg eax
	NON1:
		cmp eax,faj
		ja FALSE_RETURN_RTN

		mov eax,ny //sa = (short)(ny - gy);
		sub eax,gy
		;��Βl�����Ƃ߂�    //abs(sa)
		test eax,0x80000000
		jz NON2
		neg eax
	NON2:
		cmp eax,faj
		jbe TRUE_RETURN_RTN
FALSE_RETURN_RTN:
		mov bret,0
TRUE_RETURN_RTN:
	}
	return bret;
*/
	bool bret;
	_asm{
		mov bret,0

		mov eax,nx //sa = (short)(nx - gx);
		sub eax,gx
		;��Βl�����Ƃ߂�    //abs(sa)
		jns NON1
		neg eax
	NON1:
		cmp eax,faj
		ja RETURN_RTN

		mov eax,ny //sa = (short)(ny - gy);
		sub eax,gy
		;��Βl�����Ƃ߂�    //abs(sa)
		jns NON2
		neg eax
	NON2:
		cmp eax,faj
		ja RETURN_RTN
		mov bret,1
RETURN_RTN:
	}
	return bret;

}


//***************************************************
//���@(nx,ny,nz)���G(gx,gy,gz)�Ƃ̂����蔻�� faj �͗e�F�덷
//***************************************************
bool HitCheckP3D(int nx,int ny,int nz,
			    int gx,int gy,int gz,
				int faj)
{
	short sa;
	sa = (short)(nx - gx);
	if (abs(sa) > faj) return false;

	sa = (short)(ny - gy);
	if (abs(sa) > faj) return false;

    sa = (short)(nz - gz);
	if (abs(sa) > faj) return false;

	return true;
}

//***************************************************
//�e�N�X�`���[�̕� dwArg_c �� 2�� n��ɂȂ��Ă��邩�ǂ�������ׂ�
//p �ɁA n��� n �̕�����Ԃ�
//***************************************************
BOOL CheckPow2(unsigned char *const p, unsigned long const dwArg_c)
{
	unsigned long dwWHSize,bFastBit1,dwBitNum,i;
	//�}�b�v���Q�̂���ɂȂ��Ă��邩�ǂ����𒲂ׂ�

	dwWHSize=dwArg_c;
	for(bFastBit1=dwBitNum=i=0 ; i<32 ; i++)
	{
		/*�����Ă���r�b�g�̌��𐔂���*/
		if((dwWHSize&(1<<i)) == (unsigned long)(1<<i))
		{
			dwBitNum++;
			if(dwBitNum == 1)
			{
				bFastBit1=i;
			}
		}
	}
	if(dwBitNum != 1)
	{
		//�}�b�v���Q�̂���ł͂Ȃ�
		*p = 0;
		return(FALSE);
	}

	*p=(unsigned char)bFastBit1;
	return TRUE;
}

#endif //LIFE END

