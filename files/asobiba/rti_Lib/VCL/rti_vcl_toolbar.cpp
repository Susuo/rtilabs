/**********************************************************/
// �^�u�R���g���[���̓y��
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef TOOLBAR_LIFE

#include <.\VCL\rti_vcl_toolbar.h>

TToolBar::TToolBar(TObject* hOwner,char *Caption,
				 DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,TOOLBARCLASSNAME);
	//�c�[���o�[�R���g���[��
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //�쐬���s
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //�����̃I�[�i�[�Ɏ������쐬���ꂽ���Ƃ�`����
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_TOOLBAR;
}

TToolBar::~TToolBar()
{
	DestroyComponent();
}
/**********************************************************/
/*
    �ǉ�
*/
/**********************************************************/
int TToolBar::Add(int iBitmap,BYTE fsState,BYTE fsStyle,DWORD dwData,
                   int iString,SimpleEvent CallRtn)
{
    TBBUTTON tbb;
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE,
        (WPARAM)sizeof(TBBUTTON), 0);
    tbb.iBitmap   = iBitmap; //�{�^���C���[�W�̔ԍ��@�O����n�܂�
    tbb.idCommand = Application->GetId(); // WM_COMMAND�̎���ID 
    tbb.fsState   = fsState; // �{�^���̏�ԁ@�g�p�\���Ȃ� 
    tbb.fsStyle   = fsStyle; // �{�^���X�^�C��
    tbb.dwData    = dwData; // �A�v���P�[�V������`�f�[�^ 
    tbb.iString   = iString; // �{�^���X�g�����O�@�O����n�܂�

    struct _ToolBarUsingStruct *tpuf;

    //�e��ݒ�
    tpuf = VToolBarUsing.at(ButtonCounter) ;
    tbb.iBitmap += tpuf->UsingBmpNo;
    tpuf->CallRtn = CallRtn;
    tpuf->idCommand = tbb.idCommand;

    //�{�^���𑝂₷
    ButtonCounter++;

    return SendMessage(hWnd, TB_ADDBUTTONS, (WPARAM)1, 
           (LPARAM)&tbb);
}

/**********************************************************/
/*
 �r�b�g�}�b�v�ǉ� �ŏ���NULL �������� �t�@�C������
    �ŏ��� NULL �ł͂Ȃ�������A�t�@�C������ NULL  
*/
/**********************************************************/
int TToolBar::AddBitMap(UINT nID,char *filename,int pace)
{
    TBADDBITMAP tbab;
    HINSTANCE   hInit;
    UINT nSetID;
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE,
        (WPARAM)sizeof(TBBUTTON), 0);
    if (nID == USER_DEFINED)
    {
        //�t�@�C�����[�h
        hInit  = NULL;
        nSetID = (int) LoadImage(Application->GethInstance(),filename,IMAGE_BITMAP,
    	               0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������
    }
    else
    {
        hInit  = HINST_COMMCTRL;
        nSetID = nID;
    }

    tbab.hInst =    hInit;
    tbab.nID   =    nSetID;
    //BITMAP�o�^
    int UsingNo = 
        SendMessage(hWnd, TB_ADDBITMAP, pace, (LPARAM)&tbab);

    struct _ToolBarUsingStruct *tpuf;
    for(int i = 0; i < pace;i ++)
    {
        tpuf = new _ToolBarUsingStruct();
        tpuf->UsingBmpNo = UsingNo;
        //�Ō���ɒǉ����Ă���.
        VToolBarUsing.push_back( tpuf );

    }
    return UsingNo;
}

/**********************************************************/
/* ��{���[�`�� */
/**********************************************************/
LRESULT TToolBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}
#endif //LIFE END
