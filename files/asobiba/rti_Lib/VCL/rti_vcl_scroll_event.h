/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef SCROLLBAR_LIFE

#ifndef ___TSCROLLBAR_EVENTH
#define ___TSCROLLBAR_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TScrollEvent	: public TBaseObject
{
public:

	TScrollEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
