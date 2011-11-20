#include "StartWithExeFile.h"
#ifdef GEAJI_LIFE

#ifndef ___GEAJIH
#define ___GEAJIH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>
/*****************************************************************/
//�Q�[�W
/*****************************************************************/

class TGeaji
{
private:
    BMD bmd;    //�G
    int bmdx , bmdy , bmdw, bmdh;
    int max;   //�ő�l
    int min;   //�ŏ��l
    int distance;  //MAX �� MIN �̍�
    int true_width;  //�����ɊG�悵��������
    int draw_width; //���݂̊G�悷�鉡��
    int value;      //�����ȃQ�[�W�̒l
    int GeajiSpeed; //�Q�[�W�̃X�s�[�h
public:
    TGeaji(){};
    ~TGeaji(){}; 
    void Set(BMD pbmd,int px,int py ,int pw,int ph,int pmax,int pmin,int _GeajiSpeed);
    void ReSetMax(int pmax);
    void NowValue(int value);
    void AfterValue(int value);
	void Draw(GraphicsProc Proc,int x,int y);
};

typedef TGeaji* GEAJI;

#endif

#endif //LIFE END
