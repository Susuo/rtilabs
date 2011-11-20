#include "StartWithExeFile.h"
#ifdef FEADE_LIFE

/*****************************************************************/
/*****************************************************************/
//�w�肵���F�ɋ߂Â���
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_feade_effect.h>

/*****************************************************************/
//RGB �́A ���ꂼ�� 255 �` -255 .  ���ׂ�255 �ɂ���Ɣ�.
//                                 ���؂�-255 �ɂ���ƍ�.
/*****************************************************************/
TFeadeEffect::TFeadeEffect()
{
}

TFeadeEffect::~TFeadeEffect()
{
}

/*****************************************************************/
//RGB �́A ���ꂼ�� 255 �` -255 .  ���ׂ�255 �ɂ���Ɣ�.
//                                 ���؂�-255 �ɂ���ƍ�.
/*****************************************************************/
void TFeadeEffect::Set(BMD _OutBmd,int nr,int ng,int nb,int gr,int gg ,int gb,int fream)
{
    OutBmd = _OutBmd; //�o��
    //�ړ��������F
    GoalR = gr;
    GoalG = gg;
    GoalB = gb;
    //�����F
    NowR  = nr;
    NowG  = ng;
    NowB  = nb;

    //���x�����Ƃ�߂�
    SpeedR = (GoalR - NowR) / fream;
    SpeedG = (GoalG - NowG) / fream;
    SpeedB = (GoalB - NowB) / fream;

    //���݂̃t���[���� 0 �ɂȂ�����I��.
    NowFream = fream + 1;
}


/****************************************************************/
//�ړI�̐F�ɋ߂Â��郁�C�� �I���񂾂����� false ��Ԃ�
/****************************************************************/
bool TFeadeEffect::Draw()
{
    bool ret;
    //�t�B���^��������
    FilterDraw(OutBmd,0,0,OutBmd->w,OutBmd->h,NowR,NowG,NowB);
    //�t�B���^��ό`���Ă����܂�.
    if (NowFream != 0)
    {//�F�̃`�F���W���Ă����܂�.
        NowR  += SpeedR;
        NowG  += SpeedG;
        NowB  += SpeedB;
        NowFream--;
        ret = true;
    }
    else
    { //�I���@�O�̂��߃S�[���̐F�������Ă���.
        NowR  = GoalR;
        NowG  = GoalG;
        NowB  = GoalB;
        ret = false;
    }
    return ret;
}

#endif //LIFE END
