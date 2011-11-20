#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_ChangeDisplay.h>	//GDI �𑜓x�ϊ�
#include <.\Game\rti_GameMain.h>

//������.
static bool GDIInit(HWND hwnd,int w,int h,int bps);
//�I������.
static bool GDIEnd();
//�t�@�C���ǂݍ���
static bool GDIGraphicsMake(BMD puf,const char* filename);
static bool GDIGraphicsMakeSub(BMD bmd,HDC memDC);
//�̈�m��
static bool GDIGraphicsAlloc(BMD bmd);
//�̈���
static bool GDIGraphicsFree(BMD bmd);
//���b�N
static bool GDIGraphicsLock(BMD bmd);
//�A�����b�N
static bool GDIGraphicsUnLock(BMD bmd);
//������ bmd �� DC ���쐬���� (^^;;;
static HDC GDIGetDC(BMD bmd);
//DC ����������. (DC �ɑ΂��鑀�삪�t�B�[�h�o�b�N�����.)
static bool GDIDeleteDC(BMD bmd,HDC hdc);
//DC->BMD �̒��g���X�V����.
static bool GDIUpdataDC(BMD bmd,HDC hdc);
//���݂̊G�惂�[�h�̎擾
static UseDrawMode  GDIGetMode();
//���t���b�V�����i.
static void GDIRef(HWND hwnd);
//�t���X�N���[���ɂ���!
static bool GDIDoFullScreen(bool CoerceChange);
//�E�B���h���[�h�ɂ���!!
static bool GDIDoWindow(bool CoerceChange);
//�G��|�C���g�̐ݒ�(GDI�ł͖����ł�)
static void GDISetDrawPoint(int x,int y);

//GDI�����p���鏉�������[�`�����҂��.
static struct _CommDraw CommDrawGDI = 
	{GDIInit,GDIEnd,GDIGraphicsMake,GDIGraphicsAlloc,GDIGraphicsFree,
		GDIGraphicsLock,GDIGraphicsUnLock,GDIGetDC,GDIDeleteDC,GDIUpdataDC,GDIGetMode,GDIRef,
		GDIDoFullScreen,GDIDoWindow,GDISetDrawPoint};
//�𑜓x�ϊ�
static TChangeDisplay TCS;

//GDI�̏����̗p������Ă���.
void GDIAdoption()
{
	CommDraw = &CommDrawGDI;
}

//����������.
bool GDIInit(HWND hwnd,int w,int h,int bps)
{
	NowScreenMode.fFullScreen = false; //GDI ���[�h�́A�E�B���h���[�h�ŋN��.
	NowScreenMode.iWidth      = w;
	NowScreenMode.iHeight     = h;
	NowScreenMode.iBpp        = bps;
	return true;
}

//�I������.
bool GDIEnd()
{
	if (NowScreenMode.fFullScreen)
	{
		//�t���X�N�[������������𑜓x�����ɖ߂�.
		TCS.Restoration();
	}
	return true;
}

//�t���X�N�[����
bool GDIDoFullScreen(bool CoerceChange = false)
{
	if (!CoerceChange)
		if (NowScreenMode.fFullScreen)	return false;	//���łɃt���X�N�[�����ł�.
    //�𑜓x�ύX
	if ( TCS.Change(NowScreenMode.iWidth,NowScreenMode.iHeight,0) )
	{
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, FULLWINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		GameForm->Move(0,0);	//��������Ȃ��ƁA �Q�C�c��98 �ł́A���������Ȃ�.
		NowScreenMode.fFullScreen = true;
		return true;
	}
	else
	{
		FriendlyError("GDIDoFullScreen","�t���X�N�[�����ݒ�",
			"���̃r�f�I�J�[�hor�f�B�X�v���C�́A���̉𑜓x���T�|�[�g���Ă��܂���.",
			"�r�f�I�J�[�h�Ȃǂ��܂Ƃ��Ȃ��̂ƌ������Ă�������");
		return false;
	}
}

//�E�B���h���[�h
bool GDIDoWindow(bool CoerceChange = false)
{
	if (!CoerceChange)
		if (!NowScreenMode.fFullScreen)	return false;	//���łɃE�B���h���[�h�ł�.
    //�𑜓x�ύX
	if ( TCS.Restoration() )
	{
    	SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, WINDOW_STYLE);
	    GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
        //GameForm->Align(ALIGN_MOVE_CENTER);
		GameForm->Move(100,100);
        InvalidateRect(NULL,NULL,TRUE);	//��������Ȃ��ƁA�ĊG�悳��Ȃ� �������܂ɂł�.
    	NowScreenMode.fFullScreen = false;
		return true;
	}
	else
	{
		FriendlyError("GDIDoWindow","�E�B���h���[�h�ɂ��邽�߂ɉ�ʂ����ɖ߂��ݒ�",
			"�����s��","�����s���ł�.");
		return false;
	}
}

bool GDIGraphicsMake(BMD puf,const char* filename)
{
    BITMAP bmp_info;
    HDC memDC = NULL;            //�������f�o�C�X�R���e�L�X�g
    HDC hdc   = NULL;
    HBITMAP hBit = NULL;         //�r�b�g�}�b�v�����
    try
    {
	    hdc = GetDC(NULL);                     //�f�o�C�X�R���e�L�X�g�擾
        if (!hdc)
			throw (FriendlyError("GraphicsMake","�f�o�C�X�R���e�L�X�g�擾","?","?"));

    	memDC = CreateCompatibleDC(hdc); //������DC�擾
        if (!memDC)
			throw (FriendlyError("GraphicsMake","MemDC�擾","?","?"));

        if ( !ReleaseDC(NULL,hdc) )		//DC �J��
			 throw (FriendlyError("GraphicsMake","�f�o�C�X�R���e�L�X�g�j��","",""));

	    hBit = (HBITMAP) LoadImage(NULL,filename,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������
	    if (!hBit) throw (FileNotOpen("GraphicsMake",filename) );

        GetObject(hBit, sizeof(BITMAP), &bmp_info);
        puf->w = bmp_info.bmWidth;
        puf->h = bmp_info.bmHeight;
    
	    SelectObject(memDC,hBit);   //�r�b�g�}�b�v�I��
    
	    if ( !GDIGraphicsMakeSub(puf,memDC) )
			 throw (FriendlyError("GraphicsMake","�摜�̈�m��&�]��","����������Ă܂���?","�������𑝂₵�Ă�������"));
        if ( DeleteDC(memDC) ) memDC = NULL;       //������DC�J��
        else  throw (FriendlyError("GraphicsMake","MemDC�폜","?","?"));

        if ( DeleteObject(hBit) ) hBit = NULL;    //�r�b�g�}�b�v�n���h���폜
        else  throw (FriendlyError("GraphicsMake","HBITMAP�폜","?","?"));
    }
    catch(...)
    {
		if (hdc)	ReleaseDC(NULL,hdc);		   //DC�j��
	    if (memDC)  DeleteDC(memDC);       //������DC�J��
	    if (hBit)   DeleteObject(hBit);    //�r�b�g�}�b�v�n���h���폜
	   	return false;
    }
	return true;
}

//�̈�m��
bool GDIGraphicsMakeSub(BMD bmd,HDC memDC)
{
    if (! CommDraw->GraphicsAlloc(bmd) ) return false;

	//�摜�]��(�蔲���`)
	BitBlt( (HDC)(bmd->Tag) ,0,0,bmd->w,bmd->h, memDC,0,0,SRCCOPY);

	return true;
}

//�̈�m��
bool GDIGraphicsAlloc(BMD bmd)
{
	unsigned char Pow2W;
	int X,Y;
	BITMAPINFO bm_info;
	HBITMAP hBit;
	HDC hdc,memDC;

	X = bmd->w ;
	Y = bmd->h ;
	//2�ׂ̂�������.
	CheckPow2(&Pow2W,X);
	bmd->Pow2W = Pow2W;

	hdc = GetDC(NULL);

	memset(&bm_info,0,sizeof(BITMAPINFO));
	bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bm_info.bmiHeader.biWidth = X;
	bm_info.bmiHeader.biHeight = -(Y+1);	// + 1 ���Ȃ��ƃ��o�ł� (^^;;
	bm_info.bmiHeader.biPlanes = 1;
	bm_info.bmiHeader.biBitCount = 24;
	bm_info.bmiHeader.biCompression = BI_RGB;
//	bm_info.bmiHeader.biSizeImage = (X * 3) * (Y+1);
	hBit = CreateDIBSection( hdc, &bm_info , DIB_RGB_COLORS, (void **)&bmd->bm, NULL, 0 );
	memDC = CreateCompatibleDC( hdc );
	if( ! memDC )
	{
		DeleteObject( hBit );
		return	false;
	}
	SelectObject( memDC, hBit );
	//�ӂ[�� DC �̍폜.
	ReleaseDC(NULL,hdc);
	//GDI ���[�h�̓������f�o�C�X�R���e�L�X�g�� Tag �ɓ���܂�.
	bmd->Tag = memDC;

	return true;						//�听��!! (^-^
}

//�̈���
bool GDIGraphicsFree(BMD bmd)
{
	HDC memDC = (HDC) bmd->Tag;
	HBITMAP hBit = (HBITMAP)GetCurrentObject(memDC,OBJ_BITMAP);

	DeleteObject( hBit );
	DeleteDC( memDC ) ;

	return true;
}

//���b�N
bool GDIGraphicsLock(BMD bmd)
{
	return true;
}

//�A�����b�N
bool GDIGraphicsUnLock(BMD bmd)
{
	return true;
}

//������ bmd �� DC ���쐬���� (^^;;;
HDC GDIGetDC(BMD bmd)
{
	return (HDC)bmd->Tag	;						//�������f�o�C�X�R���e�L�X�g��Ԃ�.
}

//DC ����������. (DC �ɑ΂��鑀�삪�t�B�[�h�o�b�N�����.)
bool GDIDeleteDC(BMD bmd,HDC hdc)
{
	return true;
}
 

//DC->BMD �̒��g���X�V����.
bool GDIUpdataDC(BMD bmd,HDC hdc)
{
	return true;
}

//���݂̊G�惂�[�h�̎擾
UseDrawMode GDIGetMode()
{
	return USE_GDI_MODE;
}

//���t���b�V�����i.
void GDIRef(HWND hwnd)
{
	InvalidateRect(hwnd,NULL,false);
}

//�G��|�C���g�̐ݒ�(GDI�ł͖����ł�)
void GDISetDrawPoint(int x,int y)
{
}

