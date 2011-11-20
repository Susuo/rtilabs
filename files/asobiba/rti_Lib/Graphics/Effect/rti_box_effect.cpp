/*****************************************************************/
/*****************************************************************/
//�@�l�p��c�������Ă����܂�.
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef BOXEFFECT_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_box_effect.h>

TBoxEffect::TBoxEffect()
{
    BoxBlockBuf = NULL;
}

TBoxEffect::~TBoxEffect()
{
    Init();
}

void TBoxEffect::Init()
{
    if (BoxBlockBuf)  free(BoxBlockBuf);
}

void TBoxEffect::Set(BMD bmd,BMD _OutBmd,int sizex,int sizey,int speed,unsigned char kind)
{
    int t,wx,wy, falltime , falltimebackup;
    OutBmd = _OutBmd; //�o��
    SpBuf = bmd;
    Speed = speed;

	//�^�C���ɐ؂�􂢂��Ƃ��̃T�C�Y�����߂�
	TableSize = GraphicsCutTile
		(SpBuf,sizex,sizey,&MaxXSize,&MaxYSize);

    Init();
    BoxBlockBuf = (struct _BoxBlock*)
        malloc(TableSize * sizeof(struct _BoxBlock) +1);
    wx = 0,wy = 0;
    //t�̓��[�v�J�E���^�[�Ɏg����
    for(t = 0; t < TableSize ; t ++)
    {
        wx += sizex;
        wy += sizey;
        if (wx >= SpBuf->w) wx = 0;
        if (wy >= SpBuf->h) wy = 0;
        BoxBlockBuf[t].x = wx;
        BoxBlockBuf[t].y = wy;
    }


    falltime = 0; //���ꗎ���o������
    struct _BoxBlock* lpBoxBlockBuf = BoxBlockBuf;
    for(wy=0 ; wy < MaxYSize ; wy ++ )
    {
        falltimebackup = falltime ;
        for(wx=0 ; wx < MaxXSize ; wx ++ )
        {
            lpBoxBlockBuf->x = wx * sizex;
            lpBoxBlockBuf->y = wy * sizey;
            lpBoxBlockBuf->nw = 0;
            lpBoxBlockBuf->nh = 0;
            t = (lpBoxBlockBuf->x + sizex) - SpBuf->w;
            if (t>0) lpBoxBlockBuf->sizex = t;
            else     lpBoxBlockBuf->sizex = sizex;
            t = (lpBoxBlockBuf->y + sizey) - SpBuf->h;
            if (t>0) lpBoxBlockBuf->sizey = t;
            else     lpBoxBlockBuf->sizey = sizey;
            lpBoxBlockBuf->alive = true;

            lpBoxBlockBuf++;
        }
    }
    timecounter = 0;
    deedcount   = 0;
}


int TBoxEffect::Draw(int x,int y,GraphicsProc Proc)
{
    short wx,wy;
    struct _BoxBlock* lpBoxBlockBuf = BoxBlockBuf;
    for(wy=0 ; wy < MaxYSize ; wy ++ )
    {
        for(wx=0 ; wx < MaxXSize ; wx ++ )
        {
            if (lpBoxBlockBuf->alive)
            {   //�c��������.
                lpBoxBlockBuf->nw += Speed; 
                lpBoxBlockBuf->nh += Speed; 
                if (lpBoxBlockBuf->nw >= lpBoxBlockBuf->sizex ||
                    lpBoxBlockBuf->nh >= lpBoxBlockBuf->sizey )
                {
                    lpBoxBlockBuf->nw = lpBoxBlockBuf->sizex;
                    lpBoxBlockBuf->nh = lpBoxBlockBuf->sizey;
                    lpBoxBlockBuf->alive = false;
                    deedcount ++;  //���̃J�E���g
                }
                Proc(SpBuf,lpBoxBlockBuf->x,lpBoxBlockBuf->y,
                    lpBoxBlockBuf->nw,lpBoxBlockBuf->nh,
                    OutBmd,x + lpBoxBlockBuf->x,y + lpBoxBlockBuf->y);
            }
            else
            {   //����ȏ�c�������邱�Ƃ͂ł��Ȃ��̂ŕ��ʂɊG��
                Proc(SpBuf,lpBoxBlockBuf->x,lpBoxBlockBuf->y,
                    lpBoxBlockBuf->sizex,lpBoxBlockBuf->sizey,
                    OutBmd,x + lpBoxBlockBuf->x,y + lpBoxBlockBuf->y);
            }
            lpBoxBlockBuf++;
        }
    }

    timecounter ++;
    return deedcount - TableSize;  //0���I��
}

#endif  //LIFE END
