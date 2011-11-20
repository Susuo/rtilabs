/**********************************************************/
// �t�H�[���̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef FORM_LIFE

#include <.\VCL\rti_vcl_form_event.h>
#include <.\VCL\rti_vcl_apprication.h>

/**********************************************************/
/*�R���X�g���N�^*/
/**********************************************************/
TFormEvent::TFormEvent()
{
#ifdef MENU_LIFE
	MainMenu	= NULL;
	PopupMenu	= NULL;
#endif
    //�t�H�[�������[�_���ŊJ�����Ƃ��ɏ�񂪊i�[����܂�.
    //�܂�A������ NULL �������ꍇ�́A���[�_���ł͂Ȃ����Ă��Ƃł�.
    ShowModalUnEnabledWindows = NULL;
}

/**********************************************************/
/*�f�X�g���N�^*/
/**********************************************************/
TFormEvent::~TFormEvent()
{
    HideModal();
}

/**********************************************************/
/*�t�H�[���̃��[�_�����������܂�*/
/**********************************************************/
void TFormEvent::HideModal()
{
    if (ShowModalUnEnabledWindows == NULL) return; //���[�_���ł͂Ȃ�
    for(int i = 0; i <= ShowModalUnEnabledWindowsCounter ; i++)
    {
        HWND hwnd = ShowModalUnEnabledWindows[i];
        if(IsWindow(hwnd))
        { //���̃E�B���h�͂܂��L��
            //�L���ɂ���
            EnableWindow(hwnd,TRUE);
        }
    }
	GlobalFree(ShowModalUnEnabledWindows);
    ShowModalUnEnabledWindows = NULL;

    //�����𖳌��ɂ��܂�.
    ShowWindow(GethWnd(),SW_HIDE);
}

/**********************************************************/
/*�t�H�[�������[�_���ŊJ���܂�*/
/**********************************************************/
void TFormEvent::ShowModal()
{
    //�����ȊO�̂��ׂẴE�B���h�n���h�����擾���Ă����܂�.
    //�܂��A�����ȊO�����ׂẴt�H�[���𖳌��ɂ��܂�.
    int i = 0;
    ObjectIT it;
    HWND hwnd;

    ShowModalUnEnabledWindowsCounter = Application->HowMenyHaveControls();
	ShowModalUnEnabledWindows = (HWND*) 
        GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
			sizeof(HWND) * (ShowModalUnEnabledWindowsCounter +1) );

    
    for(it=Application->GetComponentFarst();
        it != Application->GetComponentEnd() ; it++)
        {
            hwnd = (HWND)( (*it)->GethWnd() );
            if(this->hWnd != hwnd)
            { //�������g�Ɣ�ׂȂ��悤��...
				if (IsWindow(hwnd) && IsWindowEnabled(hwnd) && IsWindowVisible(hwnd))
				{
					ShowModalUnEnabledWindows[i++] = hwnd;
					//�����ɂ���
					EnableWindow(hwnd,FALSE);
				}
            }
        }
    //������L���ɂ��܂�.
    ShowWindow( GethWnd() ,SW_SHOW);
}

#ifdef MENU_LIFE
/*******************************************************************/
//���j���[
/*******************************************************************/
void TFormEvent::SetMenu(TMenu *menu)
{
	MainMenu	= menu;
//    SetMenu(menu);
    DrawMenuBar(hWnd);
}

/*******************************************************************/
//�|�b�v�A�b�v���j���[
/*******************************************************************/
void TFormEvent::SetPopupMenu(TMenu *menu,UINT flg )
{
	PopupMenu	= menu;
	PopupFlg    = flg;
}

/*******************************************************************/
//�|�b�v�A�b�v���j���[��\��
/*******************************************************************/
void TFormEvent::ShowPopupMenu(int x,int y,TMenu *menu)
{
	TrackPopupMenu(PopupMenu->GethWnd() , NULL,
		x, y, 0 ,hWnd, NULL);
}
#endif

/*******************************************************************/
//�|�b�v�A�b�v���j���[�������I�ɓ��삷��T�[�r�X
/*******************************************************************/
void TFormEvent::PopupMenuAutoShow(UINT pos,UINT flg)
{
#ifdef MENU_LIFE
	if (PopupMenu)
	{ //�|�b�v�A�b�v���j���[����A�\�����Ă���.
		UINT disflg = PopupFlg & AUTOPOPUP_DISPLAYFLGS;
		if( (disflg&flg) == disflg)
		{
			POINT p;
			p.x  = LOWORD(pos);
			p.y  = HIWORD(pos);
			if (PopupFlg & AUTOPOPUP_CLIENTADD)
			{ //���W�ϊ�
				ClientToScreen(hWnd,&p);
			}
			ShowPopupMenu(p.x,p.y,PopupMenu);
		}
	}
#endif
}

#endif
