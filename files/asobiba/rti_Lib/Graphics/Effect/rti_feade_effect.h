#include "StartWithExeFile.h"
#ifdef FEADE_LIFE

#ifndef ___FEADE_EFFECTH
#define ___FEADE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X

class TFeadeEffect
{
private:
    BMD OutBmd; //�o��
    int GoalR,GoalG,GoalB;   //�ڕW�Ƃ���F
    int NowR,NowG,NowB;   //���݂̐F
    int SpeedR,SpeedG,SpeedB;   //�ς���X�s�[�h
    int NowFream;         //���݂̃t���[����
public:
	TFeadeEffect();
    ~TFeadeEffect();
    void Set(BMD OutBmd,int nr,int ng,int nb,int gr,int gg ,int gb,int speed);
	bool Draw();
    int NowFreamReat(){return NowFream;};
};

typedef TFeadeEffect* EFREADE;

#endif

#endif //LIFE END
