/**********************************************************/
// �{�^���̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#if defined(BUTTON_LIFE) || defined(CHECKBUTTON_LIFE) || defined(GROUPBOX_LIFE) || defined(RADIOBUTTON_LIFE)

#include <.\VCL\rti_vcl_button_event.h>

/**********************************************************/
//�R���X�g���N�^   ���������܂�...
/**********************************************************/
TButtonEvent::TButtonEvent()
{
	//BN_CLICKED	���[�U�[���{�^�����N���b�N�������Ƃ������܂��B 
	OnClick		= NULL;
	//BN_HILITE	���[�U�[���{�^���𔽓]�\�����������Ƃ������܂��B 
	OnHilite	= NULL;
	//BN_UNHILITE	���]�\������������K�v�����邱�Ƃ������܂��B 
	OnUnHilite	= NULL;
}

/**********************************************************/
//WM_COMMAND �C�x���g�f�B�X�p�b�`��
/**********************************************************/
void TButtonEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case BN_CLICKED:	//���[�U�[���{�^�����N���b�N�������Ƃ������܂��B 
		if(OnClick) OnClick(this);
		break;
	case BN_HILITE:	//���[�U�[���{�^���𔽓]�\�����������Ƃ������܂��B 
		if(OnHilite) OnHilite(this);
		break;
	case BN_UNHILITE:	//���]�\������������K�v�����邱�Ƃ������܂��B 
		if(OnUnHilite) OnUnHilite(this);
		break;
	}
}

#endif
