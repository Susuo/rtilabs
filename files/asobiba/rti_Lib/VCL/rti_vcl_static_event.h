/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef STATICTEXT_LIFE

#ifndef ___TSTATIC_EVENTH
#define ___TSTATIC_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TStaticEvent	: public TBaseObject
{
public:

	TStaticEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
