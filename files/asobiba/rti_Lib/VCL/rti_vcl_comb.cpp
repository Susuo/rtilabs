/**********************************************************/
// �R���{�{�b�N�X�̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_comb.h>
/*
        �R���{�{�b�N�X�������s�R�̏ꍇ�́A
        �c���𑝂₵�Ă����Ă݂Ă�(^^;;;

        �R���{�{�b�N�X�̂΂����@(�{

*/


TComboBox::TComboBox(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"COMBOBOX");
    if (!CreateObject(hOwner,Caption,dwStyle | CBS_DROPDOWN | WS_VSCROLL,x,y,w,h,false,StaticProc) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_COMBOBOX;
}

TComboBox::~TComboBox()
{
	DestroyComponent();
}



/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TComboBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
