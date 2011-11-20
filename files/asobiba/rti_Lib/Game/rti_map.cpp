#include "StartWithExeFile.h"
#ifdef BASE_MAP


#include <.\Game\rti_map.h>
#include <.\Base\rti_error_message.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_debug.h>
#include <stdio.h>
/*

	���e�B's Map Liblary R.T.I (C) copylight since 1999...

*/



/************************************************************************************/
//�}�b�v���[�h
/************************************************************************************/
bool TMap::Read(int no)
{
	FILE *fp;
	char heder[sizeof(MAP_HEDER)];
	char filename[256];
	int filenamesize;

	//�t�@�C�����쐬.
	sprintf(filename,MAP_FILENAME,no);

	MapNo = no;
	ChipFileName = NULL;
	Map = NULL;
	Chip = NULL;

    try
    {
	    //�t�@�C����ǂݍ��݂܂�
	    fp = fopen(filename,"rb");
	    if (fp == NULL)
	    {
            throw FileNotOpen("TMap::Read",filename);
    	}
    	//�w�b�_��ǂݍ��݌���
    	fread(heder,sizeof(MAP_HEDER),sizeof(char),fp); //�w�b�_
	    if (lstrcmp(heder,MAP_HEDER) != 0)
	    {
            throw FriendlyNameError("TMap::Read",filename,"�w�b�_�����������ɂ�","����ȃt�@�C���ɍ����ւ��Ă�������");
    	}
    	filenamesize = fgetc(fp); //BMP�̃t�@�C�����̒���
    	//�t�@�C�����i�[
    	ChipFileName = (char*) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,filenamesize +1);
    	if (ChipFileName == NULL) 
    	{
            throw FriendlyNameError("TMap::Read",filename,"BMP�t�@�C�������i�[���郁�����m�ۂɎ��s�ɂ�","�t�@�C���������������A������������܂���");
	    }
	    fread(ChipFileName, filenamesize,sizeof(char),fp); //BMP�t�@�C����

        //�g�p����}�b�v�`�b�vBMP�t�@�C�����`����
	    Chip = BmpChar->Make(ChipFileName);
	    if (Chip == NULL) 
	    {
            throw FriendlyNameError("TMap::Read",filename,"�w�肳�ꂽ BMP���ǂ߂܂���.","�t�@�C���������������AMAP_CHIP�t�@�C��������܂���.");
    	}
        int UseMemroy = sizeof(short) * (MapW * MapH)+1;
        //X Y ��ǂݍ���
    	fread(&(MapW),1,sizeof(short),fp); //X�T�C�Y
    	fread(&(MapH),1,sizeof(short),fp); //Y�T�C�Y
        //�}�b�v�`�b�v�p�̃������m��
    	Map = (short*) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, UseMemroy);
    	if (Map == NULL) 
    	{
            throw MemoryDeficient("TMap::Read","MAP�̈�쐬",UseMemroy);
    	}
    }
    catch(...)
    {
        FriendlyNameError("TMap::Read",filename,"���̃}�b�v�t�@�C���ǂݍ��݂͒f�O����܂���.","");
    	Destroy();
	    if (fp != NULL)	fclose(fp);
	    return false;
    }
    //�}�b�v��ǂݍ���
    fread(Map,(MapW * MapH),sizeof(short),fp);
	fclose(fp);
	return true;

}


/************************************************************************************/
//�}�b�v�f�[�^�̔j��
/************************************************************************************/
void TMap::Destroy()
{
	if (ChipFileName != NULL)		GlobalFree(ChipFileName);
	if (Map != NULL)				GlobalFree(Map);
	if (Chip != NULL)				BmpChar->Breaker(Chip); 
	ChipFileName = NULL;
	Map = NULL;
	Chip = NULL;
}


/***********************************************************/
//�}�b�v�̊O�ɔ�яo���Ȃ��悤��... ��яo���Ă����� true
/***********************************************************/
bool TMap::Clipping(int x,int y,int w,int h)
{
    return (x <= 0-w || x >= MAP_STOP_XSCROLL ||
					y <= 0-h || y >= MAP_STOP_YSCROLL );
}

/***********************************************************/
//���͕\������Ă��Ȃ��̂���? ����ĂȂ� true
/***********************************************************/
bool TMap::IamNotDisplay(short x,short y,short faj)
{
    x -= Nowx;
    y -= Nowy;
    return (x < -faj || x > SCREENXSIZE+faj || y < -faj || y > SCREENYSIZE+faj);
}


/**************************************************************/
//�}�b�v �`��
/**************************************************************/
void TMap::MapDrawing(BMD bmd,int Dx,int Dy,int Sx,int Sy,int W,int H)
{
	short wchip;
	short *wmap;

	wmap = Map; //������...���Ǝv��.

	//�}�b�v���G�悵�܂�
	short lx,ly,sx,sy;
	lx = Sx >> MAP_CHIP_POW_SIZE;
	ly = Sy >> MAP_CHIP_POW_SIZE;
	sx = Sx % MAP_CHIP_SIZE;
	sy = Sy % MAP_CHIP_SIZE;
	short x,y,ix,iy;
	for (y = 0 ,iy = ly ;	y <= H ;	    y +=MAP_CHIP_SIZE , iy++ )
	{
		for (x = 0,ix = lx ; x <= W ;	x +=MAP_CHIP_SIZE ,ix ++)
		{
            //�}�b�v�`�b�v�G��
			wchip = wmap[MAP_NOW_POINT(ix,iy)];
			::DirectDraw(Chip
						,MAP_CHIPNO_TO_MAP_X(wchip) << MAP_CHIP_POW_SIZE
						,MAP_CHIPNO_TO_MAP_Y(wchip) << MAP_CHIP_POW_SIZE
						,MAP_CHIP_SIZE,MAP_CHIP_SIZE
						,bmd
						,Dx + x-sx,Dy + y-sy);
		}
	}
}

/**************************************************************/
//�}�b�v���G�悵�܂�
/**************************************************************/
void TMap::DrawMain(BMD bmd,int w,int h)
{
	MapDrawing(bmd,0,0,Nowx,Nowy,w,h);
}

/**************************************************************/
//��l���ړ��Ȃǂɂ��}�b�v�X�N���[��
/**************************************************************/
void TMap::MapMasterMove(short nx,short ny,int speed)
{
	short lx,ly;  //�X�N���[�����W�ɕϊ�
	lx = nx-Nowx;
	ly = ny-Nowy;
    //���E
    if (lx <=(SCREENXSIZE/2)-NOMOVEX)
		if (Nowx-speed >= 0) Nowx -=speed;
    if (lx >=(SCREENXSIZE/2)+NOMOVEX)
		if (Nowx+speed <= MAP_STOP_XSCROLL) Nowx +=speed;
    //�㉺
	if (ly <=(SCREENYSIZE/2)-NOMOVEY)
        if (Nowy-speed >= 0) Nowy -=speed;
	if (ly >=(SCREENYSIZE/2)+NOMOVEY)
        if (Nowy+speed <= MAP_STOP_YSCROLL) Nowy +=speed;
}


/**************************************************************/
//�w�肵���|�C���g����ʂ̒��S�ɂ���.
/**************************************************************/
void TMap::SetCeneter(short nx,short ny)
{
	int lx,ly;
	lx = -(SCREENXSIZEHARF-nx+Nowx);
	ly = -(SCREENYSIZEHARF-ny+Nowy);

	if (lx > 0)
	{
		if (Nowx+lx <= MAP_STOP_XSCROLL) Nowx +=lx;
        else Nowx = MAP_STOP_XSCROLL;
	}
	else
	{
        if (Nowx+lx >= 0)                Nowx +=lx;
        else Nowx = 0;
	}
	if (ly > 0)
	{
		if (Nowy+ly <= MAP_STOP_YSCROLL) Nowy +=ly;
        else Nowy = MAP_STOP_YSCROLL;
	}
	else
	{
        if (Nowy+ly >= 0)                Nowy +=ly;
        else Nowy = 0;
	}
}

/**************************************************************/
//���̃|�C���g��ʉ߂ł��邩�`�F�b�N
/**************************************************************/
bool TMap::If(short nx,short ny)
{
	//�����Ŗ��ɂȂ�̂��A
	//�}�b�v�`�b�v���܂����ꍇ�ł���.
	//�Ƃ肠�����A���̂��ׂĂ̂܂����`�b�v�Ɣ�����Ƃ�.
	//�����A��ł��ړ��ł��Ȃ��`�b�v���܂������ƁA�v�ɂȂ�.
	short lx,ly;
	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	return true;
}

#endif  //LIFE END
