/**********************************************************/
// �G�f�B�g�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#include <.\VCL\rti_vcl_edit_event.h>

/**********************************************************/
//�R���X�g���N�^   ���������܂�...
/**********************************************************/
TEditEvent::TEditEvent()
{
	//�ҏW���ꂽ�Ƃ�
	OnChange  = NULL;
	//EN_HSCROLL	���[�U�[���X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
	OnHScroll  = NULL;
	//EN_MAXTEXT	�}���������؂�̂Ă�ꂽ���Ƃ������܂��B 
	OnMaxText  = NULL;
	//EN_UPDATE	�G�f�B�b�g �R���g���[��������̃e�L�X�g��\�����悤�Ƃ��Ă��邱�Ƃ������܂��B 
	OnUpdate  = NULL;
	//EN_VSCROLL	���[�U�[�������X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
	OnVscroll  = NULL;
}

/**********************************************************/
//���͂���Ă��郄�c�� int �^�ɕϊ����ĕԂ��܂�
/**********************************************************/
int TEditEvent::GetInt()
{
    char buf[256];
    //�܂����͂���Ă��郄�c���擾
    GetCaption(buf,256);
    return atoi(buf);
}

/**********************************************************/
//WM_COMMAND �C�x���g�f�B�X�p�b�`��
/**********************************************************/
void TEditEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case EN_CHANGE:   //�ҏW���ꂽ�Ƃ�
		if (OnChange) OnChange(this);
		break;
	case EN_HSCROLL:	//���[�U�[���X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
		if (OnHScroll) OnHScroll(this);
		break;
	case EN_MAXTEXT:	//�}���������؂�̂Ă�ꂽ���Ƃ������܂��B 
		if (OnMaxText) OnMaxText(this);
		break;
	case EN_UPDATE:		//�G�f�B�b�g �R���g���[��������̃e�L�X�g��\�����悤�Ƃ��Ă��邱�Ƃ������܂��B 
		if (OnUpdate) OnUpdate(this);
		break;
	case EN_VSCROLL:	//���[�U�[�������X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
		if (OnVscroll) OnVscroll(this);
		break;
	}
}

#endif
