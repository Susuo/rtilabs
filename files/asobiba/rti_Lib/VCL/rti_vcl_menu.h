#include "StartWithExeFile.h"
#ifdef MENU_LIFE

#ifndef ___MENUH
#define ___MENUH
#include <.\VCL\rti_vcl_object.h>

class TMenu ;

class TMenu : public TObject
{
private:
	//�V�K�쐬(���ׂĂ��̃��[�`���Ŕ��������Ȃ��Ă͂Ȃ�Ȃ�)
	void CreateMenuObject(TObject* hOwner,char *Caption, DWORD dwStyle);
    void DestroyComponentMenu();
    void PopUpCancle(TObject *Owner);
protected:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
	//�I�u�W�F�N�g�̍폜
	void DestroyComponent();
	//�I�u�W�F�N�g�̏�����
    void InitComponent(){};
public:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
    TMenu::TMenu(TMenu* whOwner,char *Caption,DWORD wdwStyle);
	~TMenu(); //�R���X�g���N�^

    void WmCommandCome(WPARAM wParam);

    void SetId(UINT k){Id = k;};
    SimpleEvent MenuClick;
};

#endif

#endif //LIFE END
