#include "StartWithExeFile.h"

#ifdef STATICTEXT_LIFE

#ifndef ___TSTATICTEXTHXX
#define ___TSTATICTEXTHXX
#include <.\VCL\rti_vcl_static_event.h>
#include <.\VCL\rti_vcl_base.h>

class TStaticText : public TStaticEvent
{
public:
    TStaticText(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TStaticText();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};
};

#endif

#endif   // LIFE END
