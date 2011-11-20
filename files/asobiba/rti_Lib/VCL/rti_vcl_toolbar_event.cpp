/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef TOOLBAR_LIFE

#include <.\VCL\rti_vcl_toolbar_event.h>

/**********************************************************/
//�R���X�g���N�^   ���������܂�...
/**********************************************************/
TToolBarEvent::TToolBarEvent()
{
    //�{�^���̌�������
    ButtonCounter = 0;
}
/**********************************************************/
//�f�X�g���N�^   
/**********************************************************/
TToolBarEvent::~TToolBarEvent()
{
    ToolBarUsingIT it;
    for(it  = VToolBarUsing.begin() ;
		it != VToolBarUsing.end() ; it ++)
	{
	    delete (*it);
	}
	//���X�g�̂��ׂẴf�[�^�̔j��
	VToolBarUsing.clear();
}
/**********************************************************/
//WM_COMMAND �C�x���g�f�B�X�p�b�`��
/**********************************************************/
void TToolBarEvent::WmCommandCome(WPARAM wParam)
{
    //�{�^������
    FindCommandToButton(wParam);
//	switch(HIWORD(wParam))
//	{
//	}
}

/**********************************************************/
//WM_COMMAND ����A�{�^����T��
/**********************************************************/
bool TToolBarEvent::FindCommandToButton(WPARAM wParam)
{
    //���̃��b�Z�[�W������
    ToolBarUsingIT it;
    for(it  = VToolBarUsing.begin() ;
		it != VToolBarUsing.end() ; it ++)
	{ 
	    if ((*it)->idCommand == wParam) break;  //����
	}
    if(it == VToolBarUsing.end()) return false ; //�������s

    //�C�x���g���N����
    if ((*it)->CallRtn)
            (*it)->CallRtn(this);
    return true;
}

#endif
