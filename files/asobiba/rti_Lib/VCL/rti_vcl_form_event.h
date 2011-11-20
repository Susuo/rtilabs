/**********************************************************/
// �{�^���̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef FORM_LIFE

#ifndef ___TFORMEVENTH
#define ___TFORMEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
#include <.\VCL\rti_vcl_menu.h>

//�t�H�[���̃C�x���g�N���X

#define AUTOPOPUP_CLIENTADD		0x80000000
#define AUTOPOPUP_DISPLAYFLGS	0x40000000 //�\��OK
#define AUTOPOPUP_DEFAULT		0x20000000
#define AUTOPOPUP_PARENT		0x10000000 //�����̎q���̃N���b�N�ł�
class TFormEvent	: public TBaseObject 
{
private:
#ifdef MENU_LIFE
	TMenu *MainMenu;
	TMenu *PopupMenu;
#endif
	UINT PopupFlg;
    HWND *ShowModalUnEnabledWindows;
    unsigned short ShowModalUnEnabledWindowsCounter;
protected:
	//�����I�Ƀ|�b�v�A�b�v���o���T�[�r�X
	void PopupMenuAutoShow(UINT pos,UINT flg);
public:
	TFormEvent();
    ~TFormEvent();
	void WmCommandCome(WPARAM wParam){};
    void HideModal();
    void ShowModal();
#ifdef MENU_LIFE
	void SetMenu(TMenu *menu);
	void SetPopupMenu(TMenu *menu,UINT flg);
	void ShowPopupMenu(int x,int y,TMenu *menu);
#endif
};


#endif

#endif
