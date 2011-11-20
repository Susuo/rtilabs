// DisplayWin32.cpp: DisplayWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayWin32.h"
#include "RFunctionScript.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DisplayWin32::DisplayWin32(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{
	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//���C���ƂȂ郊�X�g�r���[�����܂��B
	ListView.Create(mainSystem->getInstance(), 
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_TABSTOP | LVS_REPORT | LVS_OWNERDATA | WS_VISIBLE);

	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	//��s�I����������A Fixed ���h���b�O�ł�����ł���悤�ɂ��܂��B
	ListView.setExtended( ListView.getExtended() | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

	//�^���Ɉړ�����.
	this->HarfWindow();
}

//�E�B���h�E�𔼕��̈ʒu�Ɉړ�����.
void DisplayWin32::HarfWindow() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	ListView.MoveWindow(baseWindowRC.left , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);

}

DisplayWin32::~DisplayWin32()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;
}

//WindowProc���󂯎��
//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
//�����͏������Ȃ��z�� false ��Ԃ��B
bool DisplayWin32::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_NOTIFY:
		{
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
					if (this->OnKeyDown((LPNMKEY) lparam) )
					{
						*outRet = 0;
						return true;
					}
				}
			case NM_SETFOCUS:
				{
					//�t�H�[�J�X���ς�������Ƃ�ʒm.
					Filer * filer = Filer::getFiler();
					filer->OnChangeFocus();
					*outRet = 0;
					return true;
				}
			}
		}
		break;
	}
	return false;
}

//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
void DisplayWin32::OnChangeFixedColum()
{
	RECT rc;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &rc);


	int windowWidthSize = rc.left - rc.right - 16;

	//���ׂẴJ�����̔j��
	ListView.ClearColumn();

	for(int i = 0; i < FixedColumn.getColumnSize() ; i++)
	{
		const RFixedColumn* colum = FixedColumn.getColumn(i);
		//����ȏ�v�f�Ȃ��悾�����烋�[�v�𔲂��܂��B
		if (colum->getType() == __FILECOLUMN_TYPE_END) break;

		//�e�v�f�ɕ�����ď������܂��B
		switch( colum->getType() )
		{
		case __FILECOLUMN_TYPE_NAME:
		case __FILECOLUMN_TYPE_EXT:
		case __FILECOLUMN_TYPE_SIZE:
		case __FILECOLUMN_TYPE_DATE:
		case __FILECOLUMN_TYPE_ATTRIB:
			ListView.InsertColumn( i , 0 , (char*)colum->getName().c_str() ,
				colum->getWidthParsent() * windowWidthSize, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT );
			break;
		case __FILECOLUMN_TYPE_ICON:
			ListView.InsertColumn( i , 0 , (char*)colum->getName().c_str() , 
				16 , LVCF_FMT | LVCF_WIDTH | LVCF_IMAGE | LVCF_SUBITEM ,LVCFMT_LEFT );
			break;
		}
	}
}

//�\�����ڂ̐ݒ�
void DisplayWin32::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ListView.SetItemCountEx(inSize,0);
	this->RfileItems = inRfileItems;
	this->RfileItemsSize = inSize;

	//�C���[�W���X�g�Z�b�g.
	ListView.setImageList(FileModifyChacheLargeWin32::getInstance()->getImageList() , LVSIL_NORMAL);
}


//�_�u���N���b�N
bool DisplayWin32::OnDoubleClick()
{
	RFunctionScript	rfs;
	rfs.Interpreter("&VIEW $P$F");

	return true;
}

//�J�����N���b�N
bool DisplayWin32::OnColumnClick(LPDRAWITEMSTRUCT inItem)
{
	return true;
}

//�L�[���Ȃ񂩉�����܂���
bool DisplayWin32::OnKeyDown(LPNMKEY inKey) 
{
	RFunctionScript	rfs;

	switch(inKey->nVKey & 0x00ff)
	{
	case VK_RETURN:
		rfs.Interpreter("&VIEW $P$F");
		return true;
	case VK_BACK:
		rfs.Interpreter("&UPDIR");
		return true;
	}
	return false;

}

//�A�N�e�B���ɂȂ�܂���
void DisplayWin32::OnActive()  const
{
	ListView.Show();

	::SetFocus( ListView.getWindowHandle() );
}

//��A�N�e�B���ɂȂ�܂���
void DisplayWin32::OnNoActive() const
{
	ListView.Hide();
}


bool DisplayWin32::OnGetDispInfo(NMLVDISPINFO* inInfo)
{
	return false;
}


bool DisplayWin32::OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw )
{
	return false;
}


//���݃J�[�\��������t�@�C�������擾���܂��B
string DisplayWin32::getActiveFilename() const 
{
	int index = ListView.getNowSelected();
	if (index < 0 || index >= RfileItemsSize) return "";

	//Rfile �擾.
	const Rfile* rf = this->RfileItems[index];

	return rf->getVars(__FILECOLUMN_TYPE_NAME);
}

//���݃}�[�N����Ă���t�@�C����n���܂��B
StringList DisplayWin32::getMarkFilenames() const 
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
