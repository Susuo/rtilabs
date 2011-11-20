// RToolBarWin32.cpp: RToolBarWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RToolBarWin32.h"

#define FILER_TOOL_BACK				0
#define FILER_TOOL_NEXT				1
#define FILER_TOOL_UP				2
#define FILER_TOOL_UPDATE			3
#define FILER_TOOL_SEARCH			4
#define FILER_TOOL_SORTCHANGE		5
#define FILER_TOOL_DISPCHANGE		6
#define FILER_TOOL_SEP				10

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RToolBarWin32::RToolBarWin32(FilerSub* inFilerSub) : RToolBar(inFilerSub)
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//ToolBar���̂����̍쐬.
	Table.Create(mainSystem->getInstance(),
					  mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE,
					  this,
					  &RToolBarWin32::WindowProc);
	//Tool�o�[
	//���s������ RDoNotInitaliseException �𓊂��܂��B
	ToolBar.Create(mainSystem->getInstance(),
					  Table.getWindowHandle(), 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE);

	TBBUTTON tbs[] = {
		{ 0 , FILER_TOOL_BACK		 , TBSTATE_ENABLED, TBSTYLE_DROPDOWN , 0, 0} ,
		{ 1 , FILER_TOOL_NEXT		 , TBSTATE_ENABLED, TBSTYLE_DROPDOWN , 0, 0} ,
		{ 2 , FILER_TOOL_UP			 , TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0} ,
		{ -1, FILER_TOOL_SEP		 , TBSTATE_ENABLED, TBSTYLE_SEP , 0, 0} ,
		{ 3 , FILER_TOOL_UPDATE		 , TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0} ,
		{ 4 , FILER_TOOL_SEARCH		 , TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0} ,
		{ -1, FILER_TOOL_SEP		 , TBSTATE_ENABLED, TBSTYLE_SEP , 0, 0} ,
		{ 5 , FILER_TOOL_SORTCHANGE	 , TBSTATE_ENABLED, TBSTYLE_DROPDOWN , 0, 0} ,
		{ 6 , FILER_TOOL_DISPCHANGE	 , TBSTATE_ENABLED, TBSTYLE_DROPDOWN , 0, 0} 
	};
	ToolBar.CreateButton(mainSystem->getInstance(),"resource\\Toolbar.bmp" ,
		tbs, sizeof(tbs) / sizeof(tbs[0]) );

	//�t���b�g�c�[���o�[��  http://kumei.ne.jp/c_lang/sdk2/sdk_148.htm
	ToolBar.setStyle( ToolBar.getStyle() | TBSTYLE_FLAT);
	InvalidateRect( ToolBar.getWindowHandle(), NULL, TRUE); 
	//DropDown   http://kumei.ne.jp/c_lang/sdk2/sdk_189.htm
	ToolBar.setExtended( ToolBar.getExtended() | TBSTYLE_EX_DRAWDDARROWS);

	OnSize();
}

RToolBarWin32::~RToolBarWin32()
{
}

//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
void RToolBarWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	//�c�[���o�[
	Table.MoveWindow(itemRC.left ,itemRC.top + TOOL_BAR_START , itemRC.right ,itemRC.top + TOOL_BAR_SIZE);
	//�c�[���o�[�̃c�[���o�[ (w
	ToolBar.MoveWindow(0 , 0 , itemRC.right , TOOL_BAR_SIZE );
}

//�E�B���h�E�v���V�[�W��.
LRESULT RToolBarWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_COMMAND:
		{
			Window* win = FSub->getActiveWindow();
			switch(LOWORD(wparam))
			{
			case FILER_TOOL_BACK:
				if (win)	win->HistoryBack();
				break;
			case FILER_TOOL_NEXT:
				if (win)	win->HistoryNext();
				break;
			case FILER_TOOL_UP:
				if (win)	win->UpDirectory();
				break;
			case FILER_TOOL_UPDATE:
				break;
			case FILER_TOOL_SEARCH:
				break;
			case FILER_TOOL_SORTCHANGE:
				break;
			case FILER_TOOL_DISPCHANGE:
				break;
			}
		}		
		break;
	}
	return Table.CallWindowProc( hwnd,msg,wparam,lparam);
}
