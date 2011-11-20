// FilerSub.cpp: FilerSub �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FilerSub.h"
#include "RAddressBarWin32.h"
#include "RTabBarWin32.h"
#include "RToolBarWin32.h"
#include "RStatueWin32.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

FilerSub::FilerSub(bool inIsLeft)
{
	IsLeft = inIsLeft;

	AddressBar = new RAddressBarWin32(this);
	TabBar = new RTabBarWin32(this);
	ToolBar = new RToolBarWin32(this);
	StatueBar = new RStatueWin32(this);

	ActiveWindow = NULL;
	TargetWindow = NULL;
}

FilerSub::~FilerSub()
{
	delete AddressBar;
	delete TabBar;
	delete ToolBar;
	delete StatueBar;

	for(TAB_WINDOWS::iterator p = Wins.begin(); p != Wins.end() ; p++)
	{
		delete (*p);
	}
	Wins.clear();
}

//�V�����E�B���h�E���J��
Window* FilerSub::NewWindow(string inPath)
{
	Window* win = new Window( this , inPath);
	Wins.push_back( win );
	TabBar->InertTab(win);

	if (ActiveWindow == NULL) ActiveWindow = win;

	return win;
}

//�w�肵���E�B���h�E���A�N�e�B���ɂ���.
//TAB����ĂԂ���!
bool FilerSub::setActiveWindow(Window* inWin)
{
	//�����̊Ǘ���?
	if ( !Search(inWin) ) return false;

	//���łɃA�N�e�B��.
	if (ActiveWindow != inWin)
	{
		//��A�N�e�B����ʒm
		if (ActiveWindow) ActiveWindow->OnNoActive();
		//�ݒ�.
		ActiveWindow = inWin;
	}
	//�A�N�e�B����ʒm.
	ActiveWindow->OnActive();

	//�A�h���o�[��ύX
	AddressBar->OnAddressChange( inWin->getNowPath() );
	//�^�u�̍X�V�������B
	TabBar->UpdateTab(inWin);

	return true;
}

//�w�肵���E�B���h�E���^�[�Q�b�g�ɂ���
bool FilerSub::setTargetWindow(Window* inWin)
{
	if (inWin != NULL )
	{
		//�����̊Ǘ���?
		if ( !Search(inWin) ) return false;
	}

	TargetWindow = inWin;
	return true;
}



//�A�N�e�B���E�B���h�E�ł���?
bool FilerSub::IsActiveWindow(const Window* inWin)
{
	return ActiveWindow == inWin;
}

//�E�B���h�E�����T�C�Y���ꂽ�Ƃ��ɌĂ΂��
void FilerSub::OnWindowResize()
{
	AddressBar->OnSize();
	TabBar->OnSize();
	ToolBar->OnSize();
	StatueBar->OnSize();
}

//����
bool FilerSub::Search(const Window* inWin) const
{
	return  (find(Wins.begin() , Wins.end() , inWin) != Wins.end());
}


//OnQuickChange �����������Ƃ��ɌĂ΂��
void FilerSub::OnQuickChange()
{
	ASSERT(ActiveWindow);

	//�A�h���o�[��ύX
	AddressBar->OnAddressChange( ActiveWindow->getNowPath() );
	//�^�u�̍X�V�������B
	TabBar->UpdateTab( ActiveWindow );
}

//�^�C�g�����ύX���ꂽ�ꍇ
void FilerSub::OnTitleChange()
{
	ASSERT(ActiveWindow);

	//�^�u�̍X�V�������B
	TabBar->UpdateTab( ActiveWindow );
}

//�E�B���h�E�����
bool FilerSub::CloseWindow(Window* inTargetWindow)
{
	//�����̊Ǘ����ł͂Ȃ���Ζv.
	TAB_WINDOWS::iterator p = find(Wins.begin() , Wins.end() , inTargetWindow);
	if (p == Wins.end() ) return false;

	//�������A�N�e�B����������A�ׂ̃��c���A�N�e�B���ɂ���.
	//�������鏇�Ԃ� �E -> ��
	if (ActiveWindow == inTargetWindow)
	{
		if (Wins.size() > 1)
		{//�ق��Ƀ^�u������͂�����
			TAB_WINDOWS::iterator next = (p + 1);
			if (next == Wins.end())
			{
				//���͎�������ԍŌゾ�����ꍇ�A���Ɍ���.
				next = (p - 1);
			}
			ActiveWindow = NULL;	//�����͂���������̂Ŗ����ɂ��Ă���.
			setActiveWindow( *next );
		}
		else
		{//�B��̃^�u�̏ꍇ
			ActiveWindow = NULL;
		}
	}

	//�폜.
	delete *p;
	Wins.erase( p );

	TabBar->DeleteTab(inTargetWindow);

	return true;
}

