#include "StartWithExeFile.h"
#ifdef RADIOBOX_LIFE

#ifndef TRADIOBOXH
#define TRADIOBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_groupbox.h>
#include <.\VCL\rti_vcl_radiobutton.h>
#include <.\VCL\rti_vcl_button_event.h>


class TRadioBox : public TGroupBox
{
private:
	int ButtonPossion;
    unsigned long GetIndexMain(bool ByteReturn);
public:
    TRadioBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TRadioBox();
	TRadioButton* Add(char *Caption,DWORD dwStyle);
    //�A���z�u���K�v�ȃR���|�[�l���g�̃R���g���[���̍Ĕz�u�𑣂�
    virtual void RefControl();
    //VCL�ɂ��T�u�N���X���Ŏ��s�����v���V�[�W��
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�I��������
    virtual void UserDestroy(){delete this;};

    int GetIndexInt(){return (int) GetIndexMain(false);};
    unsigned long GetIndex(){return GetIndexMain(true);};
    void SetIndex(unsigned long bit);

    void (*OnChange)(TObject *my ,TObject *you);
};

#endif


#endif //LIFE END
