#include "StartWithExeFile.h"
#ifdef BARST_LIFE

#ifndef ___BARST_EFFECTH
#define ___BARST_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X

class TBarstEffect 
{
private:
    void Init();

    BMD OutBmd; //�o��
	BMD pbmd;
	struct BarstStruct;
	struct BarstStruct
	{
		bool alive;
		int org;       //���Ƃ��Ƃ���������ׂ��������Ƃ��� 
		int x,y,sx,sy; //���ݒn �� �ړ���
		int w,h;       //���ƍ���
	};
	struct BarstStruct *list;
	int listmax;
	int eopuf;
	int px,py;
	int pw,ph;   //�G�̏c���Ɖ���
	int pww,phh; //�����ړ�
    int pbasepeed ;
public:
	//���̕��ƍ����́A�G�̍����ł͂Ȃ��āA�؂蔲���l�p�`�̒����ł�
	TBarstEffect();
    ~TBarstEffect();
	void ReSet(int x,int y,bool set_possition,int basespeed);
	int Draw();
	void RevReSet(int x,int y,bool set_possition,int basespeed);
	int RevDraw();

    bool Set(BMD bmd,BMD _OutBmd,bool desth,int x,int y,int w,int h,int basespeed);
};

#endif

#endif //LIFE END
