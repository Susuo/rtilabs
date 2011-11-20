// DisplayListViewBigIconWin32.cpp: DisplayListViewBigIconWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayListViewBigIconWin32.h"
#include "RFunctionScript.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


DisplayListViewBigIconWin32::DisplayListViewBigIconWin32
(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{
	ASSERT(Win != NULL);

	//Windows�̂��߂̃��C��.
	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ郊�X�g�r���[�̍쐬.
	ListView.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_TABSTOP | LVS_ICON | LVS_OWNERDATA | WS_VISIBLE );

	//�R�[���o�b�N�o�^
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	//�ݒ��ǂݍ���.
	SetupLoad.Load(FILER_DEFAULT_DEFINE_FILENAME,"FileView");

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;

	//�^���Ɉړ�����.
	this->HarfWindow();
}


//�E�B���h�E�𔼕��̈ʒu�Ɉړ�����.
void DisplayListViewBigIconWin32::HarfWindow() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	ListView.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayListViewBigIconWin32::~DisplayListViewBigIconWin32()
{
	//�֘A�t�����Ă���C���[�W���X�g������ɔj�����₪��̂�
	//�����ȃ��X�g�ɖ���ւ��āA����ɂ͔j������Ȃ��悤�ɂ��܂��B
	//�C���[�W���X�g�͋��L�̂��̂������Ă��邩��ˁ[
	//�j�������Ⴄ�Ƃ�����̃E�B���h�E�ł������Ȃ�C���[�W���X�g���G���[�ő�ςȂ��ƂɂȂ�B
	//���̃f�o�b�N��2���Ԃ�����������Ȃ����v���v��.
	ListView.setImageList(NULL , LVSIL_NORMAL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayListViewBigIconWin32::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( ListView.getWindowHandle() ) )		HarfWindow();
		break;
	case WM_NOTIFY:
		{
			if (!IsWindowVisible( ListView.getWindowHandle() ) ) return false;

			LPNMHDR nmhdr = (LPNMHDR)lparam;
			if (nmhdr->hwndFrom != ListView.getWindowHandle() )	return false;

			switch(nmhdr->code)
			{
			case LVN_GETDISPINFO:
				{
					if (this->OnGetDispInfo((NMLVDISPINFO*) lparam) )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case NM_DBLCLK:	//�_�u���N���b�N���ꂽ�Ƃ�
				{
					if (this->OnDoubleClick() )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case NM_RCLICK:
				{
					if (this->OnRClick((LPNMHDR)lparam) )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case LVN_COLUMNCLICK:	//�J�������N���b�N���ꂽ�Ƃ�
				{
					if (this->OnColumnClick((LPDRAWITEMSTRUCT) lparam ) )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case LVN_KEYDOWN:
				{
					if (OnKeyDown(((LPNMKEY)lparam)->nVKey , MODIFY_KEY_NONE) ) 
					{
						*outRet = 0;
						return true;
					}
				}
			case NM_SETFOCUS:
				{
					//�t�H�[�J�X���ς�������Ƃ�ʒm.
					Filer * filer = Filer::getFiler();
					filer->OnChangeFocus( Win->getIsLeft() );
					*outRet = 0;
					return true;
				}
			}

		}
		break;
	}
	return false;
}

//�L�[�������ꂽ�Ƃ��̏���
bool DisplayListViewBigIconWin32::OnKeyDown(unsigned char inKeyCode  , int inModifyKey ) const
{
	try
	{
		return this->SetupLoad.KeyDown(inKeyCode, (MODIFY_KEY) inModifyKey);
	}
	catch(RParseException e)
	{
		printf("�G���[���������܂���:\n%s\n", e.getMessage() );
	}
	return false;
}

//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
void DisplayListViewBigIconWin32::OnChangeFixedColum()
{
}

//�\�����ڂ̐ݒ�
void DisplayListViewBigIconWin32::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	this->RfileItems = inRfileItems;
	this->RfileItemsSize = inSize;

	//�C���[�W���X�g�Z�b�g.
	ListView.setImageList(FileModifyChacheLargeWin32::getInstance()->getImageList() , LVSIL_NORMAL);

	//������Ăяo���ƁALVN_GETDISPINFO ���Ă΂��炵���̂ŁA���
	//RfileItems RfileItemsSize ���Z�b�g���Ȃ��Ɨ����Ł[
	ListView.SetItemCountEx(inSize ,0 );

	//���ׂĂ̑I�����������܂��B
	ListView.ResetSelect();

	//�A�N�e�B���ȑI����O��̃f�B���N�g���ɍ��킹�܂��B
	const string lastPath = Win->getLastPath();
	const string nowPath = Win->getNowPath();
	if (nowPath != lastPath.substr( 0 ,nowPath.size() ) )
	{	//�V�����������f�B���N�g���Ȃ̂ŁA�擪�̃A�C�e���Ƀt�H�[�J�X�𓖂Ă܂��B
		ListView.SetItemState(0 , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		TRACE("���̃p�X�͈�v���Ȃ��������ł� %s -> %s\n" ,lastPath.c_str() ,  nowPath.c_str() );
	}
	else
	{	//�Ώۃt�@�C�����X�L�������܂��B

		//�T�[�`����^�[�Q�b�g.
		string filename = lastPath.substr( nowPath.size() );
		//�f�B���N�g���ȂǁA�Ō�� \\ �������Ă���ꍇ������̂Ő؂藎�Ƃ��܂��B
		if ( ! MultiString::IsPathToComplement(filename) )
		{
			filename = filename.substr(0 , filename.size() - 1 );
		}

		for(int i = 0; i<inSize ; i++)
		{
			if ( inRfileItems[i]->getVars(__FILECOLUMN_TYPE_NAME) == filename)
			{//�q�b�g.
				ListView.SetItemState(i , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				break;
			}
		}
		if (i==inSize)
		{//�q�b�g���Ȃ������ꍇ�́A��͂�A�擪�ɍ��킹�܂��B
			ListView.SetItemState(0 , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

}



//�_�u���N���b�N
bool DisplayListViewBigIconWin32::OnDoubleClick() const
{
	//�}�E�X�N���b�N�֌W�͂ǂ����I������Ă��邱�Ƃ������ł��B
	//�ǂ����I������Ă��Ȃ��ꍇ�́A�L�����Z������.
	if ( ListView.HitItem() == -1) return false;

	return OnKeyDown(VK_LBUTTON , MODIFY_KEY_DBCLICK);
}

//�J�����N���b�N
bool DisplayListViewBigIconWin32::OnColumnClick(LPDRAWITEMSTRUCT inItem) const
{
	return true;
}

//�A�N�e�B���ɂȂ�܂���
void DisplayListViewBigIconWin32::OnActive()  const
{
	HarfWindow();

	ListView.Show();

	::SetFocus( ListView.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayListViewBigIconWin32::OnNoActive() const
{
	ListView.Hide();
}


bool DisplayListViewBigIconWin32::OnGetDispInfo(NMLVDISPINFO* inInfo) const
{
	if (!RfileItems) return false;
	LVITEMA * item = &inInfo->item;

	const Rfile* rf = this->RfileItems[item->iItem];

	item->iImage = rf->getImageIndex();
	item->stateMask = 0;
	item->state = 0;
	item->pszText = (char*) rf->getVars(__FILECOLUMN_TYPE_NAME).c_str();
	return true;
}

bool DisplayListViewBigIconWin32::OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw ) const
{
	return false;
}

//���݃J�[�\��������t�@�C�������擾���܂��B
string DisplayListViewBigIconWin32::getActiveFilename() const 
{
	const int index = ListView.getNowSelected();
	if (index < 0 || index >= RfileItemsSize) return "";

	//Rfile �擾.
	const Rfile* rf = this->RfileItems[index];

	return rf->getVars(__FILECOLUMN_TYPE_NAME);
}

//���݃}�[�N����Ă���t�@�C����n���܂��B
StringList DisplayListViewBigIconWin32::getMarkFilenames() const 
{
	StringList selected;

	int index = -1;
	while((index = ListView_GetNextItem(ListView.getWindowHandle(), index, LVNI_ALL | LVNI_SELECTED))!=-1)
	{
		//�O�̂���
		if (index < 0 || index >= RfileItemsSize) continue;

		//Rfile �擾.
		const Rfile* rf = this->RfileItems[index];

		selected.push_back( rf->getVars(__FILECOLUMN_TYPE_NAME) );
	}

	return selected;
}


//�E�N���b�N
bool DisplayListViewBigIconWin32::OnRClick(const NMHDR * inNmhdr) 
{
	try
	{
		//�ǂ����I������Ă��Ȃ��ꍇ�́A���݈ʒu�̃A�C�e����I��������.
		//�N���b�N�����ʒu�ɂ���A�C�e�����A�N�e�B���ɂ���.
		const int item = ListView.HitItem();
		if (item != -1)
		{
			//���̈ʒu�̃A�C�e���͑I������Ă��܂���?
			if (! (ListView.GetItemState(item , LVIS_SELECTED) & (LVIS_SELECTED)) )
			{
				//�I������Ă��Ȃ��̂ŁA���ݑI������Ă���A�C�e�������ׂĉ������A
				ListView.ResetSelect();

				//���݃A�C�e����I�������܂��B
				ListView.SetItemState(item , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				
			}
		}

		//�|�b�v�A�b�v.
		PopupMenu("Popup");
	}
	catch(RException e)
	{
		printf("�G���[���������܂���:\n%s",e.getMessage() );
		return false;
	}

	return true;	
}


//�w�肵�����j���[���|�b�v�A�b�v����.
string DisplayListViewBigIconWin32::PopupMenu(const string inMenuName) const throw(RWin32Exception,RParseException,RNotFoundException) 
{
	POINT pt;
	GetCursorPos(&pt);

	return SetupLoad.Popup(inMenuName , pt , ListView.getWindowHandle());
}

//�\��������̂̍쐬.
void DisplayListViewBigIconWin32::CreateShowObject(const PathInterface* inPath) 
{

}

