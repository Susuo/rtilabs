// BFindAdressDlg.cpp: BFindAdressDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "BFindAdressDlg.h"
#include "WindowsMain.h"
#include "BBanana.h"
#include "resrc1.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BFindAdressDlg::BFindAdressDlg()
{

}

BFindAdressDlg::~BFindAdressDlg()
{

}

void BFindAdressDlg::Create(BBanana	*	poolBanana) throw(RWin32Exception)
{
	RDialog::Create(WindowsMain::getMainSystem()->getInstance() ,
					WindowsMain::getMainSystem()->getWindowHandle() , 
					"IDD_ADDRESS_FIND_DIALOG");

	Word.MountWindow( this->GetDlgItem(IDC_WORD_EDIT) );
	ListView.MountWindow( this->GetDlgItem(IDC_LIST) );
	ListView.InsertColumn(0 , 0 , "" , 320, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);

	this->Banana = poolBanana;
}

void BFindAdressDlg::OnCancel()
{
	this->Hide();
}

void BFindAdressDlg::OnSearch()
{
	string word = Word.getText();

	if ( word.empty() )
	{
		//�����񂪂���܂���
		TRACE( "��������������Ă�������" );
		return ;
	}

	RGISSmallFindCityToNumberConvert cityToNumberConvert;
	Banana->getGisMaster()->AllCityToCodeTable(&cityToNumberConvert);

	BAddressParse parse;
	if ( ! parse.Parse(word , cityToNumberConvert.getTable() , cityToNumberConvert.getTableCount()   ) )
	{
		TRACE( "�Z�����p�[�X�ł��܂���" );
		return ;
	}

	FindAddress.Delete();
	Banana->getGisMaster()->FindAddress(parse.getCode() , parse.getMachiCyoume() , parse.getBanchi() , &FindAddress);

	ListView.DeleteItemAll();

	int count = FindAddress.getCount();
	for(int i = 0;  i < count ; i++)
	{
		const RGISSmallFindObject * findObject = FindAddress.getPoint(i);
		int itemIndex = ListView.GetItemCount();
		ListView.SetItem(itemIndex  , 0 , findObject->getName() );
	}
}

bool BFindAdressDlg::OnDoubleClick()
{
	unsigned int select = ListView.getNowSelected();
	if (FindAddress.getCount() < select)
	{
		return false;	//�͈͊O�I��.
	}
	const RGISSmallFindObject * findObject = FindAddress.getPoint(select);
	Banana->LookAt( findObject->getPos() );

#ifdef _WIN32_WCE
	//CE �̏ꍇ�͑����������ق����킩��₷��
	this->Hide();
#endif
	return true;
}

bool BFindAdressDlg::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case VK_ESCAPE:
		OnCancel();
		break;
	case VK_RETURN:
		OnSearch();
		break;
	}
	return true;
}

//�E�B���h�E�v���V�[�W��.
LRESULT BFindAdressDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_SIPDOWN ;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//����
				OnSearch();
				break;
			case IDCANCEL:	//��ݾ�
				OnCancel();
				break;
			}
		}
		break;
	case WM_KEYDOWN:
		OnKeyDown( (unsigned char)inWParam );
		break;
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)inLParam;
			if (nmhdr && nmhdr->hwndFrom == ListView.getWindowHandle() )
			{
				switch(nmhdr->code)
				{
				case NM_CLICK:	//�_�u���N���b�N���ꂽ�Ƃ�
					{
						if (this->OnDoubleClick() )
						{
							return true;
						}
					}
					break;
				case LVN_KEYDOWN:
					{
					#ifndef _WIN32_WCE
						if (this->OnKeyDown( ((LPNMKEY)inLParam)->nVKey) )
					#else
						if (this->OnKeyDown( ((LPNMKEY)inLParam)->wVKey) )
					#endif
						{
							return true;
						}
					}
				}
			}
			else return false;
		}
		break;
	}
	return FALSE;
}

