/**********************************************************/
//�X�e�[�^�X�o�[�̓y��
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef STATUS_LIFE

#include <.\VCL\rti_vcl_status.h>

TStatusBar::TStatusBar(TObject* hOwner,char *Caption,DWORD dwStyle,
					   int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"STATUSCLASSNAME");
	//�X�e�[�^�X�o�[
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_STATUS;
}

TStatusBar::~TStatusBar()
{
	DestroyComponent();
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TStatusBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
