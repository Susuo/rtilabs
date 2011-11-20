/**********************************************************/
// �{�^���̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#include <.\VCL\rti_vcl_comb_event.h>

/**********************************************************/
//�R���X�g���N�^   ���������܂�...
/**********************************************************/
TCombEvent::TCombEvent()
{
	//CBN_CLOSEUP	�R���{ �{�b�N�X�̃��X�g �{�b�N�X���N���[�Y���ꂽ���Ƃ������܂��B 
	OnClose = NULL;
	//CBN_DROPDOWN	�R���{ �{�b�N�X�̃��X�g �{�b�N�X���h���b�v�_�E������Ă��邱�Ƃ������܂��B 
	OnDropDown = NULL;
	//CBN_EDITCHANGE	���[�U�[���G�f�B�b�g �R���g���[�����̃e�L�X�g��ύX�������Ƃ������܂��B 
	OnEditChange = NULL;
	//CBN_EDITUPDATE	����̃e�L�X�g���\������悤�Ƃ��Ă��邱�Ƃ������܂��B 
	OnEditUpDate = NULL;
	//CBN_SELENDCANCEL	���[�U�[�̑I�����ڂ��L�����Z������K�v�����邱�Ƃ������܂��B 
	OnChange = NULL;
	//CBN_SELENDOK	���[�U�[�̑I�����ڂ��L���ł��邱�Ƃ������܂��B 
	OnCancel = NULL;
	//CBN_SETFOCUS	�R���{ �{�b�N�X�����̓t�H�[�J�X���󂯎���Ă��邱�Ƃ������܂��B 
	OnOk = NULL;
}

/**********************************************************/
//WM_COMMAND �C�x���g�f�B�X�p�b�`��
/**********************************************************/
void TCombEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case CBN_CLOSEUP:	//�R���{ �{�b�N�X�̃��X�g �{�b�N�X���N���[�Y���ꂽ���Ƃ������܂��B 
		if (OnClose) OnClose(this);
		break;
	case CBN_DROPDOWN:	//�R���{ �{�b�N�X�̃��X�g �{�b�N�X���h���b�v�_�E������Ă��邱�Ƃ������܂��B 
		if (OnDropDown) OnDropDown(this);
		break;
	case CBN_EDITCHANGE:	//���[�U�[���G�f�B�b�g �R���g���[�����̃e�L�X�g��ύX�������Ƃ������܂��B 
		if (OnEditChange) OnEditChange(this);
		break;
	case CBN_EDITUPDATE:	//����̃e�L�X�g���\������悤�Ƃ��Ă��邱�Ƃ������܂��B 
		if (OnEditUpDate) OnEditUpDate(this);
		break;
	case CBN_SELCHANGE:	//�R���{ �{�b�N�X�ŐV�������X�g���ڂ��I�����ꂽ���Ƃ������܂��B 
		if (OnChange) OnChange(this);
		break;
	case CBN_SELENDCANCEL:	//���[�U�[�̑I�����ڂ��L�����Z������K�v�����邱�Ƃ������܂��B 
		if (OnCancel) OnCancel(this);
		break;
	case CBN_SELENDOK:	//���[�U�[�̑I�����ڂ��L���ł��邱�Ƃ������܂��B 
		if (OnOk) OnOk(this);
		break;
	}
}

#endif
