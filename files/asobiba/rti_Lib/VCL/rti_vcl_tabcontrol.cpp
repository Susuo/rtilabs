/**********************************************************/
// �^�u�R���g���[���̓y��
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef TABCOONTROL_LIFE
#include <.\VCL\rti_vcl_tabcontrol.h>

TTabControl::TTabControl(TObject* hOwner,char *Caption,
				 DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,WC_TABCONTROL);
	//�^�u�R���g���[��
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_TABCONTROL;
}

TTabControl::~TTabControl()
{
	DestroyComponent();
}

int TTabControl::Add(unsigned int mask,char* text)
{
    static int page = -1;
    TC_ITEM tcItem;

    page++;
    tcItem.mask = mask;
    tcItem.pszText = text;
    SendMessage(this->hWnd, TCM_INSERTITEM, (WPARAM)page, (LPARAM)&tcItem);

    return page;
}
/**********************************************************/
// �R���g���[�����w�肵���y�[�W�Ɗ֘A�t���܂�
/**********************************************************/
bool TTabControl::ControlReAllocPage(TObject* Obj,int page)
{
	if(Obj->GethOwer() != this)
	{ //�������I�[�i�[�Ƃ��č쐬���Ă��Ȃ��R���|�[�l���g�͒m���
		return false;
	}
	//�I�[�i�[���g�p����̈���g�p�����Ă��炤.
	Obj->SetOwerUse(page); //���̃y�[�W�Ŏg���񂾂҂��
	                          // 0 �͖����Ȃ̂� + 1 ����.
	return true;
}

/**********************************************************/
// �R���g���[�����w�肵���y�[�W��\�����܂�
/**********************************************************/
void TTabControl::ControlSetingPage()
{
    TBaseObject* TheObject;

    ObjectIT it;
    for(it=GetComponentFarst(); it != GetComponentEnd() ; it++)
    {
		TheObject = (TBaseObject*) (*it);
		if (TheObject->GetOwerUse()==GetPage() ||
			           TheObject->GetOwerUse()==ALL_PAGE)
		{ //���ݕ\������Ă���y�[�W
            TheObject->Show();
            TheObject->Enabled();
		}
		else
		{
            TheObject->Hide();
            TheObject->UnEnabled();
		}
    }
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TTabControl::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    case WM_PAINT:
		//�R���g���[����L���A�����ɂ���
    	ControlSetingPage();
        break;
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}
#endif //LIFE END
