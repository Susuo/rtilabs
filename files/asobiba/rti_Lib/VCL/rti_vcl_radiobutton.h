#include "StartWithExeFile.h"
#ifdef RADIOBUTTON_LIFE

#ifndef ___TRADIOBUTTONH
#define ___TRADIOBUTTONH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_button_event.h>

class TRadioButton : public TButtonEvent
{
public:
    TRadioButton(TObject* hOwner,char *Caption,
		DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TRadioButton();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};
};

#define RADIOBUTTON(object) ((TRadioButton*)(object))

#endif


#endif //LIFE END
