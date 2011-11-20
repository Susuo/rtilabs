#include "StartWithExeFile.h"
#ifdef NOMAL_BITMAL_LIFE
#include <.\Graphics\rti_nomal_gdi_bitmap.h>

/********************************************************/
//�r�b�g�}�b�v���[�h
/********************************************************/
void PictureLoad(char *filename,HWND hwnd,NBMD Pic)
{
	HDC hdc;
    BITMAP bmp_info;
	hdc = GetDC(hwnd);                     //�f�o�C�X�R���e�L�X�g�擾

	Pic->memDC = CreateCompatibleDC(hdc); //������DC�擾
	ReleaseDC(hwnd,hdc);                   //DC �J��

    if(!Pic->memDC) return ;

	Pic->hBit = LoadImage(Application->hInstance,filename,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������
    if(!Pic->hBit) 
    {
      	DeleteDC(Pic->memDC);       //������DC�J��
        return ;
    }
    GetObject(Pic->hBit, sizeof(BITMAP), &bmp_info);
    Pic->w = bmp_info.bmWidth;
    Pic->h = bmp_info.bmHeight;

	SelectObject(Pic->memDC,Pic->hBit);   //�r�b�g�}�b�v�I��
}

/********************************************************/
//�r�b�g�}�b�v�j��
/********************************************************/
void PictureDestroy(NBMD Pic)
{
  	DeleteDC(Pic->memDC);       //������DC�J��
    DeleteObject(Pic->hBit);    //�r�b�g�}�b�v�n���h���폜
}

/********************************************************/
//��̗̈�쐬
/********************************************************/
void PictureCreate(int x,int y,HWND hwnd,NBMD Pic)
{
	HDC hdc;
    hdc = GetDC(hwnd);
	Pic->memDC = CreateCompatibleDC(hdc); //������DC�擾
	ReleaseDC(hwnd,hdc);					//DC �J��

	Pic->hBit = CreateCompatibleBitmap(hdc,x,y);//�r�b�g�}�b�v�̈�쐬
	Pic->w = x;
	Pic->h = y;

	SelectObject(Pic->memDC,Pic->hBit); //�r�b�g�}�b�v�I��
}

#endif //END LIFE
