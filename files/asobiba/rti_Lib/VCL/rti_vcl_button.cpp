/**********************************************************/
// �{�^��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef BUTTON_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_button.h>

TButton::TButton(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"BUTTON");
	//�{�^�������
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);


    Kind            = OBJECT_KIND_BUTTON;
}

TButton::~TButton()
{
	DestroyComponent();
}


/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TButton::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
