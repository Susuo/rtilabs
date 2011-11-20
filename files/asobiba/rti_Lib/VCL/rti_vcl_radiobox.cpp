/**********************************************************/
// �`�F�b�N�{�b�N�X
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef RADIOBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_radiobox.h>
#include <.\VCL\rti_vcl_button.h>

TRadioBox::TRadioBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
    :TGroupBox(hOwner,Caption,dwStyle,x,y,w,h,UserWndp)
{
	ButtonPossion = 0;
    OnChange      = NULL;
}

TRadioBox::~TRadioBox()
{
} 


TRadioButton* TRadioBox::Add(char *Caption,DWORD dwStyle)
{
	RECT rect;
	SIZE s;
	HDC hdc ;
	int width;
	GetWindowRect( GethWnd(),&rect);

    hdc = GetDC(hWnd);
	GetTextExtentPoint32(hdc, Caption, lstrlen(Caption), &s);
    ReleaseDC( hWnd,hdc);
	width = rect.right - rect.left ;
	
	//�����Ȃ��������� (^^;;;;
	return (new TRadioButton(this,
		Caption,dwStyle,
		10,ButtonPossion+= s.cy,  width-20,  s.cy) );
} 

/**********************************************************/
//�A���z�u���K�v�ȃR���|�[�l���g�̃R���g���[���̍Ĕz�u�𑣂�
/**********************************************************/
void TRadioBox::RefControl()
{
	RECT rect;
	SIZE s;
	HDC hdc ;
	int width;
    ObjectIT it;
    char Caption[MAX_PATH];
    TBaseObject * TBO;

    ButtonPossion = 0; //�{�^���̈ʒu�̏�����

    GetWindowRect( hWnd,&rect);
    hdc = GetDC(hWnd);

    width = rect.right - rect.left ;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            TBO = ((TBaseObject*)(*it));
            if ( TBO->GetKind() == OBJECT_KIND_RADIOBUTTON)
            { 
                TBO->GetCaption(Caption,MAX_PATH);
            	GetTextExtentPoint32(hdc, Caption , lstrlen(Caption), &s);
                TBO->Move(10,ButtonPossion+= s.cy);
            }
        }
    ReleaseDC( hWnd,hdc);
}

unsigned long TRadioBox::GetIndexMain(bool ByteReturn)
{
    unsigned long ret = 0;
    unsigned long counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_RADIOBUTTON)
            {
                //�`�F�b�N�{�^���݂���.
                if (((TRadioButton*)(*it))->Get()
		    										== BST_CHECKED)
                {//�`�F�b�N����Ă���.
                    ret = ret | (0x00000001 << counter);
                    break;  //���W�I�{�b�N�X�͔r�������炱��ł���.                 
                }
                counter++;  //���W�I�{�^���J�E���g�A�b�v���܂�
            }
        }

    if (ByteReturn)   return ret; //�r�b�g��Ԃ�
    else              return  counter;//���ݑI�����Ă���Ƃ���𐔒l�Ԃ�
}


/**********************************************************/
//�{�^�����`�F�b�N���܂�
/**********************************************************/
void TRadioBox::SetIndex(unsigned long bit)
{
    unsigned long ret = 0;
    int counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_RADIOBUTTON)
            {
                //�`�F�b�N�{�^���݂���.
                if (bit & (0x00000001 << counter) )
                { //�����̃{�^���̓`�F�b�N����
                    RADIOBUTTON((*it))->Set();
                }
                else
                { //�{�^���̃`�F�b�N�����낷
                    RADIOBUTTON((*it))->ReSet();
                }
                counter++;  //�`�F�b�N�{�^���J�E���g�A�b�v���܂�
            }
        }
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TRadioBox::UserDisPacth(HWND hwnd , UINT message,
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
