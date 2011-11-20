#include "StartWithExeFile.h"
#ifdef GEAJI_LIFE
#include <.\Game\rti_geaji.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_backsurface.h>
/*****************************************************************/
//�Q�[�W���[�`��
/*****************************************************************/

/*****************************************************************/
//�Q�[�W�̏����ݒ�
/*****************************************************************/
void TGeaji::Set(BMD pbmd,int px,int py ,int pw,int ph,int pmax,int pmin,int _GeajiSpeed)
{
    bmd = pbmd; //�Q�[�W�̊G
    bmdx = px ; //���̊G���X�^�[�g������ X
    bmdy = py ; //���̊G���X�^�[�g������ Y
    bmdw = pw ; //���̊G�̕�
    bmdh = ph ; //���̊G�̍���

    max = pmax; //�Q�[�W�̍ő�l
    min = pmin; //�Q�[�W�̍ŏ��l
    distance = (pmax - pmin); //����
    draw_width = 0; 
    true_width = 0;
    GeajiSpeed = _GeajiSpeed; //�Q�[�W�̃X�s�[�h
    value = pmin;
}

/*****************************************************************/
//�ő�l��ύX������
/*****************************************************************/
void TGeaji::ReSetMax(int pmax)
{
    max = pmax;
    distance = (max - min); //����

    //�O�̂��߂��Ƃߒ���
    draw_width = bmdw * value / distance;
    true_width = draw_width;
}

/*****************************************************************/
//�w�肵���l�ɃQ�[�W��ύX����
/*****************************************************************/
void TGeaji::NowValue(int pvalue)
{
    //�ȈՃN���b�s���O
    if (pvalue > max) pvalue = max;
    if (pvalue < min) pvalue = min;
    //�Q�[�W�̍ő� : distance = X : value �̔䗦������
    draw_width = bmdw * pvalue / distance;
    true_width = draw_width;
    //�Q�[�W�̒l��ύX
    value = pvalue;
}

/*****************************************************************/
//�w�肵���l�ɃQ�[�W���������ړ������ĕύX����
/*****************************************************************/
void TGeaji::AfterValue(int pvalue)
{
    //�ȈՃN���b�s���O
    if (pvalue > max) pvalue = max;
    if (pvalue < min) pvalue = min;
    //�Q�[�W�̍ő� : distance = X : value �̔䗦������
    true_width = bmdw * pvalue / distance;
    //�Q�[�W�̒l��ύX
    value = pvalue;
}


/*****************************************************************/
//�Q�[�W�̊G��
/*****************************************************************/
void TGeaji::Draw(GraphicsProc Proc,int x,int y)
{
    Proc(bmd,bmdx,bmdy,draw_width,bmdh,Sprite,x,y);
    //�������Ă����Q�[�W�̏ꍇ��.......
    if (draw_width != true_width)
    {
        if(draw_width < true_width)
        {
            draw_width+=GeajiSpeed;
            if (draw_width > true_width) draw_width = true_width; //�␳
        }
        else
        {
            draw_width-=GeajiSpeed;
            if (draw_width < true_width) draw_width = true_width; //�␳
        }
    }

}

#endif
