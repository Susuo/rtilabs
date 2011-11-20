// DisplayBitmapView.cpp: DisplayBitmapView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayBitmapView.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DisplayBitmapView::DisplayBitmapView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ�r���[�̍쐬.
	ImageViewer.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER );

	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//�T�C�Y��ύX
	this->WindowResize();

}

//�E�B���h�E�����T�C�Y����
void DisplayBitmapView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	ImageViewer.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayBitmapView::~DisplayBitmapView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayBitmapView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( ImageViewer.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//�\�����ڂ̐ݒ�
void DisplayBitmapView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	const string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);
	ImageViewer.Load(inFilename);

	return ;
}

//�A�N�e�B���ɂȂ�܂���
void DisplayBitmapView::OnActive() const
{
	this->WindowResize();

	ImageViewer.Show();

	::SetFocus( ImageViewer.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayBitmapView::OnNoActive() const
{
	ImageViewer.Hide();
}


