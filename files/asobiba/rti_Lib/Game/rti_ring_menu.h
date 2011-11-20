#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#ifndef __RING_MENU_LISTH
#define __RING_MENU_LISTH


#include <windows.h>
#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_anime_ex.h>

#define ROTATE_MENU_MAX 20

#define ROTATION_NONE  0x00
#define ROTATION_RIGHT 0x01
#define ROTATION_LEFT 0x02
#define ROTATION_OPEN 0x04
#define ROTATION_CLOSE 0x08

class TRingMenu
{
	struct Anime_Src_struct AS[ROTATE_MENU_MAX]; //�f�[�^
    Angle1024 Kankaku[ROTATE_MENU_MAX];
    Angle1024 Icon_Count;					//���݂̃A�C�R���̐�
	char Selected; 								//���ݑI�����Ă���Ƃ���
    short Icon_Kankaku;             //�A�C�R����z�u���銴�o
    short RotationSpeed;            //��]���x

	//�I�[�v���N���[�Y�̏����p
	short Distance;					//���S����̋���
	short Icon_HoseiDistance;		//���S����̋����␳
	short DistanceSpeed;			//���x�����
    //��]�A�j���[�V�����p
    Angle1024 Rotation;
    char OldSelected;               //��]���n�߂�O�ɑI��ł������c
    short Icon_Hosei;               //�A�C�R����␳

	//�A�C�R�����W�̍Čv�Z.
	void CalsIconPoint();
    void NomalDraw(GraphicsProc Proc,int x,int y);      //�ʏ�G��

	//����A�C�R���G�惋�[�`�����҂��
	void SpDraw(GraphicsProc Proc,int x,int y);
    void RotationDraw(GraphicsProc Proc,int x,int y);   //��]�G��
	void OpenCloseDraw(GraphicsProc Proc,int x,int y);	//�I�[�v���N���[�Y
public:
	TRingMenu();
	int GetSelect(){return Selected;};
	void SetSelect(char _Selected){Selected = _Selected;};
    void Set(short _Distance,short _RotationSpeed);
	int Add(struct Anime_Src_struct *AS);	//���j���[�ɒǉ�
	bool Del(short num);	//���j���[����폜

	bool MenuRotate(bool LR);	//���j���[��]
	bool MenuOpenClose(bool Open);	//�I�[�v���N���[�Y�A�j���[�V����
    void MenuClose(){Rotation = ROTATION_NONE;};        //���j���[�I�� ��]����������.
    bool GetNowNone(){return Rotation == ROTATION_NONE;}; //�����Ȃ��ʏ탂�[�h�������� true

	void Draw(GraphicsProc Proc,int x,int y)
    {
		switch(Rotation)
		{
		case ROTATION_NONE:
			NomalDraw(Proc,x,y);
			break;
		case ROTATION_RIGHT:
		case ROTATION_LEFT:
			RotationDraw(Proc,x,y);
			break;
		case ROTATION_OPEN:
		case ROTATION_CLOSE:
			OpenCloseDraw(Proc,x,y);	//�I�[�v���N���[�Y
			break;
		}
    }
};


#endif

#endif
