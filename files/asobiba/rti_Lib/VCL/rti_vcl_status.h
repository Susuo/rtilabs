#include "StartWithExeFile.h"

#ifdef STATUS_LIFE

#ifndef ___TSTATUSHXX
#define ___TSTATUSHXX
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_status_event.h>

class TStatusBar : public TStatusEvent
{
public:
    TStatusBar(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TStatusBar();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};
};

#endif

#endif   // LIFE END
