/*****************************************************************/
/*****************************************************************/
//�@������� �ڂ₟�`�ƊG�悵�܂�
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef SLOW_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_slow_effect.h>

TSlowEffect::TSlowEffect()
{
}

TSlowEffect::~TSlowEffect()
{
}

void TSlowEffect::Set(BMD _bmd,BMD _OutBmd,int _sizex,int _sizey,int _tsizex,int _tsizey,int _speed)
{
    OutBmd = _OutBmd; //�o��
 	bmd = _bmd;
    timecounter = 0;
	sizex = _sizex;
	sizey = _sizey;
	tsizex = _tsizex;
	tsizey = _tsizey;
	speed = _speed;
	sizexhalf =_sizex>>1;
	sizeyhalf =_sizey>>1;
}


bool TSlowEffect::Draw(GraphicsProc Proc,int x,int y)
{
	short wy,wx;  
	short sy,sx;  //�G��J�n�_
	short gw,gh;  //�G�敝
	short ty,tx;  //�G�掲�����炷
    short bmdw,bmdh;  //�G�̃T�C�Y�̃e���|����
	bool end_of_flg = false;  //�I���t���O
	bool ret = true;          //�߂�l

	//�������̂��߂� �G�̍����ƕ����e���|�������
	bmdw = bmd->w;
	bmdh = bmd->h;

	//�G�掲�����炷
	tx = tsizex+(timecounter>>2);
	ty = tsizey+(timecounter>>2);
	//�ȈՃN���b�s���O����1
	if (tx > sizexhalf) 
	{
		tx = sizexhalf;
		end_of_flg = true;
	}
	if (ty > sizeyhalf) 
	{
		if (end_of_flg) ret= false; //�I��
		ty = sizeyhalf;
	}
			
    for(wy=0 ; wy < bmdh ; wy +=sizey )
    {
        for(wx=0 ; wx < bmdw ; wx +=sizex )
        {
			//   |���̃^�C���̒��S| ���ꂪ��{�^
			sx = wx + sizexhalf - (tx);
			sy = wy + sizeyhalf - (ty);
			gw = tx<<1;
			gh = ty<<1;
			//���x�͗����邪�N���b�s���O���Ă���
			if (sx+gw >= bmdw) gw = bmdw - sx;
			if (sy+gh >= bmdh) gh = bmdh - sy;
			if (gw > 0 && gh > 0)
			{
				Proc(bmd,sx,sy,gw,gh, OutBmd,x + sx,y + sy);
			}
        }
    }
    timecounter +=speed ;
	return ret;
}

#endif  //LIFE END
