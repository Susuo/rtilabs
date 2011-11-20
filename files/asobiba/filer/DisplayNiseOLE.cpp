// DisplayNiseOLE.cpp: DisplayNiseOLE �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayNiseOLE.h"

DisplayNiseOLE::DisplayNiseOLE
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//�T�C�Y��ύX
	this->WindowResize();

}

//�E�B���h�E�����T�C�Y����
void DisplayNiseOLE::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	NiseOLE.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);

}

DisplayNiseOLE::~DisplayNiseOLE()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayNiseOLE::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( NiseOLE.getWindowHandle() ) )		WindowResize();
		break;
	case WM_PAINT:
	case WM_ERASEBKGND:
	case WM_SETFOCUS:
	case WM_SHOWWINDOW:
		//PostMessage(NiseOLE.getWindowHandle() , msg , wparam ,lparam);
		::InvalidateRect(NiseOLE.getWindowHandle()  , NULL , FALSE);
		break;

//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}


//�\�����ڂ̐ݒ�
void DisplayNiseOLE::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	const string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);
	NiseOLE.Exec(mainSystem->getInstance(),	mainSystem->getBaseWindow( Win->getIsLeft() ),inFilename);

	return ;
}

//�A�N�e�B���ɂȂ�܂���
void DisplayNiseOLE::OnActive() const
{
	this->WindowResize();

	NiseOLE.Show();

	::SetFocus( NiseOLE.getWindowHandle() );
	//�ĕ`��
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	::InvalidateRect(mainSystem->getWindowHandle() , NULL ,TRUE);
}

//��A�N�e�B���ɂȂ�܂���
void DisplayNiseOLE::OnNoActive() const
{
	NiseOLE.Hide();
}

