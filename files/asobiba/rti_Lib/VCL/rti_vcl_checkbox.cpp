/**********************************************************/
// �`�F�b�N�{�b�N�X
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef CHECKBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_checkbox.h>
#include <.\VCL\rti_vcl_button.h>

TCheckBox::TCheckBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
    :TGroupBox(hOwner,Caption,dwStyle,x,y,w,h,UserWndp)
{
	ButtonPossion = 0;
    OnChange      = NULL;
}

TCheckBox::~TCheckBox()
{
} 


TCheckButton* TCheckBox::Add(char *Caption,DWORD dwStyle)
{
	RECT rect;
	SIZE s;
	HDC hdc ;
	int width;
	GetWindowRect( GethWnd(),&rect);

    hdc = GetDC( GethWnd() );
	GetTextExtentPoint32(hdc, Caption, 
		lstrlen(Caption), &s);
    ReleaseDC( GethWnd(),hdc);
	width = rect.right - rect.left ;
	
	//�����Ȃ��������� (^^;;;;
	return (new TCheckButton(this,
		Caption,dwStyle,
		10,ButtonPossion+= s.cy,  width-20,  s.cy) );
} 

//�`�F�b�N����Ă���Ƃ����Ԃ��܂�
//�m�[�`�F�b�N�� 0 �ł�
unsigned long TCheckBox::GetIndex()
{
    unsigned long ret = 0;
    int counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_CHECKBUTTON)
            {
                //�`�F�b�N�{�^���݂���.
                if (((TCheckButton*)(*it))->Get()
		    										== BST_CHECKED)
                {//�`�F�b�N����Ă���.
                    ret = ret | (0x00000001 << counter);
                }
                counter++;  //���W�I�{�^���J�E���g�A�b�v���܂�
            }
        }
    return ret;
}

//�{�^�����`�F�b�N���܂�
void TCheckBox::SetIndex(unsigned long bit)
{
    unsigned long ret = 0;
    int counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_CHECKBUTTON)
            {
                //�`�F�b�N�{�^���݂���.
                if (bit & (0x00000001 << counter) )
                { //�����̃{�^���̓`�F�b�N����
                    CHECKBUTTON((*it))->Set();
                }
                else
                { //�{�^���̃`�F�b�N�����낷
                    CHECKBUTTON((*it))->ReSet();
                }
                counter++;  //�`�F�b�N�{�^���J�E���g�A�b�v���܂�
            }
        }
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TCheckBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    TObject *Obj;
    switch(message)
    {
	case WM_COMMAND:
       if (OnChange) 
       {
           Obj = ((TObject*)GetWindowLong((HWND)lParam, GWL_USERDATA));
           if (!Obj) break;
           OnChange(this,Obj);
       }
       break;
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}


#endif //LIFE END
