// DisplayMediaView.cpp: DisplayMediaView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayMediaView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



DisplayMediaView::DisplayMediaView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ�r���[�̍쐬.
	Media.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER );

	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//�T�C�Y��ύX
	this->WindowResize();

}

//�E�B���h�E�����T�C�Y����
void DisplayMediaView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	Media.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayMediaView::~DisplayMediaView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayMediaView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( Media.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//�\�����ڂ̐ݒ�
void DisplayMediaView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	Media.Load( inFilename , mainSystem->getInstance() );
}

//�A�N�e�B���ɂȂ�܂���
void DisplayMediaView::OnActive() const
{
	this->WindowResize();

	Media.Show();

	::SetFocus( Media.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayMediaView::OnNoActive() const
{
	Media.Hide();
}

