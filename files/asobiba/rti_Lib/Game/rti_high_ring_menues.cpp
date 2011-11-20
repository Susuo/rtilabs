#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#include <.\Game\rti_high_ring_menues.h>
#include <.\Base\rti_key.h>
/********************************************************/
//�R���X�g���N�^
/********************************************************/
THighRingMenues::THighRingMenues()
{
    MenuKind = 0;
    MenuRotationEvent = NULL;
    MenuCloseEvent = NULL;
    MenuChangeEvent = NULL;

    JoyStick = NULL;

}

/********************************************************/
//�f�X�g���N�^
/********************************************************/
THighRingMenues::~THighRingMenues()
{
    std::vector<TRingMenu*>::iterator it;

    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = RingMenuList.begin() ; it != RingMenuList.end() ; it ++)
    {
        delete (*it);
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    RingMenuList.clear();

}

/********************************************************/
//�ǉ� �G�A��̃A�C�R���̕��ƍ����@���̊G�ɉ��̃A�C�R��������̂�.
/********************************************************/
TRingMenu* THighRingMenues::Add(BMD _bmd,int w,int h,int count)
{
	//�����O���j���[�쐬
	struct Anime_Src_struct AS;
    TRingMenu *Trm = new TRingMenu;
    //�����O���j���[�ǉ�
    RingMenuList.push_back(Trm);

	AS.bmd = _bmd;
	AS.y = 0;
	AS.w = w;
	AS.h = h;
	Trm->Set(80,6);
	for (int i = 0; i < count ; i++)
	{
		AS.x = (w+1)*i;
		Trm->Add(&AS);
	}
    return Trm;
}

/********************************************************/
//���j���[�����̂Ƃ��ɍŏ��ɌĂ�ł�(�́[��)
/********************************************************/
void THighRingMenues::Open()
{
    NowMenuClosing = false;
    NowMenuEnd = false;

	//���ݎd�l���Ă��郁�j���[���擾
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	//�I�[�v�����������
	NowMenu->MenuOpenClose(true);
}

/********************************************************/
//���j���[��{����
/********************************************************/
void THighRingMenues::Draw(GraphicsProc Proc,int x,int y)
{
	//�L�[����
    if (KeyPush(VK_LEFT) ) 		MenuRotation(false);
    if (KeyPush(VK_RIGHT) )		MenuRotation(true);
    if (KeyPushOne(VK_UP) ) 		MenuChange(true);
    if (KeyPushOne(VK_DOWN) )		MenuChange(false);
	if (KeyPushOne(0x5A) || KeyPushOne(0x58))   MenuClose();

	//�W���C�X�e�B�b�N���g����
	if (JoyStick && JoyStick->PadAlive())
	{
		JoyStick->JoyStickRef();
		if (JoyStick->KeyGet() & JOY_POS_RIGHT)	MenuRotation(true);
		if (JoyStick->KeyGet() & JOY_POS_LEFT)		MenuRotation(false);
		if (JoyStick->KeyOnesGet() & JOY_POS_TOP)		MenuChange(true);
		if (JoyStick->KeyOnesGet() & JOY_POS_DOWN)		MenuChange(false);
		if (JoyStick->ButtonOnesGet() & (JOY_BTN_A | JOY_BTN_B) )  MenuClose();
	}

	//���ݎd�l���Ă��郁�j���[���擾
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	//�G��
	NowMenu->Draw(Proc,x,y);

    //���j���[�̏I�����������Ă���̂Ȃ��...
	MenuClosingProf();
	
}


/********************************************************/
//��]�Ȃ�
/********************************************************/
void THighRingMenues::MenuRotation(bool LR)
{
	//���ݎd�l���Ă��郁�j���[���擾
	TRingMenu *NowMenu = RingMenuList[MenuKind];
    //��]���ߔ���
	NowMenu->MenuRotate(LR);
    //��]�����Ƃ����C�x���g
    if (MenuRotationEvent) MenuRotationEvent(this,NowMenu,MenuKind);
}

/********************************************************/
//���j���[�̐؂�ւ����n�߂�
/********************************************************/
void THighRingMenues::MenuChange(bool Ud)
{
	//���ݎg�p���Ă��郁�j���[���擾
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	if (NowMenu->MenuOpenClose(false) )
	{	//���݂̃��j���[�̃N���[�Y�������F!!
		NowMenuClosing = true;
		//�ǂ����ɐ؂�ւ���̂��ۑ�
		NowMenuClosingBeforUd = Ud;
	}
}

/********************************************************/
//���j���[�̐؂�ւ�����������
/********************************************************/
void THighRingMenues::MenuClosingProf()
{
	if (NowMenuClosing)
	{	//���݂̃��j���[�̃N���[�Y�������ł�!
		//���ݎg�p���Ă��郁�j���[���擾
		TRingMenu *NowMenu = RingMenuList[MenuKind];
		if (NowMenu->GetNowNone() )
		{	//�I���������I������.
			NowMenuClosing = false;
            if (NowMenuEnd)
            {   //���j���[����������ꍇ
                MenuCloseDo(NowMenu);
            }
            else
            {   //���ۂɐ؂�ւ���.
			    MenuChangeDo();
            }
		}
	}
}

/********************************************************/
//���j���[�̐؂�ւ�����.
/********************************************************/
void THighRingMenues::MenuChangeDo()
{
    int menu_count = RingMenuList.size();
    menu_count--;
    while(1)
    {
	    if (NowMenuClosingBeforUd) 
    	{
	    	MenuKind++;
		    if (MenuKind > menu_count) MenuKind = 0;
    	}
    	else
    	{
	    	MenuKind--;
		    if (MenuKind < 0) MenuKind = menu_count;
	    }
        if (!MenuChangeEvent) break;
        if (MenuChangeEvent(MenuKind) ) break;
    }
    //�V�K�ɐ؂�ւ������j���[�ɑ΂��ăI�[�v��������������.
    TRingMenu *NowMenu = RingMenuList[MenuKind];
    NowMenu->MenuOpenClose(true);
}

/********************************************************/
//���j���[�����߂鏀��������
/********************************************************/
void THighRingMenues::MenuClose()
{
	//���ݎg�p���Ă��郁�j���[���擾
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	if (NowMenu->MenuOpenClose(false) )
	{	//���݂̃��j���[�̃N���[�Y�������F!!
		NowMenuClosing = true;
        //���j���[�����Ƃ������}
		NowMenuEnd = true;
	}
}


/********************************************************/
//���j���[�����߂�
/********************************************************/
void THighRingMenues::MenuCloseDo(TRingMenu *NowMenu)
{
    //���j���[����ɕ���Ƃ������}�𑗂�.
    NowMenu->MenuClose();
    //�I�������Ƃ����C�x���g
    if (MenuCloseEvent) MenuCloseEvent(this,NowMenu,MenuKind);
}


/********************************************************/
//�w�肳�ꂽ���j���[���擾
/********************************************************/
TRingMenu* THighRingMenues::GetMenu(int no)
{
    if (no < 0) return NULL;
    if (no > RingMenuList.size())   return NULL;
    return RingMenuList[no];
}


#endif		//LIFE END
