#include "StartWithExeFile.h"
#ifdef FLOWT_LIFE

/*****************************************************************/
/*****************************************************************/
//�Ԃ��~�点��
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_flowt.h>


TFlowtEffect::TFlowtEffect()
{
    FlowtChar = NULL;
}

TFlowtEffect::~TFlowtEffect()
{
    Init();
}

void TFlowtEffect::Init()
{
    if (FlowtChar) free(FlowtChar);
}

void TFlowtEffect::Set(BMD _bmd,BMD _OutBmd,int _onesizew,int _total,int _yspeed,int _xspeedmax,int _flow,int _areatparsent,int _zureparsent)
{
    OutBmd = _OutBmd; //�o��
    bmd = _bmd;               //�e�N�X�`��
    onesizew = _onesizew;      //��̃s�[�X�̉���
    total = _total;           //�s�[�X�̐�
    xspeedmax = _xspeedmax;   //�c�̈ړ��ʂ̐��ő�l(���ۂɂ͂����2�{�ړ�����)
    yspeed = _yspeed;         //������X�s�[�h
    flow = _flow;             //��x�ɕ\�����鐔
    areatparsent = _areatparsent; //�o������m��
    zureparsent = _zureparsent;   //������N�����m��

    Init();
    int size =flow * sizeof(struct _FlowtChar) +1;
    FlowtChar = (struct _FlowtChar*) malloc(size);
    ZeroMemory(FlowtChar,size);

}

void TFlowtEffect::Draw(GraphicsProc Proc,int x,int y,int w,int h)
{
    struct _FlowtChar* lpwFlowtChar = FlowtChar;
    int i;
    bool allredy_make = false;
    for(i = 0 ;i < flow ; i ++)
    {
        if (lpwFlowtChar->alive)
        {
            lpwFlowtChar->x += lpwFlowtChar->xspeed;
            if ( rand100(zureparsent) )
            {
                lpwFlowtChar->xspeed+=1;
                if (lpwFlowtChar->xspeed>=xspeedmax)
                    lpwFlowtChar->xspeed = -lpwFlowtChar->xspeed;
            }
            lpwFlowtChar->y += lpwFlowtChar->yspeed;
            if (lpwFlowtChar->y>=h)
            { //����
                lpwFlowtChar->alive = false;
            }
            else
            {
                Proc(bmd,lpwFlowtChar->wsize,
                    0,onesizew ,bmd->h,
                    OutBmd,x+lpwFlowtChar->x,y+lpwFlowtChar->y);
            }
        }
        else //�󂫔���
        {
            if ( rand100(areatparsent) && !allredy_make)
            { //�o�^
                lpwFlowtChar->alive = true;
                lpwFlowtChar->y = 0;
                lpwFlowtChar->x = randX(0,w);
                lpwFlowtChar->xspeed = randX(-xspeedmax,xspeedmax);
                lpwFlowtChar->yspeed = yspeed;
                lpwFlowtChar->wsize = onesizew * randX(0,total);
                allredy_make = true;
            }
        }
        lpwFlowtChar++;
    }
}


#endif //LIFE END
