/*****************************************************************/
/*****************************************************************/
//�u���C���h�̂悤�ɊG��
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef BLINED_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_blined_effect.h>

TBLinedEffect::TBLinedEffect()
{
    BLined = NULL;
}

TBLinedEffect::~TBLinedEffect()
{
	Init();
}

void TBLinedEffect::Init()
{
	if (BLined) free(BLined);
}

void TBLinedEffect::Set(BMD _bmd,BMD _OutBmd,int _linehaba,int _linekankaku,int _speed)
{
    OutBmd = _OutBmd; //�o��
	linehaba = (short)_linehaba;
	speed    = (short)_speed;
	bmd = _bmd;

	size = bmd->w / _linekankaku;
	if (bmd->w % _linekankaku) size++;
	
    Init();
	BLined = (struct _BLined*)
		malloc(sizeof(struct _BLined) * size+1);
	struct _BLined *lpBLined = BLined;
	short kankaku = 0;
	short start = 0;
	int i;
	for(i = 0 ; i < size ;i++)
	{
		lpBLined->alive = true;
		lpBLined->start = start;  //�X�^�[�g���鎞��
		lpBLined->x = kankaku;    //�����o���͂��߂�ʒu
		lpBLined->linew = 0;      //�����o�����Ƃ���̈ʒu

		start += (_linekankaku >>4);
		kankaku += _linekankaku;

		if (kankaku > bmd->w) kankaku = bmd->w;

		lpBLined->toutatu = kankaku-_linehaba+1; //�����o���I���ʒu
		lpBLined++;

	}
	counter = 0;
}


int TBLinedEffect::Draw(GraphicsProc Proc,int x,int y)
{
	struct _BLined *lpBLined = BLined;
	int i;
	for(i = 0 ; i < size ;i++)
	{
		Proc(bmd,lpBLined->x,0,
			lpBLined->linew,bmd->h,
			OutBmd,x + lpBLined->x , 0);
		if (counter >= lpBLined->start)
		{
			//�w��͈͂܂œ��B���Ă��邩
			if (lpBLined->x + lpBLined->linew
				< lpBLined->toutatu )
			{
				lpBLined->linew+=linehaba; //�����o���͈͂��L����
			}
			else
			{  //���B���Ă���
				if (lpBLined->alive) 
				{
					lpBLined->alive = false;
                    deedcount ++;  //���̃J�E���g
				}
			}
		}
		lpBLined++;
	}
	counter += speed;

    return deedcount - size;  //0���I��
	
}

#endif  //LIFE END
