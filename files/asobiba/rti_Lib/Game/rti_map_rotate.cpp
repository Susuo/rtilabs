#include "StartWithExeFile.h"
#ifdef ROTATE_MAP

#include <.\Game\rti_map_rotate.h>
#include <.\Graphics\rti_backsurface.h>

/**************************************************************/
//�}�b�v���G�悵�܂�(�~���X�N���[��)
/**************************************************************/
void TMapRotate::DrawMain(BMD bmd,int w,int h)
{
	int x,y;
    x = (MAP_FULLX - Nowx);
    y = (MAP_FULLY - Nowy);
    //�㉺�A���E�̊ђ�
  	if ( x < w && y < h )
    {
        //����
		MapDrawing(bmd,0,0,   Nowx,Nowy,  x,y);
        //�E��
		MapDrawing(bmd,x,0,   0,Nowy,  w - x,y);
        //����
		MapDrawing(bmd,0,y,   Nowx,0,  x,h - y);
        //�E��
		MapDrawing(bmd,x,y,   0,0,  w - x,h - y);
        return;
    }
    //���E�̊ђ�
	if ( x < w)
	{
        //��
		MapDrawing(bmd,0,0,  Nowx,Nowy,  x,h);
        //�E
		MapDrawing(bmd,x,0,  0,Nowy,  w - x,h);
        return;
	}
    //�㉺�̊ђ�
    if ( y < h)
    {
        //��
		MapDrawing(bmd,0,0,   Nowx,Nowy,  w,y);
        //��
		MapDrawing(bmd,0,y,   Nowx,0,  w , h - y);
        return;
    }

    //�ʏ�G��.
   	MapDrawing(bmd,0,0,Nowx,Nowy,w,h);
}

/**************************************************************/
//��l���ړ��Ȃǂɂ��}�b�v�X�N���[��(�~��)
/**************************************************************/
void TMapRotate::MapMasterMove(short nx,short ny,int speed)
{
	short lx,ly;  //�X�N���[�����W�ɕϊ�
	//�X�N���[�����W�ɕϊ�
	lx = GetScreenX(nx);
	ly = GetScreenY(ny);
    //���E
	if (ly >=(SCREENYSIZE/2)+NOMOVEY)		Nowy +=speed;
	if (ly <=(SCREENYSIZE/2)-NOMOVEY)	    Nowy -=speed;
    //�㉺
    if (lx >=(SCREENXSIZE/2)+NOMOVEX)		Nowx +=speed;
    if (lx <=(SCREENXSIZE/2)-NOMOVEX)		Nowx -=speed;
	//�␳
	MapHosei(&Nowx,&Nowy);
}
/**************************************************************/
//�~���X�N���[���̏ꍇ�̃L�����N�^�ʒu�̕␳
/**************************************************************/
void TMapRotate::MapHosei(int *x,int *y)
{
	if (*x > MAP_FULLX) *x -= MAP_FULLX;
	if (*y > MAP_FULLY) *y -= MAP_FULLY;
    if (*x < 0) *x += MAP_FULLX;
    if (*y < 0) *y += MAP_FULLY;
}
/**************************************************************/
//�~���X�N���[���̏ꍇ�̃L�����N�^�ʒu�̕␳(�I�[�i�[���[�h���҂��)
/**************************************************************/
void TMapRotate::MapHosei(short *x,short *y)
{
	if (*x > MAP_FULLX) *x -= MAP_FULLX;
	if (*y > MAP_FULLY) *y -= MAP_FULLY;
    if (*x < 0) *x += MAP_FULLX;
    if (*y < 0) *y += MAP_FULLY;
}

/**************************************************************/
//�w�肵���|�C���g����ʂ̒��S�ɂ���.
/**************************************************************/
void TMapRotate::SetCeneter(short nx,short ny)
{
	Nowx = nx - SCREENXSIZEHARF;
	Nowy = ny - SCREENYSIZEHARF;
	//�␳
	MapHosei(&Nowx , &Nowy);
}

/**************************************************************/
//���̃|�C���g��ʉ߂ł��邩�`�F�b�N
/**************************************************************/
bool TMapRotate::If(short nx,short ny)
{
	//�����Ŗ��ɂȂ�̂��A
	//�}�b�v�`�b�v���܂����ꍇ�ł���.
	//�Ƃ肠�����A���̂��ׂĂ̂܂����`�b�v�Ɣ�����Ƃ�.
	//�����A��ł��ړ��ł��Ȃ��`�b�v���܂������ƁA�v�ɂȂ�.
	short lx,ly;
	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//�␳
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//�␳
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//�␳
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//�␳
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	return true;
}

#endif //LIFE END
