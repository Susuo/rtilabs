/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef TABCOONTROL_LIFE

#ifndef ___TTABCOONTROL_EVENTH
#define ___TTABCOONTROL_EVENTH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TTabControlObject	: public TBaseObject
{
public:
    //���݂̃y�[�W�����擾����
    int GetPage(){return TabCtrl_GetCurSel(hWnd);};

	TTabControlObject();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
