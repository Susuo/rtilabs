#include "StartWithExeFile.h"
#ifdef SAVE_MENU_LIST

#include <.\Game\rti_list_menu.h>
#include <.\Graphics\rti_backsurface.h>
/***************************************************************/
//���X�g���j���[
//DOOM ��@FF �Ȃǂ̃Z�[�u�X���b�g�݂����ȃ��c�����
/***************************************************************/

/***************************************************************/
//�R���X�g���N�^
/***************************************************************/
TListMenu::TListMenu()
{
    AllDel();
};

/***************************************************************/
//���ׂč폜
/***************************************************************/
void TListMenu::AllDel()
{
    Selected=0;
    Menu_Count=0;
    Fream  = NULL;
    Cursor = NULL;
}


/***************************************************************/
//�Z�b�g
/***************************************************************/
void TListMenu::Set(BMD fream,BMD cursor)
{
    Fream   = fream;        //���j���[�̘g
    Cursor  = cursor;       //�J�[�\��
}


/***************************************************************/
//�ǉ�
/***************************************************************/
int TListMenu::Add(BMD bmd)
{
	//�ǉ��s�\
	if (Menu_Count >= LIST_MENU_MAX) return -1;
	//�ۑ�
	Card[Menu_Count] = bmd;
	//���j���[�̐��𑝂₷
	Menu_Count++;
	//�ǉ������Ƃ����Ԃ� 0 �`
	return Menu_Count-1;
}

/***************************************************************/
//�폜
/***************************************************************/
bool TListMenu::Del(short num)
{
	//����Ȃ���͂Ȃ�!!
	if (num >= Menu_Count) return false;
	//���炷
	for(int i=0; i < (Menu_Count - num - 1) ; i ++)
		Card[i+num]=Card[i+num+1];
	//���j���[�̐������炷
	Menu_Count--;
	//�����A���ݑI��ł���Ƃ��낪���ł����ꍇ�A���炷
	if (Selected >= Menu_Count) Selected = 0;
	return true;
}

/***************************************************************/
//�ړ�
/***************************************************************/
void TListMenu::MenuUd(bool ud)
{
	if (ud)
	{ // ��
		Selected--;
		//�␳
		if (Selected < 0) Selected = Menu_Count-1;
	}
	else
	{ //��
		Selected++;
		//�␳
		if (Selected >= Menu_Count) Selected = 0;
	}
}

/***************************************************************/
//�G��
/***************************************************************/
void TListMenu::Draw(GraphicsProc Proc,int x,int y)
{
    BMD TempBmd;
    //�t���[��
    if (Fream)
        ::Draw(Fream,0,0,Fream->w,Fream->h,Sprite,x,y);
    int NowCardY = y;
    for (int i=0; i < Menu_Count ; i++)
    {
        TempBmd = Card[i];
        Proc(TempBmd,0,0,TempBmd->w,TempBmd->h,
            Sprite,x,NowCardY);
        if (i == Selected)
        {   //�J�[�\���G��
            if (Cursor)
               ::Draw(Cursor,0,0,Cursor->w,Cursor->h,Sprite,x,NowCardY);
        }
        NowCardY += TempBmd->h;
    }
}


#endif
