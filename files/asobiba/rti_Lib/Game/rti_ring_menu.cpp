#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#include <.\Game\rti_ring_menu.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_glookup.h>
#include <.\Base\rti_debug.h>
/********************************************************/
//�����O���j���[
/********************************************************/

/********************************************************/
//�R���X�g���N�^
/********************************************************/
TRingMenu::TRingMenu()
{
	Icon_Count = 0;
	Selected   = 0;
    Distance   = 0;
    Rotation   = 0;
    RotationSpeed = 1;
}

/********************************************************/
//�Z�b�g ���S����̋������Z�b�g
/********************************************************/
void TRingMenu::Set(short _Distance,short _RotationSpeed)
{
	Distance = _Distance;
    RotationSpeed = _RotationSpeed;
}

/********************************************************/
//���j���[�ɒǉ�
/********************************************************/
int TRingMenu::Add(struct Anime_Src_struct *as)
{
	//�ǉ��s�\
	if (Icon_Count >= ROTATE_MENU_MAX) return -1;
	//�ۑ�
	memcpy(&AS[Icon_Count],as,sizeof(Anime_Src_struct) );
	//���j���[�̐��𑝂₷
	Icon_Count++;
	//�A�C�R�����W�̍Čv�Z.
	CalsIconPoint();
	//�ǉ������Ƃ����Ԃ� 0 �`
	return Icon_Count-1;
}

/********************************************************/
//�폜
/********************************************************/
bool TRingMenu::Del(short num)
{
	//����Ȃ���͂Ȃ�!!
	if (num >= Icon_Count) return false;
	//���炷
	for(int i=0; i < (Icon_Count - num - 1) ; i ++)
		memcpy(&AS[i+num],&AS[i+num+1],sizeof(Anime_Src_struct) );
	//���j���[�̐������炷
	Icon_Count--;
	//�����A���ݑI��ł���Ƃ��낪���ł����ꍇ�A���炷
	if (Selected >= Icon_Count) Selected = 0;
	//�A�C�R�����W�̍Čv�Z.
	CalsIconPoint();
	return true;
}

/********************************************************/
//�A�C�R�����W�̍Čv�Z.
/********************************************************/
void TRingMenu::CalsIconPoint()
{
	int i;          //�J�E���^
	int seeta = 0;  //�p�x

    //�A�C�R���̊Ԋu.
	Icon_Kankaku = 256 / Icon_Count;

	for(i=0 ; i < Icon_Count ; i++)
	{
        Kankaku[i] = seeta;
		seeta += Icon_Kankaku;
	}
}

/********************************************************/
//��]
/********************************************************/
bool TRingMenu::MenuRotate(bool LR)
{
    if (Rotation != ROTATION_NONE) return false;
    //�ߋ��ɑI������Ă������̂�ۑ�
    OldSelected = Selected;
	if (LR)
	{ // �E��]
        Rotation = ROTATION_RIGHT;
		Selected--;
		//�␳
		if (Selected < 0) Selected = Icon_Count-1;
	}
	else
	{ //����]
        Rotation = ROTATION_LEFT;
		Selected++;
		//�␳
		if (Selected >= Icon_Count) Selected = 0;
	}
    Icon_Hosei = 0; //�A�C�R���␳�̏�����
	return true;
}

/********************************************************/
//�I�[�v���N���[�Y
/********************************************************/
bool TRingMenu::MenuOpenClose(bool Open)
{
	//�����ق��ɂ���Ă����疳��
    if (Rotation != ROTATION_NONE) return false;
	//�߂Â��鑬�x���ɂ�[
	DistanceSpeed = 40;
    //�ߋ��ɑI������Ă������̂�ۑ� (��]�������邽��)
	OldSelected = Selected;	//OpenClose �ł͂قڃ_�~�[�ł�.
	if (Open)
	{ // �I�[�v��
        Rotation = ROTATION_OPEN;
		Icon_HoseiDistance = 300;
		//�␳����ʒu�́A��]�I�������Ƃ��� 0 �ɂȂ�悤�ɂ��܂�.
		Icon_Hosei = -(Icon_HoseiDistance / DistanceSpeed) * RotationSpeed;
	}
	else
	{ //�N���[�Y
        Rotation = ROTATION_CLOSE;
		Icon_HoseiDistance = Distance;
	    Icon_Hosei = 0; //�A�C�R���␳�̏�����
	}
	return true;
}

/********************************************************/
//�ʏ�G��
/********************************************************/
void TRingMenu::NomalDraw(GraphicsProc Proc,int x,int y)
{
    int i;
	Anime_Src_struct *TempAS = &AS[Selected];
 	Angle1024 *lpKankaku = &Kankaku[0];
    Angle1024 TempKankaku;

	//���ݑI������Ă�����̂�^�񒆂ɕ\��
    Proc(TempAS->bmd,TempAS->x,TempAS->y,
		 TempAS->w,TempAS->h, Sprite, x,y);

	//�G��
	for (i = Selected ; i < Icon_Count ; i++)
	{
        TempKankaku = *lpKankaku;
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Distance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Distance,TempKankaku));
        //���̃f�[�^��
		TempAS++;
		lpKankaku++;
	}
	TempAS = &AS[0];
	for (i = 0; i < Selected ; i ++)
	{
        TempKankaku = *lpKankaku;
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Distance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Distance,TempKankaku));
        //���̃f�[�^��
		TempAS++;
		lpKankaku++;
	}
}

/********************************************************/
//��]�G��
/********************************************************/
void TRingMenu::RotationDraw(GraphicsProc Proc,int x,int y)
{
    //�\���p�x�ړ�
    switch(Rotation)
    {
    case ROTATION_RIGHT:    //�E��]
        //�p�x�ړ�
        Icon_Hosei += RotationSpeed;
        //�␳����肷���Ȃ��悤�ɂ��� (^^;
        if (Icon_Hosei > Icon_Kankaku)
        {
            Icon_Hosei = Icon_Kankaku;
            Rotation = ROTATION_NONE; //��]�̏I��
        }
        break;
    case ROTATION_LEFT:     //����]
        //�p�x�ړ�
        Icon_Hosei -= RotationSpeed;
        //�␳����肷���Ȃ��悤�ɂ��� (^^;
        if (Icon_Hosei < -Icon_Kankaku) 
        {
            Icon_Hosei = -Icon_Kankaku;
            Rotation = ROTATION_NONE; //��]�̏I��
        }
        break;
    }
	//�G�惋�[�`�����ĂԂ҂��
	SpDraw(Proc,x,y);
}

/********************************************************/
//�I�[�v���N���[�Y
/********************************************************/
void TRingMenu::OpenCloseDraw(GraphicsProc Proc,int x,int y)
{
	//�A�C�R������]�����Ċi�D������
    Icon_Hosei += RotationSpeed;
    //�\���p�x�ړ�
    switch(Rotation)
    {
    case ROTATION_OPEN:    //���[�h
        //�p�x�ړ�
        Icon_HoseiDistance -= DistanceSpeed;
        //�␳����肷���Ȃ��悤�ɂ��� (^^;
        if (Icon_HoseiDistance < Distance)
        {
            Icon_HoseiDistance = Distance;
			Icon_Hosei = 0;
            Rotation = ROTATION_NONE; //�I�[�v���̏I��
        }
        break;
    case ROTATION_CLOSE:     //���[�h
        //�p�x�ړ�
        Icon_HoseiDistance += DistanceSpeed;
        //�␳����肷���Ȃ��悤�ɂ��� (^^;
        if (Icon_HoseiDistance > 300) 
        {
            Rotation = ROTATION_NONE; //��]�̏I��
        }
        break;
    }
	//�G�惋�[�`�����ĂԂ҂��
	SpDraw(Proc,x,y);
}

/********************************************************/
//����A�C�R���G�惋�[�`�����҂��
/********************************************************/
void TRingMenu::SpDraw(GraphicsProc Proc,int x,int y)
{
    int i;
	Anime_Src_struct *TempAS = &AS[OldSelected];
 	Angle1024 *lpKankaku = &Kankaku[0];
    Angle1024 TempKankaku;

	//�G��
	for (i = OldSelected ; i < Icon_Count ; i++)
	{
        //�o�C�g�̌��E�ō��W�n���[�v����������
        TempKankaku = (Angle1024) (*lpKankaku + Icon_Hosei);
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Icon_HoseiDistance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Icon_HoseiDistance,TempKankaku));
        //���̃f�[�^��
		TempAS++;
		lpKankaku++;
	}
	TempAS = &AS[0];
	for (i = 0; i < OldSelected ; i ++)
	{
        //�o�C�g�̌��E�ō��W�n���[�v����������
        TempKankaku = (unsigned char ) (*lpKankaku + Icon_Hosei);
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Icon_HoseiDistance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Icon_HoseiDistance,TempKankaku));
        //���̃f�[�^��
		TempAS++;
		lpKankaku++;
	}
}

#endif

