#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#ifndef ___TEDITH
#define ___TEDITH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_edit_event.h>

class TEdit : public TEditEvent
{
public:
    TEdit(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TEdit();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};
};


#endif


#endif //LIFE END
