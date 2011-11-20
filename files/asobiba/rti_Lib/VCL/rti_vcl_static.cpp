/**********************************************************/
//�X�^�e�B�b�N�e�L�X�g
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef STATICTEXT_LIFE

#include <.\VCL\rti_vcl_static.h>

TStaticText::TStaticText(TObject* hOwner,char *Caption,
						 DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"STATIC");
	//�X�e�[�^�X�o�[
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_STATICTEXT;
}

TStaticText::~TStaticText()
{
	DestroyComponent();
}


/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TStaticText::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
