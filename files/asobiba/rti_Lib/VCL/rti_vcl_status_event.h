/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef STATUS_LIFE

#ifndef ___TSTATUSHXX_EVENTH
#define ___TSTATUSHXX_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TStatusEvent	: public TBaseObject
{
public:

	TStatusEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
