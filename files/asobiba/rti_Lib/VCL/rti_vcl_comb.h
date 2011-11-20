#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#ifndef ___TCOMBOBOXH
#define ___TCOMBOBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_comb_event.h>
//#include "rti_vcl_object.h"

class TComboBox : public TCombEvent
{
public:
    TComboBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TComboBox();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};

};

#define COMBOBOX(object) ((TComboBox*)(object))

#endif


#endif //LIFE END
