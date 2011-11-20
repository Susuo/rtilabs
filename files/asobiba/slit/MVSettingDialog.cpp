// MVSettingDialog.cpp: MVSettingDialog �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVSettingDialog.h"
#include "MVConfig.h"
#include "WindowsMain.h"
#include "resrc1.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MVSettingDialog::MVSettingDialog()
{

}

MVSettingDialog::~MVSettingDialog()
{

}

void MVSettingDialog::Create(MVMain	*	poolMVMain) throw(RWin32Exception)
{
	RDialog::Create(WindowsMain::getMainSystem()->getInstance() ,
					WindowsMain::getMainSystem()->getWindowHandle() , 
					"IDD_CONFIG_DIALOG");

	LastMemory.MountWindow( this->GetDlgItem(IDC_LASTMEMORY_COMBO) );
	JustFit.MountWindow( this->GetDlgItem(IDC_JUSTFIT_COMBO) );
	ScrollCount.MountWindow( this->GetDlgItem(IDC_SCROLLCOUNT_COMBO) );
	ScrollType.MountWindow( this->GetDlgItem(IDC_SCROLLTYPE_COMBO) );
	

	LastMemory.AddItem("����");			//0
	LastMemory.AddItem("���Ȃ�");		//1

	JustFit.AddItem("����");			//0
	JustFit.AddItem("���Ȃ�");			//1

	ScrollCount.AddItem("1");			//0
	ScrollCount.AddItem("2");			//1
	ScrollCount.AddItem("3");			//2
	ScrollCount.AddItem("4");			//3
	ScrollCount.AddItem("5");			//4
	ScrollCount.AddItem("6");			//5

	ScrollType.AddItem("��");			//0
	ScrollType.AddItem("�c");			//1

	LastMemory.Select( MVConfig::getInstance()->getLastMemory() == true ? 1 : 0);
	JustFit.Select( MVConfig::getInstance()->getJustFit() == true ? 1 : 0);
	ScrollCount.Select( MVConfig::getInstance()->getScrollCount() - 1);
	ScrollType.Select( MVConfig::getInstance()->getScrollType() );

	this->PoolMVMain = poolMVMain;
}

void MVSettingDialog::OnCancel()
{
	this->Hide();
}

void MVSettingDialog::OnOK()
{
	try
	{
		MVConfig::getInstance()->setFlg(LastMemory.getNowSelected() == 1 ? true : false ,
										JustFit.getNowSelected() == 1 ? true : false,
										ScrollCount.getNowSelected() + 1,
										(MVConfig::SCROLL_TYPE) ScrollType.getNowSelected() );

		PoolMVMain->OnSettingChange();
	}
	catch(RException e)
	{
		RMessageBox::Message( e.getMessage() );
	}
	this->Hide();
	WindowsMain::Update();
}

//�E�B���h�E�v���V�[�W��.
LRESULT MVSettingDialog::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//�J��
				OnOK();
				break;
			case IDCANCEL:	//��ݾ�
				OnCancel();
				break;
			}
		}
		break;
	}
	return FALSE;
}

