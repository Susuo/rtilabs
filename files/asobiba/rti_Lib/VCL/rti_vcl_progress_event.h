/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef PROGRESS_LIFE

#ifndef ___TPROGRESS_EVENTH
#define ___TPROGRESS_EVENTH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TProgressEvent	: public TBaseObject
{
public:
    //����ŁA�v���O���X�o�[�͈̔͂����߂܂��B �f�t�H���g�ł͂O����P�O�O�܂łƂȂ��Ă��܂��B 
    void SetRange(WORD nMinRange,WORD nMaxRange)
    {SendMessage(hWnd,PBM_SETRANGE, 0L , MAKELPARAM(nMinRange, nMaxRange));};

    //����ŁA�P��̑������w�肵�܂��B 
    void SetStep(int nStepInc) 
    {SendMessage(hWnd,PBM_SETSTEP, (WPARAM) nStepInc ,0L);};

    //����𑗐M����ƂP�X�e�b�v�������i�݂܂��B ���^���ɂȂ����Ƃ��͂܂��A�O����n�܂�܂��B
    void StepIt() 
    {SendMessage(hWnd,PBM_STEPIT, 0L ,0L);};


	TProgressEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
