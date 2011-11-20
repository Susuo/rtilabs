/**********************************************************/
// �s�N�`���[�{�b�N�X  
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef PICTUREBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_picturebox.h>

TPictureBox::TPictureBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
    :TGroupBox(hOwner,Caption,dwStyle,x,y,w,h,UserWndp)
{
	memDC = NULL;
	hBit = NULL;
}

TPictureBox::~TPictureBox()
{
	DestroyPicture();
} 

/**********************************************************/
/* �G���w�� */
/**********************************************************/
void TPictureBox::SetPicture(char *FileName)
{
	//�O�̂��ߊJ��
	DestroyPicture();

	HDC hdc;
    BITMAP bmp_info;
	hdc = GetDC(hWnd);                     //�f�o�C�X�R���e�L�X�g�擾

	memDC = CreateCompatibleDC(hdc); //������DC�擾
	hBit = LoadImage(Application->hInstance,FileName,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������
    GetObject(hBit, sizeof(BITMAP), &bmp_info);
    w = bmp_info.bmWidth;
    h = bmp_info.bmHeight;

	SelectObject(memDC,hBit);   //�r�b�g�}�b�v�I��

	ReleaseDC(hWnd,hdc);					//DC �J��
}

/**********************************************************/
/* �G���w�� */
/**********************************************************/
void TPictureBox::SetPicture(BMD bmd)
{
	//�O�̂��ߊJ��
	DestroyPicture();

	HDC hdc;
	hdc = GetDC(hWnd);                     //�f�o�C�X�R���e�L�X�g�擾

	memDC = CreateCompatibleDC(hdc); //������DC�擾
	hBit  = CreateBitmap(bmd->bm_header.biWidth,
						 bmd->bm_header.biHeight,
						 bmd->bm_header.biPlanes,
						 bmd->bm_header.biBitCount,
						 bmd->bm); 
	w = bmd->bm_header.biWidth;
	h = bmd->bm_header.biHeight;

	SelectObject(memDC,hBit);   //�r�b�g�}�b�v�I��

	ReleaseDC(hWnd,hdc);					//DC �J��
}

/**********************************************************/
/* �������J�� */
/**********************************************************/
void TPictureBox::DestroyPicture()
{
    if (memDC) 
	{
		DeleteDC(memDC);       //������DC�J��
		memDC = NULL;
	}
    if (hBit) 
	{
	    DeleteObject(hBit);    //�r�b�g�}�b�v�n���h���폜
		hBit = NULL;
	}
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TPictureBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;

    switch(message)
    {
	case WM_PAINT:
        if (memDC)
        {
            BeginPaint(hwnd,&ps);            //PAINT��p�̃f�o�R���擾
		    BitBlt(ps.hdc ,0,0,	w,h,  memDC,0,0,SRCCOPY);
	                                    //������DC����f�B�X�v���CDC�֓]��
		    EndPaint(hwnd,&ps);              //WM_PAINT��p�̃f�o�R���J��
        }
		break;
    }
    return TGroupBox::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
