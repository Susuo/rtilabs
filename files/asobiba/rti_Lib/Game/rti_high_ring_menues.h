#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#ifndef __HIGH_RING_MENUES_LISTH
#define __HIGH_RING_MENUES_LISTH

#include <vector>
#include <.\Game\rti_ring_menu.h>
#include <.\Base\rti_joystick.h>

class THighRingMenues;

//���j���[����]�A�I�������Ƃ���..
typedef void (*RingMenuEvent)
        (THighRingMenues* Menues,TRingMenu* NowMenu,int MenuKind);
//���j���[�؂�ւ��̑O�Ƀ`�F���W
typedef bool (*RingMenuChange)(int MenuKind);

class THighRingMenues
{
    bool NowMenuEnd;        //���j���[�𔲂��邽�߂ɃN���[�Y���Ă��܂�.
    bool NowMenuClosing ;	//���݃��j���[����鏈�����Ă܂�.
    bool NowMenuClosingBeforUd;	//���j���[������玟�̃��j���[�� �㉺�̂ǂ���?
    int MenuKind ;              //���ݎg�p���Ă��郁�j���[
    TJoyStick *JoyStick;        //�W���C�X�e�B�b�N
    std::vector<TRingMenu*> RingMenuList; //�����O���j���[���X�g.

    //��]�Ȃ�
    void MenuRotation(bool LR);
    //���j���[�̐؂�ւ�����������
    void MenuClosingProf();

    //���j���[�̐؂�ւ����n�߂�
    void MenuChange(bool Ud);
    //���j���[�̐؂�ւ�����.
    void MenuChangeDo();

    //���j���[�����߂鏀��������
    void MenuClose();
    //���j���[�����߂�
    void MenuCloseDo(TRingMenu *NowMenu);
public:
    //���j���[����]�������ɌĂ΂��
    RingMenuEvent MenuRotationEvent;
    //���j���[�������Ƃ��ɌĂ΂��
    RingMenuEvent MenuCloseEvent;
    //���j���[���`�F���W�����Ƃ��ɌĂ΂��.
    RingMenuChange MenuChangeEvent;
    //�R���X�g���N�^
    THighRingMenues();
    //�f�X�g���N�^
    ~THighRingMenues();
    //�ǉ� �G�A��̃A�C�R���̕��ƍ����@���̊G�ɉ��̃A�C�R��������̂�.
    TRingMenu* Add(BMD _bmd,int w,int h,int count);
    //���j���[�B���J���̂ł�!
    void Open();
    //��{����
	void Draw(GraphicsProc Proc,int x,int y);
    //�A�N�Z�X�֐�
    TRingMenu* GetMenu(int no);
	//�W���C�X�e�B�b�N�o�^
	void SetJoyStick(TJoyStick *j){JoyStick = j;};
};

#endif

#endif		//LIFE END
