// RTaskViewWin32.cpp: RTaskViewWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTaskViewWin32.h"
#include "WindowsMain.h"

#include "RDoNotInitaliseException.h"

#include "resource.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTaskViewWin32::RTaskViewWin32() throw()
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	this->WindowHandle =
		::CreateDialog(mainSystem->getInstance() , "TASKVIEW", mainSystem->getWindowHandle() , (DLGPROC)DlgProcStatic);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("�^�X�N�Ǘ��_�C�A���O�̍쐬�Ɏ��s ErrorCode:%ld",::GetLastError() );
	}
	//class �ɃR�[���o�b�N���邽�߂ɃT�u�N���X��
	::SetWindowLong( this->WindowHandle , GWL_USERDATA,  (long)this);

	TaskList.MountWindow( ::GetDlgItem(this->WindowHandle , TASKLIST) );

	RECT rc;
	TaskList.GetClientRect(&rc);

	TaskList.InsertColumn( 0 , 0 , "ID" , rc.right * 0.20 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	TaskList.InsertColumn( 1 , 0 , "�i��" , rc.right * 0.20 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	TaskList.InsertColumn( 2 , 0 , "���e" , rc.right * 0.60 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	//��s�I����������A Fixed ���h���b�O�ł�����ł���悤�ɂ��܂��B
	TaskList.setExtended( TaskList.getExtended() | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

	::ShowWindow( WindowHandle , SW_SHOW);

}

RTaskViewWin32::~RTaskViewWin32()
{
	::DestroyWindow( this->WindowHandle );
}

//�V�����^�X�N��ǉ����܂��B
void RTaskViewWin32::InsertTask(unsigned long inID,string inMessage)
{
	RAutoLock al(&Lock);

	int last = TaskList.GetItemCount();
	TaskList.SetItem( last,0 , inID);
	TaskList.SetItem( last,1 , 0);
	TaskList.SetItem( last,2 , inMessage.c_str());
}

//�i���̍X�V
void RTaskViewWin32::UpdateGoing(unsigned long inID,int inParsent)
{
	RAutoLock al(&Lock);

	//����.
	int subitem = TaskList.SearchItem(0,inID);
	if (subitem == -1) return ;	//�����A�ˁ[��

	//�ύX����!
	TaskList.SetItem(1 , subitem , inParsent);
}

//�^�X�N�̍폜
void RTaskViewWin32::DeleteTask(unsigned long inID)
{
	RAutoLock al(&Lock);

	//����.
	int subitem = TaskList.SearchItem(0,inID);
	if (subitem == -1) return ;	//�����A�ˁ[��

	//�폜���ߔ��s!
	TaskList.DeleteItem(subitem);
}


//�E�B���h�E�v���V�[�W��.
LRESULT RTaskViewWin32::DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return TRUE;
	}
	return FALSE;
}


//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂ΂��.
void RTaskViewWin32::OnSize() 
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	Filer * filer = Filer::getFiler();

	//�t�H�[�J�X�𒲂ׂ�.
	bool left = filer->IsLeftFocus();

	//���݂̃t�H�[�J�X�̔��Α��̃E�B���h�E�̃T�C�Y�̎擾.
	RECT MyClientRect,MyWindowRect,baseWindowRC;
	::GetClientRect( mainSystem->getBaseWindow( !left) , &baseWindowRC);
	::GetWindowRect( this->WindowHandle , &MyWindowRect);
	::GetClientRect( this->WindowHandle , &MyClientRect);

	//���̃E�B���h�E�̉����A���̃E�B���h�E�̉��̕ӂƓ����ɂȂ�悤�ɔz�u���܂��B

	int windowWidth = MyWindowRect.right - MyWindowRect.left;
	int windowHeigh = MyWindowRect.bottom - MyWindowRect.top;
	MoveWindow( this->WindowHandle ,
		baseWindowRC.left , baseWindowRC.bottom - windowHeigh ,
		windowWidth , windowHeigh , TRUE );
}

