// RTabBarWin32.cpp: RTabBarWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTabBarWin32.h"
#include "WindowsMain.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTabBarWin32::RTabBarWin32(FilerSub* inFilerSub) : RTabBar(inFilerSub)
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//ToolBar���̂����̍쐬.
	Table.Create(mainSystem->getInstance(),
					  mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE,
					  this,
					  &RTabBarWin32::WindowProc);

	//�^�u�R���g���[�����쐬���܂��B
	//���s������ RDoNotInitaliseException �𓊂��܂��B
	TabControl.Create(mainSystem->getInstance(),
					  Table.getWindowHandle(), 
				      mainSystem->getID() ,
					  WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);
	OnSize();
}

RTabBarWin32::~RTabBarWin32()
{

}

//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
void RTabBarWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	Table.MoveWindow(itemRC.left , itemRC.top + TAB_BAR_START , itemRC.right , itemRC.top + TAB_BAR_SIZE );
	TabControl.MoveWindow(0 , 0 , itemRC.right , TAB_BAR_SIZE );
}

//�^�u�̒ǉ�
void RTabBarWin32::InertTab(const Window* inWindow) 
{
	//�^�u���̎擾.
	int last = TabControl.GetTabCount();

	TabControl.InsertTab(last , "loading..." , (LPARAM)inWindow);
	TabControl.SelectTab(last);
}

//�^�u�̍폜
void RTabBarWin32::DeleteTab(const Window* inWindow) 
{
	//�^�u���̎擾.
	int last = TabControl.GetTabCount();

	//inWindow�̃^�u������...
	for(int i = 0; i < last ; i++)
	{
		if ( (TabControl.GetTabParam(i)) == (LPARAM)inWindow)
		{
			//�������̂ō폜
			TabControl.DeleteTab(i);
			return ;
		}
	}
}

//�^�u�̕ύX
void RTabBarWin32::UpdateTab(const Window* inWindow) 
{
	//�^�u���̎擾.
	int last = TabControl.GetTabCount();

	//inWindow�̃^�u������...
	for(int i = 0; i < last ; i++)
	{
		if ( (TabControl.GetTabParam(i)) == (LPARAM)inWindow)
		{
			//������.
			TabControl.UpdateTab(i, inWindow->getTitle().c_str() , (LPARAM)inWindow);
			::UpdateWindow( TabControl.getWindowHandle() );
			return;
		}
	}
}

//�E�B���h�E�v���V�[�W��.
LRESULT RTabBarWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;
			switch(nmhdr->code)
			{
			case TCN_SELCHANGE:	//�^�u���ύX���ꂽ�Ƃ�.
				{
					this->OnSelChange();
					return 0;
				}
				break;
			case NM_RCLICK:		//�E�N���b�N����܂���.
				if (this->OnRClick((LPNMHDR)lparam) )
				{
					return 0;
				}
				break;
			}
		}
		break;
	}
	return Table.CallWindowProc( hwnd,msg,wparam,lparam);
}

//�I������Ă���^�u���ω����܂����B
void RTabBarWin32::OnSelChange()
{
	//Window�̎擾.
	Window* win = getCurrentWindow();
	if (win == NULL) 	return ;

	//�A�N�e�B���ɂ���.
	bool ret = FSub->setActiveWindow( win );
	if (ret)
	{
		TRACE("Tab���I�����ꂽ�̂� %p ���A�N�e�B���ɂ��܂���\n",win);
	}
}


//�E�N���b�N
bool RTabBarWin32::OnRClick(const NMHDR * inNmhdr) const
{
	//Window�̎擾.
	Window* win = getCurrentWindow();
	if (win == NULL) 	return false;


	try
	{
		//�^�[�Q�b�g�ݒ�
		FSub->setTargetWindow(win);

		//���̃E�B���h�E�̃|�b�v�A�b�v���j���[��\��.
		win->PopupMenu("TabPopup");

		//�^�[�Q�b�g�N���A
		FSub->setTargetWindow(NULL);
	}
	catch(RException e)
	{
		printf("�G���[���������܂���:\n%s",e.getMessage() );
		return false;
	}

	return true;	
}

//�J�����g�̃E�B���h�E�̎擾.
Window* RTabBarWin32::getCurrentWindow() const
{
	int index = TabControl.GetCurSel();
	if (index < 0)
	{
		TRACE("���̃^�O���A�N�e�B���ɂȂ�܂����@�����G���[\n");
		return NULL;
	}

	//Window�̎擾.
	Window* win = (Window*)TabControl.GetTabParam(index);
	if (win == NULL) 
	{
		TRACE("NULL�̃^�O���A�N�e�B���ɂȂ�܂����@�����G���[\n");
		return NULL;
	}
	return win;
}
