#include <StartWithExeFile.h>
#ifdef GLOOKUP_LIFE


#ifndef ___GLOOKUPH
#define ___GLOOKUPH

#include <.\Base\rti_fixed.h>
#include <windows.h>

//�f�B�O���[���烉�W�A���ɕϊ�
#define RRR(i) ( ((double)i) * 3.141592 / 180.f)
#define RRR1024(i) ( ((double)i) * 3.141592 / 512.f)
//���W�A������f�B�O���[�ɕϊ�
#define DDD(i) ( ((double)i) * 180.f / 3.141592 )
#define DDD1024(i) ( ((double)i) * 512.f / 3.141592 )

//�s�������_�ɃL���X�g(�Œ菬���_���g���o���O�̈�Y :-)
#define F(X) (double)(X)

//�p�C
#define PAI F(3.14159265358979323846)


//����
#define SWAP(x,y,t) t = x, x = y , y = t
//���X���b�v
#define ASM_SWAP(x,y) \
{ \
    _asm mov eax,x \
    _asm mov ebx,y \
    _asm mov x,ebx \
    _asm mov y,eax \
} 

#define BOXCHECK(x,y,sx,sy,ex,ey) (x>=sx && x<=ex && y>=sy && y<=ey)
extern double LTBSin1024[1024];
extern double LTBCos1024[1024];

void lookup_making();
unsigned long fast_hypot(int x,int y);
double fast_hypot_double(int x,int y);
void path_finder(int nx,int ny,int gx,int gy,int *sx,int *sy,int speed);
void path_finder3D(int nx,int ny,int nz,
				   int gx,int gy,int gz,
				   int *sx,int *sy,int *sz,int speed);
unsigned long farst_hypot3D(long lx,long ly,long lz);
BOOL CheckPow2(unsigned char *const p, unsigned long const dwArg_c);

bool HitCheck(int nx,int ny,int nw,int nh,
			  int gx,int gy,int gw,int gh,int faj);
bool HitCheck3D(int nx,int ny,int nz,
				int nw,int nh,int nd,
			    int gx,int gy,int gz,
				int gw,int gh,int gd,
				int faj);
bool HitCheckP(int nx,int ny,int gx,int gy,int faj);
bool HitCheckP3D(int nx,int ny,int nz,
			    int gx,int gy,int gz,
				int faj);
#define FAJEQU(mx,ex,faj) (((mx) > (ex)-(faj)) && ((mx) < (ex)+(faj)))




//100�p�[�Z���g�̂����� value ���Ȃ��Ă��邩�ǂ���
#define rand100(value) (((int)(rand() % 100)) <= (value))
//min����max �܂ł̗�����Ԃ�
#define randX(min,max) (((int)(rand() % ((unsigned)(  (max)-(min) +1)) ) ) +min)
//0 �� 1 ��Ԃ��܂�
#define randTF() (rand() & 0x00000001)

//�����Ă�����ɐi��
#define ANGLE_MOVEX(R,p) (LTBSin1024[R]*p)
#define ANGLE_MOVEY(R,p) (LTBCos1024[R]*p)

//��]����
#define ROTATEX1024(x,y,seeta) \
    ( (x)*LTBCos1024[ __ANGLE(seeta) ]+(y)*LTBSin1024[ __ANGLE(seeta) ])
#define ROTATEY1024(x,y,seeta) \
    ( (x)*LTBSin1024[ __ANGLE(seeta) ]-(y)*LTBCos1024[ __ANGLE(seeta) ])

//��]
#define SIMPLE_ROTATEX1024(x,seeta) ((x)*LTBSin1024[ __ANGLE(seeta) ])
#define SIMPLE_ROTATEY1024(y,seeta) (-(y)*LTBCos1024[ __ANGLE(seeta) ])
//1024 To 360
#define DEG(x) (__ANGLE( (Angle1024)((x)*2.844444)) )

extern Fixed  FixedLTBSin1024[1024];
extern Fixed  FixedLTBCos1024[1024];

#define GETBIT(x,n) (x & (0x1<<n) )         //�r�b�g�e�X�g�̌���
#define SETBIT(x,n) (x |= (0x1<<n) )        //�r�b�g�ɃZ�b�g
#define RESETBIT(x,n) (x &= (~(0x1<<n)) )   //����r�b�g���Z�b�g


#endif


#endif  //LIFE END

