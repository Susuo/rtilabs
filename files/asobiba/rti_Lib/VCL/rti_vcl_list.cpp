/**********************************************************/
// ���X�g�{�b�N�X�̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_list.h>

TListBox::TListBox(TObject* hOwner,char *Caption,
				   DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"LISTBOX");
	//���X�g�{�b�N�X�����
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_LISTBOX;
}

TListBox::~TListBox()
{
	DestroyComponent();
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TListBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}


#endif //LIFE END
