/*****************************************************************/
/*****************************************************************/
//�����[�W����
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef RGN_LIFE

#include <windows.h>
#include <.\Base\rti_rgn.h>
#include <.\Base\rti_debug.h>
#include <.\VCL\rti_vcl_apprication.h>

TRgn::TRgn(HWND hwnd,char *filename,char flg)
{

    //���[�W�����̌��ɂȂ�G��ǂ�
    LoadBmp(filename,hwnd);  

    RECT rect;
    int x,y,w,h;
    RegionData Data;

	if(flg==ALIGN_BMPSIZE)
	{ //�E�B���h�T�C�Y���G�̃T�C�Y�ɂ���
	    BITMAP bmp_info;
	    GetObject(RgnhBit, sizeof(BITMAP), &bmp_info);
		MoveWindow(hwnd,0,0,bmp_info.bmWidth,bmp_info.bmHeight,TRUE);
	}

    GetWindowRect(hwnd,&rect);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top ;

    OffsetRect(&rect, -rect.left , -rect.top);
    hRGNMaster= CreateRectRgnIndirect(&rect);

    int sx,ex;
    bool black_next = false;
    Data.Count = 0;
    Data.Size = 32; Data.iType = RDH_RECTANGLES;
    RECT *r;
    r = &(Data.Bounds);
    r->left   = 0;
    r->top    = 0;
    r->right  = w;
    r->bottom = h;
    for (y = 0; y <= h ; y++)
    {
        for (x = 0; x <= w ; x++)
        {
            if (GetPixel(RgnmemDC,x,y))
            {
                if (black_next)
                {//�����ō��͂����܂�
                    ex = x;
                    black_next = false;
                    //�^����
                    RgnCut(sx,ex,y,&Data);
                }
            }
            else
            {//����������
                if (! black_next)
                {//�������獕���X�^�[�g
                    sx = x;
                    black_next = true;
                }
            }
        }
        //�܂��A X �̒����`�Q�b�g���i�s����������
        if (black_next) 
		{
			RgnCut(sx,x,y,&Data);
            black_next = false;
		}
    }
    if (Data.Count > 0) 
    {
        CreateRegion(&Data);
    }
    SetWindowRgn(hwnd,hRGNMaster,TRUE);
}

TRgn::~TRgn()
{
       	DeleteDC(RgnmemDC);       //������DC�J��
	    DeleteObject(RgnhBit);    //�r�b�g�}�b�v�n���h���폜
        DeleteObject(hRGNMaster);
}

void TRgn::LoadBmp(char *filename,HWND hwnd)
{
	HDC hdc;
	hdc = GetDC(hwnd);                     //�f�o�C�X�R���e�L�X�g�擾

	RgnmemDC = CreateCompatibleDC(hdc); //������DC�擾
	RgnhBit = LoadImage(Application->GethInstance(),filename,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������

	SelectObject(RgnmemDC,RgnhBit);   //�r�b�g�}�b�v�I��

	ReleaseDC(hwnd,hdc);					//DC �J��
}


void TRgn::RgnCut(int sx,int ex,int y,RegionData *Data)
{
    if (Data->Count == 4000) {
            CreateRegion(Data);
            Data->Count = 0;
    }
    RECT *r;
    r = &(Data->Rects[Data->Count]);
    r->left   = sx;
    r->top    = y;
    r->right  = ex;
    r->bottom = y+1;
    Data->Count++;
}


void TRgn::CreateRegion(RegionData *Data)
{
    XFORM Form;

    Form.eM11 = 1; Form.eM12 = 0; Form.eM21 = 0; Form.eM22 = 1;
    Form.eDx = 0; Form.eDy = 0;
    Data->RgnSize = Data->Count * 16;
    //��
    HRGN hCutRgn = ExtCreateRegion(&Form, 32 + Data->Count * 16, (RGNDATA *)Data);
    //������
    CombineRgn(hRGNMaster,hRGNMaster,hCutRgn,RGN_XOR);
    //�����錊�̃f�[�^�͕s�v�Ȃ̂Ŕj��
    DeleteObject(hCutRgn);
}

#endif //LIFE END
