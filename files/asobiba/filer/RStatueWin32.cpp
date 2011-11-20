// RStatueWin32.cpp: RStatueWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "comm.h"
#include "RStatueWin32.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStatueWin32::RStatueWin32(FilerSub* inFilerSub) : RStatue(inFilerSub)
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//ToolBar���̂����̍쐬.
	Table.Create(mainSystem->getInstance(),
					  mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE,
					  this,
					  &RStatueWin32::WindowProc);

	//�^�u�R���g���[�����쐬���܂��B
	//���s������ RDoNotInitaliseException �𓊂��܂��B
	StatueControl.Create(mainSystem->getInstance(),
					  Table.getWindowHandle(), 
				      mainSystem->getID() ,
					  WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);

	//�v���O���X�o�[�����܂��B
	ProgressBar.Create( mainSystem->getInstance() , 
						mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
						mainSystem->getID() , 
						WS_VISIBLE);
	ProgressBar.setRange(0,100);
	ProgressBar.setGradationRange(RGB(0,0,255) , RGB(0,255,0) );
	ProgressBar.setPos(0);

	//�X�e�[�^�X�o�[��e�Ƃ��܂��B
	SetParent( ProgressBar.getWindowHandle() , StatueControl.getWindowHandle() );

	OnSize();
}

RStatueWin32::~RStatueWin32()
{

}

//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
void RStatueWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	Table.MoveWindow(itemRC.left , itemRC.bottom - STATUS_BAR_SIZE , itemRC.right , STATUS_BAR_SIZE);
	StatueControl.MoveWindow(0 , 0 , itemRC.right , STATUS_BAR_SIZE );

	//�܂��߂̍쐬
	int sbSize[3];
	sbSize[0] = itemRC.right * 0.70 ;
	sbSize[1] = itemRC.right * 0.20 ;
	sbSize[2] = -1 ;

	SendMessage(StatueControl.getWindowHandle() , SB_SETPARTS, (WPARAM)3, (LPARAM)sbSize);
	ShowWindow(StatueControl.getWindowHandle(), SW_SHOW); 

	//�v���O���X�o�[���ړ�.
	ProgressBar.MoveWindow( sbSize[0] , 0 , sbSize[1] , STATUS_BAR_SIZE );
}


//�E�B���h�E�v���V�[�W��.
LRESULT RStatueWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;
			switch(nmhdr->code)
			{
			case NM_RCLICK:		//�E�N���b�N����܂���.
				break;
			}
		}
		break;
	}
	return Table.CallWindowProc( hwnd,msg,wparam,lparam);
}


//�X�e�[�^�X�̃Z�b�g.
void RStatueWin32::setStatue(string inStatue)
{
	SendMessage(StatueControl.getWindowHandle(), SB_SETTEXT, (WPARAM)0 | 0, (LPARAM)(LPSTR)inStatue.c_str() );
}

//�v���O���X�o�[
void RStatueWin32::setProgressBar(int inParsent)
{
	//�␳
	if (inParsent > 100) inParsent = 100;
	else if (inParsent < 0)	 inParsent = 0;

	//�ʒu�\��.
	ProgressBar.setPos(inParsent);
}
