/**********************************************************/
// �{�^���̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#if defined(BUTTON_LIFE) || defined(CHECKBUTTON_LIFE) || defined(GROUPBOX_LIFE) || defined(RADIOBUTTON_LIFE)

#ifndef ___TBUTTONEVENTH
#define ___TBUTTONEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//�{�^���̃C�x���g�N���X
//�{�^��������R���|�[�l���g�͂�����p�������܂�.
//BUTTON CHECKBUTTON RADIOBUTTON GROUPBOX

class TButtonEvent	: public TBaseObject
{
public:
	TButtonEvent();
	void WmCommandCome(WPARAM wParam);
	//�{�^�����Z�b�g����(�`�F�b�N������ (���W�I���L��))
	void Set()
		{SendMessage(hWnd,BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);};
	//�{�^�������Z�b�g����(�`�F�b�N������ (���W�I���L��))
	void ReSet()
		{SendMessage(hWnd,BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);};
	//�{�^���̏�Ԃ𓾂�(���W�I���L��)
	int Get()
		{return SendMessage(hWnd,BM_GETCHECK, 0L, 0L);};

	//BN_CLICKED	���[�U�[���{�^�����N���b�N�������Ƃ������܂��B 
	SimpleEvent	OnClick;
	//BN_HILITE	���[�U�[���{�^���𔽓]�\�����������Ƃ������܂��B 
	SimpleEvent	OnHilite;
	//BN_UNHILITE	���]�\������������K�v�����邱�Ƃ������܂��B 
	SimpleEvent	OnUnHilite;
};


#endif

#endif
