/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#include <.\VCL\rti_vcl_list_event.h>

/**********************************************************/
//�R���X�g���N�^   ���������܂�...
/**********************************************************/
TListBoxEvent::TListBoxEvent()
{
	//LBN_SELCANCEL	���ڂ̑I�����L�����Z�����ꂽ���Ƃ������܂��B 
	OnCancel	= NULL;
	//LBN_SELCHANGE	�I�����ڂ��ύX����悤�Ƃ��Ă��邱�Ƃ������܂��B 
	OnChange	= NULL;
}
/**********************************************************/
//WM_COMMAND �C�x���g�f�B�X�p�b�`��
/**********************************************************/
void TListBoxEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case LBN_SELCANCEL:	//���ڂ̑I�����L�����Z�����ꂽ���Ƃ������܂��B 
		if (OnCancel) OnCancel(this);
		break;
	case LBN_SELCHANGE:	//�I�����ڂ��ύX����悤�Ƃ��Ă��邱�Ƃ������܂��B 
		if (OnChange) OnChange(this);
		break;
	}
}

#endif
