/**********************************************************/
// ���W�I�{�^���̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef RADIOBUTTON_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_radiobutton.h>

TRadioButton::TRadioButton(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"BUTTON");
	//�{�^�������
    if ( !CreateObject(hOwner,Caption,dwStyle | BS_AUTORADIOBUTTON,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_RADIOBUTTON;

}

TRadioButton::~TRadioButton()
{
	DestroyComponent();
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TRadioButton::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
