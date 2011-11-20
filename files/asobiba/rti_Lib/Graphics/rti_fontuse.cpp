#include "StartWithExeFile.h"
#ifdef FONT_USE_LIFE

/*****************************************************************/
/*****************************************************************/
//�O���t�B�b�J���t�H���g�@�����d���Ȃ�
//              Thank you For Botchy �l
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_fontuse.h>
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_error_message.h>
#include <.\Comm\rti_Comm.h>

/*****************************************************************/
//�R���X�g���N�^
/*****************************************************************/
TFontUse::TFontUse(HWND hwnd)
{
	FontInfo=(PLOGFONT)GlobalAlloc(GPTR,sizeof(LOGFONT));
	NewFont = NULL;
    FontWorkBufAlloc(hwnd);
}

/*****************************************************************/
//���[�N�G���A�m��
/*****************************************************************/
bool TFontUse::FontWorkBufAlloc(HWND hwnd)
{
    int x,y;

    x = Sprite->w;
    y = Sprite->h;
	Fontpuf = NULL; Font_memDC = NULL;
	try
	{
		//�V�K�ɍ쐬���܂�
	    Fontpuf = new (struct BMP_DATA_TABLE) ;//�������m��
		if (!Fontpuf) throw (FriendlyError("TFontUse::FontWorkBufAlloc","�������m��","","") );
		Fontpuf->w = x;
		Fontpuf->h = y;
		if ( !CommDraw->GraphicsAlloc(Fontpuf) )
			 throw (FriendlyError("TFontUse::FontWorkBufAlloc","�V�K�̈�m��","","") );
		//DC �m��.
		Font_memDC = CommDraw->GetDC(Fontpuf);
	}
	catch(...)
	{
		delete Fontpuf;
		return false;
	}
	return true;
}


/*****************************************************************/
//�f�X�g���N�^
/*****************************************************************/
TFontUse::~TFontUse()
{
	//�����A�O�Ɏg���Ă���Ȃ�ΑO�̂���Ԃ����낷
	FontBreaker();
	GlobalFree((LOCALHANDLE)FontInfo);

	//DC �j��.
	CommDraw->DeleteDC(Fontpuf,Font_memDC);
	CommDraw->GraphicsFree(Fontpuf);
	delete Fontpuf;
}

/*****************************************************************/
//�t�H���g�Z�b�g
/*****************************************************************/
void TFontUse::FontSet(char* FontName,int Size,int Round,
				   BOOL Bold,BOOL Italic,BOOL Line,BOOL Strike)
{
	//�����A�O�Ɏg���Ă���Ȃ�ΑO�̂���Ԃ����낷
	FontBreaker();

	lstrcpy(FontInfo->lfFaceName,FontName);

    NowBold = Bold;
    FontInfo->lfHeight=Size;
	FontInfo->lfWeight=(Bold==FALSE)*FW_NORMAL+(Bold==TRUE)*FW_BOLD;
	FontInfo->lfEscapement=Round*10;
	FontInfo->lfWidth=0;
	FontInfo->lfItalic=Italic;
	FontInfo->lfUnderline=Line;
    FontInfo->lfStrikeOut = Strike;
	FontInfo->lfCharSet=SHIFTJIS_CHARSET;

	NewFont=CreateFontIndirect(FontInfo);

	OldFont=(HFONT)SelectObject(Font_memDC,NewFont);
}

/*****************************************************************/
//�t�H���g�j��
/*****************************************************************/
void TFontUse::FontBreaker()
{
	if (NewFont!=NULL)
	{ //�����A�O�Ɏg���Ă���Ȃ�ΑO�̂���Ԃ����낷
		OldFont=(HFONT)SelectObject(Font_memDC,OldFont);
		DeleteObject(NewFont);
		NewFont = NULL;
	}

}

/*****************************************************************/
//�t�H���g�̐F�̃Z�b�g
/*****************************************************************/
void TFontUse::FontColorSet(unsigned long TextColor,unsigned long BackColor,
				   BOOL Mode)
{
	SetTextColor(Font_memDC,TextColor);
	SetBkColor(Font_memDC,BackColor);

	if (Mode) SetBkMode(Font_memDC,TRANSPARENT);
}


/*****************************************************************/
//�P���ȕ����\��
/*****************************************************************/
void TFontUse::Text(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,int para,
					char* Format,...)
{
	char Buffer[256];
	int rx,ry;
	rx =ex-x;
	ry =ey-y;
	if (rx>Fontpuf->w || rx < 0) return;
	if (ry>Fontpuf->h || ry < 0) return;

    //������֌W�̏�����(��ʂ� T1PUT )
    MojiInit();

	vsprintf(Buffer,Format,(char*)(&Format+1));

	//�o�b�t�@�̃N���A
	BitBlt(Font_memDC,0,0,rx,ry,NULL,0,0,BLACKNESS);

    RECT rt;
    rt.top = 0;
    rt.bottom = ry;
    rt.right = rx;
    rt.left = 0;
    DrawText(Font_memDC, Buffer, -1, &rt, para);
    //�ꕶ�����͂̏ꍇ�@�����̏o�͈ʒu���L�^����.
    SetMojiPoint(Buffer,&rt);

	//DC �X�V
	CommDraw->UpdataDC(Fontpuf,Font_memDC);

   //�����o��
   MojiDraw(bmd,Proc,x,y,ex,ey);
}

#define FONT_BIGGER      0
#define FONT_UN_BIGGER   1
#define FONT_SIZE_UP     2
#define FONT_SIZE_DOWN   3
#define FONT_ITALER      4
#define FONT_UN_ITALER   5
#define FONT_UNDER       6
#define FONT_UN_UNDER    7
#define FONT_STRIKE      8
#define FONT_UN_STRIKE   9
#define FONT_FONT_COLOR  10
#define FONT_FONT_NAME   11
#define FONT_FONT_SIZE   12
#define FONT_BACK_COLOR  13
#define FONT_BACK_MODE   14
#define FONT_SLOW        15
#define FONT_FARST       16
#define FONT_KEY         17
#define FONT_WAIT        18

#define ADDERSTYE_MAX 30
/*****************************************************************/
//���x�����\��
/*****************************************************************/
void TFontUse::TextEx(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,int para,
					  char* Format,...)
{
    static char *CommandLib[] =
    {
        {"B>"},{"/B>"},         //�ӂƕ���
        {"BIG>"},               //����傫������
        {"SMALL>"},             //���菬��������
        {"I>"},{"/I>"},         //�C�^���b�N
        {"U>"},{"/U>"},         //�A���_�[���C��
        {"S>"},{"/S>"},         //�����
        {"FONTCOLOR="},         //�t�H���g�J���[
        {"FONTNAME="},          //�t�H���g��
        {"FONTSIZE="},          //�t�H���g�T�C�Y
        {"BACKCOLOR="},         //�o�b�N�J���[
        {"BACKMODE="},          //�o�b�N�𓧖��ɂ���.
    };
    int p;
    int sx,sy;
    char Str[256];
    char DispStr[256];
    char *Now = Str;   //���݂̃t�H�[�}�b�g�̈ʒu
    char *CompStart = Str; //�R���p�C���ς݃o�b�t�@�̃X�^�[�g�ʒu
    char AdderStye[ADDERSTYE_MAX];

    //������֌W�̏�����(��ʂ� T1PUT )
    MojiInit();

    sx = sy = 0;
	//�o�b�t�@�̃N���A
	BitBlt(Font_memDC,0,0,Fontpuf->w,Fontpuf->h,NULL,0,0,BLACKNESS);
	vsprintf(Str,Format,(char*)(&Format+1));

    //�^�O�R�}���h��� & �o��.
    for(p=0; *Now ;)
    {
        if(*Now == '<')
        {
            //��v���閽�߂�T��
            for (int i=0; i < sizeof(CommandLib)/sizeof(*CommandLib) ; i ++)
            { //���ׂĂ̖��߂Ɣ�r
                if (my_strstr((Now+1),CommandLib[i])==0)
                { //��v�������߂�������

                    //�Ƃ肠�����G��
                    if (p)
                    {
                        lstrcpy(DispStr,CompStart);
                        DispStr[p] = 0;
                        Disp(x,y,ex,ey,&sx,&sy,para,DispStr);
                    }
                    //�����|�C���^��i�߂�
                    Now = Now+lstrlen(CommandLib[i]);
                    //�ǉ����𓾂�
                    Now += AdderStyrGet(Now,AdderStye);
                    FontProccesing(i,AdderStye);

                    //�܂��A�������疽�߂����߂��Ă����܂�.
                    CompStart = Now;
                    p=0;
                    break;
                }
            }
        }
        else
        {
            Now++,p++;
        }
    }
    if (CompStart != Now)
    {
        //�c����G��
        Disp(x,y,ex,ey,&sx,&sy,para,CompStart);
   }
   //DC �X�V
   CommDraw->UpdataDC(Fontpuf,Font_memDC);

   //�����o��
   MojiDraw(bmd,Proc,x,y,ex,ey);
}

int TFontUse::AdderStyrGet(char *Now,char *AdderStye)
{
    if(*Now == '>') 
    {
        AdderStye[0] = 0;
        return 1; //�ǉ��X�^�C���Ȃ�
    }

    char *LpNow = Now+1;
    char *LpAdderStye = AdderStye;

    for(int i=0; i < ADDERSTYE_MAX ; i ++)
    {
        if (*LpNow == '>' || *LpNow == 0) 
        { //�ǉ��X�^�C���擾����
            *LpAdderStye = 0;
            return (i+2);
        }
        *LpAdderStye = *LpNow;
        LpNow++;
        LpAdderStye++;
    }
//�擾�G���[�@��������
    AdderStye[0] = 0;
    return 0;

}

/*****************************************************************/
//�t�H���g�ύX����
/*****************************************************************/
void TFontUse::FontProccesing(int tagno,char *AdderStye)
{
    char  *FontName = FontInfo->lfFaceName;
    int   iSize      = FontInfo->lfHeight;
    int   iRotate    = 0;
    BOOL  fBold     = NowBold;
    BOOL  fItalic   = FontInfo->lfItalic;
    BOOL  fUnderline= FontInfo->lfUnderline;
    BOOL  fStrikeOut= FontInfo->lfStrikeOut;
    unsigned long  iTextColor = GetTextColor(Font_memDC);
    unsigned long  iBackColor = GetBkColor(Font_memDC);
    BOOL fMode                = GetBkMode(Font_memDC);

    switch(tagno)
    {
    case FONT_BIGGER: //�傫�����ɂ���.
        fBold = TRUE;
        break;
    case FONT_UN_BIGGER   :
        fBold = FALSE;
        break;
    case FONT_SIZE_UP     :
        iSize++;
        break;
    case FONT_SIZE_DOWN   :
        iSize--;
        break;
    case FONT_ITALER      :
        fItalic = TRUE;
        break;
    case FONT_UN_ITALER   :
        fItalic = FALSE;
        break;
    case FONT_UNDER       :
        fUnderline = TRUE;
        break;
    case FONT_UN_UNDER    :
        fUnderline = FALSE;
        break;
    case FONT_STRIKE      :
        fStrikeOut = TRUE;
        break;
    case FONT_UN_STRIKE   :
        fStrikeOut = FALSE;
        break;
    case FONT_FONT_COLOR  :
        iTextColor = atoi(AdderStye);
        break;
    case FONT_FONT_NAME   :
        FontName = AdderStye;
        break;
    case FONT_FONT_SIZE   :
        iSize = atoi(AdderStye);
        break;
    case FONT_BACK_COLOR  :
        iBackColor = atoi(AdderStye);
        break;
    case FONT_BACK_MODE   :
        break;
    case FONT_SLOW        :
        break;
    case FONT_FARST       :
        break;
    case FONT_KEY         :
        break;
    case FONT_WAIT        :
        break;
    }
    FontSet(FontName,iSize,iRotate,fBold,fItalic,fUnderline,fStrikeOut);
    FontColorSet(iTextColor,iBackColor,fMode);
}

/*****************************************************************/
//���̕����� p2 �� p1 �̌��݈ʒu���瑶�݂��邩�`�F�b�N
/*****************************************************************/
int TFontUse::my_strstr(const char *p1,const char *p2)
{
    char *w1,*w2;
    for(w1 = (char*)p1,w2 = (char*)p2;  *w2 ; w1++,w2++)
    {
        if (*w1 != *w2) return -1;
    }
    return 0;
}

/*****************************************************************/
//���x������\���Ŏg��������\��
/*****************************************************************/
void TFontUse::Disp(int x,int y,int ex,int ey,int *sx,int *sy,
                    int para,char* Str)
{
	int rx,ry;
	rx =ex-x;
	ry =ey-y;
	if (rx>Fontpuf->w || rx < 0) return;
	if (ry>Fontpuf->h || ry < 0) return;

    RECT rt;
    rt.top = *sy;
    rt.bottom = ry+*sy;
    rt.right = rx+*sx;
    rt.left = *sx;

    //�G�悷�镶����̒���
    int len = lstrlen(Str);
 
    SIZE s;
    GetTextExtentPoint32(Font_memDC, Str, len, &s);
    *sy += DrawText(Font_memDC, Str, -1, &rt, para) - s.cy;

    //�ꕶ�����͂̏ꍇ�@�����̏o�͈ʒu���L�^����.
    SetMojiPoint(Str,&rt);

    for(int seeklen = len;seeklen >= 0 ; seeklen--)
    {
        if (Str[seeklen] == '\n')
        {
            GetTextExtentPoint32(Font_memDC,
                    Str+seeklen+1, len-seeklen-1, &s);
            break;
        }
    }

    *sx += s.cx;
}


#endif //LIFE END

