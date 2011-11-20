#include "StartWithExeFile.h"

#ifdef POWER_LIFE

#ifndef ___POWER_EFFECTH
#define ___POWER_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X

class TPowerEffect
{
private:
    void Init();

    struct _PowerChar;
    struct _PowerChar{
        bool alive;
        int x,y,z;
        int xspeed,yspeed,zspeed;
    };
    struct _PowerChar* PowerChar;

    BMD OutBmd;
	BMD *BmdTable;

	int xpowerpoint;
	int ypowerpoint;
	int zpowerpoint;
	int samplingz; //z���W�̉��s��������ł�����T�C�Y�������e�N�X�`���̏k���o�[�W���������܂�
	int flow;      //�Ƃԗ��q�̐�
	int areatparsent; //�o������m��
	int speed;

	int kankakuz;
public:
	TPowerEffect();
    ~TPowerEffect();
	void Set(BMD bmd,BMD _OutBmd,
						 int _xpowerpoint,int _ypowerpoint,int _zpowerpoint,
						 int _areatparsent,int _flow,int _speed);
    void Draw(GraphicsProc Proc,int x,int y,int w,int h);
};

typedef TPowerEffect* EPOWER;

#endif

#endif   // LIFE END
