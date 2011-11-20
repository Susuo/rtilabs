// DisplayTextView.cpp: DisplayTextView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayTextView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DisplayTextView::DisplayTextView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ郊�X�g�r���[�̍쐬.
	RichEdit.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL);

	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//�T�C�Y��ύX
	this->WindowResize();

}

//�E�B���h�E�����T�C�Y����
void DisplayTextView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	RichEdit.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayTextView::~DisplayTextView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayTextView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( RichEdit.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//�\�����ڂ̐ݒ�
void DisplayTextView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	try
	{
		RichEdit.ReadFile( inFilename );
	}
	catch(RIOCannotOpenException e)
	{
		printf("Error: %s\n" , e.getMessage() );
	}
}

//�A�N�e�B���ɂȂ�܂���
void DisplayTextView::OnActive() const
{
	this->WindowResize();

	RichEdit.Show();

	::SetFocus( RichEdit.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayTextView::OnNoActive() const
{
	RichEdit.Hide();
}

