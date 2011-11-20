#include "StartWithExeFile.h"

#ifdef BOXEFFECT_LIFE

#ifndef ___BOX_EFFECTH
#define ___BOX_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X

class TBoxEffect
{
private:
    void Init();
    BMD OutBmd; //�o��
    BMD SpBuf; //���[���Ƃ����Ƃ��̃X�v���C�g�o�b�t�@�̓��e�̃R�s�[
    int TableSize;
    struct _BoxBlock;
    struct _BoxBlock{
        bool alive;
        short x,y;      //�X�^�[�g�ʒu
        short nw,nh;    //�������ݔ͈�
        short sizex,sizey;
    };
    struct _BoxBlock* BoxBlockBuf;
    int Speed;
    int timecounter;
    int deedcount;
    short MaxXSize,MaxYSize;

public:
    TBoxEffect();
    ~TBoxEffect();
    void Set(BMD bmd,BMD _OutBmd,int sizex,int sizey,int speed,unsigned char kind);
    int Draw(int x,int y,GraphicsProc Proc);
};

typedef TBoxEffect* EBOX;

#endif

#endif //LIFE END

