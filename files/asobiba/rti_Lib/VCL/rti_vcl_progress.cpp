/**********************************************************/
// �v���O�����X�o�[�̓y��
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef PROGRESS_LIFE

#include <.\VCL\rti_vcl_progress.h>

TProgress::TProgress(TObject* hOwner,char *Caption,DWORD dwStyle,
					 int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"PROGRESS_CLASS");
    if (! CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_PROGRESS;
}

TProgress::~TProgress()
{
	DestroyComponent();
}

void TProgress::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	}
}


/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TProgress::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif  //LIFE END
