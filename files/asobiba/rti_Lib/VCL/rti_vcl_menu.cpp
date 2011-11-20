/**********************************************************/
// ���j���[�̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef MENU_LIFE

#include <.\VCL\rti_vcl_menu.h>
#include <.\VCL\rti_vcl_apprication.h>

void TMenu::CreateMenuObject(TObject* hOwner,char *Caption, DWORD dwStyle)
{
    //�I�[�i�[�ۑ�
    hMyOwner = hOwner;
    hWnd = NULL;
    if (hMyOwner==NULL || hMyOwner->GetKind() != OBJECT_KIND_MENU)
    { //�g�b�v
        hWnd = CreateMenu();
        return;
    }
    //�|�b�v�A�b�v�����K�v�����邩..
    if(hMyOwner->GethWnd() == NULL)
    {
		char buf[256];
		UINT sstate;
        TObject *wDOwner = hMyOwner->GethOwer();
        //�I�[�i�[���z�b�v�A�b�v��������.
	    ((TMenu*)hMyOwner)->SethWnd(CreatePopupMenu());
//        DEBUG3("�m�� %d",(int)hMyOwner->GethWnd());

		//���̃I�[�i�[�̕�����擾
		GetMenuString(wDOwner->GethWnd(),hMyOwner->GetId(),
			buf,255,MF_BYCOMMAND);
		//���̃I�[�i�[�̃X�e�[�^�X�擾
		sstate =  GetMenuState(wDOwner->GethWnd(),
			hMyOwner->GetId(),MF_BYCOMMAND);
		//���̃I�[�i�[�̑�����ύX���܂�
		ModifyMenu
			(wDOwner->GethWnd(),hMyOwner->GetId(),
			 MF_BYCOMMAND | MF_POPUP | sstate,
			 (UINT)hMyOwner->GethWnd(),buf);
//		((TMenu*)hMyOwner)->SetId ( (UINT)hMyOwner->GethWnd() ); 
    }
	this->Id = Application->GetId();
	//��������Ȃ� �Ȃ�΁A�ŏ��̕��͎����̐e�ƂȂ�ׂ����j���[�ł���
    AppendMenu( hMyOwner->GethWnd(), dwStyle,(UINT)this->Id, Caption);

}

TMenu::TMenu(TMenu* whOwner,char *Caption,DWORD wdwStyle)
{
    MenuClick = NULL;

    CreateMenuObject(whOwner,Caption,wdwStyle);

    OwnerAdd(whOwner,this);

    Kind            = OBJECT_KIND_MENU;
}

TMenu::~TMenu()
{
    DestroyComponent();
}

/**********************************************************/
// �j��
/**********************************************************/
void TMenu::DestroyComponent()
{
    OwerKill        = true;
    //�����̎����Ă������ׂẴR���g���[�������
    ChildDestroy();
    //�����̃I�[�i�[�Ɏ����������邱�Ƃ�ʒm
    IamDestroy();
    //���j���[�j��
    DestroyComponentMenu();
}

/**********************************************************/
// ���j���[�̔j��
/**********************************************************/
void TMenu::DestroyComponentMenu()
{
    if(hWnd)
    { //�����̓��j���[�n���h���������Ă���.
//        DEBUG3("��� %d",(int)hWnd);
        DestroyMenu(hWnd); //���j���[�n���h�������ł�����.
    }
    else
    { //���j���[�n���h���������Ȃ����ڂł���.
//        DeleteMenu(GethOwerWnd(),Id,MF_BYCOMMAND);
    }
    //�����̃I�[�i�[�����j���[��������
    if(hMyOwner && hMyOwner->GetKind() == OBJECT_KIND_MENU)
    { //�|�b�v�A�b�v���������ǂ�������
        if(hMyOwner->HowMenyHaveControls() == 0)
        { //�I�[�i�[(�|�b�v�A�b�v)�̓R���g���[�������ׂĎ����܂���.
//            PopUpCancle(hMyOwner); //�|�b�v�A�b�v�L�����Z��
        }
    }
}

/**********************************************************/
// �|�b�v�A�b�v�L�����Z��
/**********************************************************/
void TMenu::PopUpCancle(TObject *hOwner)
{
	char buf[256];
	UINT sstate;
    TObject *wDOwner = hOwner->GethOwer();
    //�O�̂��߂��̃I�[�i�[�̃I�[�i�[�����j���[���ǂ������ׂ�
    if (!wDOwner || wDOwner->GetKind() != OBJECT_KIND_MENU) return ;

    //���̃I�[�i�[�̕�����擾
	GetMenuString(wDOwner->GethWnd(),hOwner->GetId(),
		buf,255,MF_BYCOMMAND);
	//���̃I�[�i�[�̃X�e�[�^�X�擾
	sstate =  GetMenuState(wDOwner->GethWnd(),
		hMyOwner->GetId(),MF_BYCOMMAND);
    //�|�b�v�A�b�v���Ւf
	sstate = sstate & ~(MF_POPUP) | MF_BYCOMMAND;

//	hMyOwner->SetId ( (UINT)hMyOwner->GetWnd() ); 
    //�I�[�i�[���ꎞ�I�ɏ��ł�����
    DestroyMenu(hOwner->GethWnd() ); //���j���[�n���h�������ł�����.

    AppendMenu(wDOwner->GethWnd(), sstate,hOwner->GetId(),buf);

}


/**********************************************************/
//���j���[ WM_COMMAND �f�B�X�p�b�`�� 
/**********************************************************/
void TMenu::WmCommandCome(WPARAM wParam)
{
    if(MenuClick) MenuClick(this);
//    DEBUG("Menu Event");
}


#endif // LIFE END

