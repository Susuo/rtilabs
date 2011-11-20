// DisplayWebBrowserView.cpp: DisplayWebBrowserView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayWebBrowserView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


DisplayWebBrowserView::DisplayWebBrowserView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ�r���[�̍쐬.
	WebBrowser.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER );
	//�e�̃Z�b�g.
	WebBrowser.setParent(this);
	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//�T�C�Y��ύX
	this->WindowResize();

}

//�E�B���h�E�����T�C�Y����
void DisplayWebBrowserView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	WebBrowser.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayWebBrowserView::~DisplayWebBrowserView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayWebBrowserView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( WebBrowser.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//�\�����ڂ̐ݒ�
void DisplayWebBrowserView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	WebBrowser.Navigate2( inFilename );
}

//�A�N�e�B���ɂȂ�܂���
void DisplayWebBrowserView::OnActive() const
{
	this->WindowResize();

	WebBrowser.Show();

	::SetFocus( WebBrowser.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayWebBrowserView::OnNoActive() const
{
	WebBrowser.Hide();
}


//�����炩��w�������t�@�C���ȊO�̃t�@�C�����I�[�v�����悤�Ƃ��ꂽ�ꍇ
//�u���E�U�̏ꍇ�A�ʂ̃����N�𓥂񂾏ꍇ�ł�
void DisplayWebBrowserView::OnQuickChange(string inPath)
{
	ASSERT(this->RfileOne);
	//�ǂ����A RfilePreviewOne �Ȃ̂ŏ��������Ă����Ȃ�.
	((RfilePreviewOne*)this->RfileOne)->Attach(inPath.c_str() );

	Win->OnQuickChange(inPath);
}


//�^�C�g�����ύX���ꂽ�ꍇ
void DisplayWebBrowserView::OnTitleChange()
{
	Win->OnTitleChange();
}
