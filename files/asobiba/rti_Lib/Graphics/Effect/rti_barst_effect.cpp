#include "StartWithExeFile.h"
#ifdef BARST_LIFE

/*****************************************************************/
/*****************************************************************/
//�����ƕ��A �f�X&���o�[�X
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\Effect\rti_barst_effect.h>
#include <.\Base\rti_glookup.h>

TBarstEffect::TBarstEffect()
{
    list = NULL;
}

TBarstEffect::~TBarstEffect()
{
    Init();
}

void TBarstEffect::Init()
{
	if(list) free(list);
}

void TBarstEffect::Set(BMD bmd,BMD _OutBmd,bool desth,int x,int y,int w,int h,int basespeed)
{
    OutBmd = _OutBmd; //�o��
	pbmd = bmd;  //�G�Ɋ֌W������͓r���ŕύX���邱�Ƃ͂ł��Ȃ��̂�
	pw = bmd->w; //�O���[�o���Ƃ��Ă�����Ă��� (^^;;
	ph = bmd->h; 
	pww = w;
	phh = h;

    Init();

	listmax =( (pw / pww) + (pw%pww ? 1 : 0) ) * ( (ph / phh) + (ph%phh ? 1 : 0) );
	list = (struct BarstStruct*)malloc( sizeof(struct BarstStruct) * (listmax + 1) );

	if (desth) //����
		ReSet(x,y,true,basespeed);
	else       //����
		RevReSet(x,y,true,basespeed);
}

//�����̃��Z�b�g
void TBarstEffect::ReSet(int x,int y,bool set_possition,int basespeed) 
{
	int wx,wy,i;
	struct BarstStruct* puf;
    pbasepeed = basespeed ;

	i = 0;
	for (wy = ph-1; wy >= 0  ; wy-=phh)
	{
		for (wx = pw-1; wx >= 0  ; wx-=pww)
		{
				i ++;
				puf = &(list[i]);
				puf->org = PGet(pbmd,wx,wy);
				if (set_possition)
				{ //�G�̂���ʒu���w�肵�ďグ��
					puf->x = wx + x;
					puf->y = wy + y;
				}
				while ( (puf->sx = ( (rand()%4-2) * rand()%pbasepeed )  ) == 0);
				while ( (puf->sy = ( (rand()%4-2) * rand()%pbasepeed )  ) == 0);
				puf->w = pww;
				puf->h = phh;
				puf->alive = true;
		}
	}

	eopuf = 0;
}

//���A�̃��Z�b�g
void TBarstEffect::RevReSet(int x,int y,bool set_possition,int basespeed) 
{
	struct BarstStruct* puf;
	int wx,wy,i;
	int ww,wh;
	px = x;
	py = y;
	ww = OutBmd->w -1;
	wh = OutBmd->h -1;
    pbasepeed = basespeed ;

	i = 0;
	for (wy = ph-1; wy >= 0  ; wy-=phh)
	{
		for (wx = pw-1; wx >= 0  ; wx-=pww)
		{
				i ++;
				puf = &(list[i]);
				puf->org = PGet(pbmd,wx,wy);
				if (set_possition)
				{  //�K���ɉ�ʒ[�Ɏw�肷��
					if (rand() % 2)
					{ //X�ς� �㉺
						puf->x = rand() % ww + 1;
						if (rand() % 2)	puf->y = wh;
						else			puf->y = 1;
					}
					else
					{ //X�ς� ���E
						puf->y = rand() % wh + 1;
						if (rand() % 2)	puf->x = ww;
						else			puf->x = 1;
					}
				}
				//�ړ��ʂ����߂�
				path_finder(puf->x,puf->y, x+wx , y+wy ,
					&(puf->sx),&(puf->sy),pbasepeed);
				puf->w = pww;
				puf->h = phh;
				puf->alive = true;
		}
	}

	eopuf = 0;
}

//�����G��
int TBarstEffect::Draw()
{
	int i;
	int w,h;
	w = OutBmd->w-1;
	h = OutBmd->h-1;
	struct BarstStruct* puf;
	for (i = listmax-1 ; i >= 0  ; i --)
	{
		puf = &(list[i]);
		if (puf->alive && puf->org)
		{
			if (puf->x >= w || puf->x <= 0 || puf->y >= h || puf->y <= 0)
			{
				puf->alive = false;
				eopuf ++;
			}
			else
			{
                PSet(OutBmd,puf->x,puf->y,puf->org);

                puf->x += puf->sx;
				puf->y += puf->sy;
			}
		}
	}
	return  listmax - eopuf;
}


//���A�G��
int TBarstEffect::RevDraw()
{
	int x,y;
	int ssx,ssy;
	int wy,wx,faj;
	int realx,realy;
	int w,h;
	w = OutBmd->w-1;
	h = OutBmd->h-1;
	struct BarstStruct* puf;
	struct BarstStruct* plist;
	faj = pbasepeed<<1;

	plist = list;
	for (wy = ph-1 ; wy >= 0;wy-=phh)
	{
		for (wx = pw-1; wx >=0 ; wx-=pww)
		{
			puf = plist++;
			if (puf->alive && puf->org)
			{
				x = puf->x += (ssx = puf->sx);    //�ړ� 
				y = puf->y += (ssy = puf->sy);
				realx = px+wx;        //���ۍ��W(�ړI�n)
				realy = py+wy;        //���ۍ��W(�ړI�n)
				if ( (realx - faj) <= x && (realx + faj) >= x && (realy - faj) <= y && (realy + faj) >= y)
				{ //�덷�͗e�F����
					puf->x = realx;
					puf->y = realy;
					puf->alive = false;
					eopuf ++;
				}
				if (realx > x && ssx < 0 ||
					realx < x && ssx > 0 ||
					realy > y && ssy < 0 ||
					realy < y && ssy > 0 )
				{
							path_finder(x,y, realx , realy ,
									&(puf->sx),&(puf->sy),pbasepeed);
				}
			}
			//�ꉞ�N���b�s���O���Ă���..(good bye speed)
			if (puf->x < w && puf->x > 0 && puf->y < h && puf->y > 0)
                        PSet(OutBmd,puf->x,puf->y,puf->org);
		} 
	}
	return  listmax - eopuf;
}


#endif //LIFE END

