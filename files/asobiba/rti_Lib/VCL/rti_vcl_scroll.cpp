/**********************************************************/
// �X�N���[���o�[�̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef SCROLLBAR_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_scroll.h>

TScrollBar::TScrollBar(TObject* hOwner,
	DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"SCROLLBAR");
	//�X�N���[���o�[�����
    if ( !CreateObject(hOwner,NULL,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_SCROLL;

}

TScrollBar::~TScrollBar()
{
	DestroyComponent();
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TScrollBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
