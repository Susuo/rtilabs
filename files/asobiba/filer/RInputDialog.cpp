// RInputDialog.cpp: RInputDialog �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RInputDialog.h"
#include "WindowsMain.h"
#include "IMyEnumString.h"		//�I�[�g�R���v���[�g�p.
#include "resource.h"
#include "RDataLoad.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RInputDialog::RInputDialog() 
{
	AutoComplete = NULL;
	EnumStr = NULL;
}

RInputDialog::~RInputDialog()
{
	if (AutoComplete) AutoComplete->Release();
//	if (EnumStr)	EnumStr->Release();
}

string RInputDialog::DoModal(string inTitle , string inAutoCompleteName , string inDefualt)	throw(RWin32Exception,RNoComplateException)
{
	Title = inTitle;
	AutoCompleteName = inAutoCompleteName;
	Defualt = inDefualt;

	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	int ret =::DialogBoxParam(mainSystem->getInstance() ,"INPUTDIALOG", mainSystem->getWindowHandle() , (DLGPROC)DlgProcStatic ,(LPARAM) this);
	if ( ret == -1 )
	{
		throw RWin32Exception("���̓_�C�A���O���쐬�ł��܂���");
	}
	//�L�����Z�����ꂽ�ꍇ
	if (ret == IDCANCEL)
	{
		throw RNoComplateException("���̓_�C�A���O�ŃL�����Z������܂���");
	}
	if (InputData.empty() )
	{
		throw RNoComplateException("���̓_�C�A���O�� NULL �����͂���܂���");
	}

	return InputData;
}

//�E�B���h�E�v���V�[�W��.
LRESULT RInputDialog::DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return OnInitDialog() != false;
	case WM_COMMAND:
		{
			int p = LOWORD(wparam);
			switch (LOWORD(wparam)) 
			{
			case IDOK:
				OnOK();
				break;
			case IDCANCEL:
				OnCancel();
				break;
			}
		}
		break;
	}
	return FALSE;
}

//OK�������ꂽ�Ƃ��̏���.
void RInputDialog::OnOK()
{
	//�G�f�B�b�g�{�b�N�X���͂���Ă���l��  InputData �Ɏ擾.
	HWND	EditWindowHandle = GetDlgItem(this->WindowHandle, IDC_INPUTEDIT);
	char	buf[MAX_PATH];
	GetWindowText(EditWindowHandle , buf , MAX_PATH); 
	InputData = buf;

	::EndDialog(this->WindowHandle , IDOK);
}

//�L�����Z���������ꂽ�Ƃ��̏���.
void RInputDialog::OnCancel()
{
	::EndDialog(this->WindowHandle , IDCANCEL);
}

bool RInputDialog::OnInitDialog()
{

	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	try
	{
		//�^�C�g���ɃZ�b�g
		SetWindowText(this->WindowHandle , Title.c_str() );

		//�G�f�B�b�g�{�b�N�X�Ƀf�B�t�H���g���Z�b�g.
		HWND	EditWindowHandle = GetDlgItem(this->WindowHandle, IDC_INPUTEDIT);
		SetWindowText(EditWindowHandle , Defualt.c_str() );

		//�⊮�V�X�e�����쐬.
		EnumStr = new IMyEnumString(NULL, LoadComplete(AutoCompleteName) );

		// IAutoComplete�C���^�[�t�F�C�X���쐬
		if (CoCreateInstance(CLSID_AutoComplete, NULL, CLSCTX_INPROC_SERVER,
				IID_IAutoComplete, (void**)&AutoComplete) != S_OK)
		{
			throw RNotSupportException("CoCreateInstance �Ɏ��s���܂���.");
		}

		// �G�f�B�b�g�{�b�N�X��Ώۂɂ���.
		if (AutoComplete->Init(EditWindowHandle, EnumStr, NULL, NULL) != S_OK)
		{
			throw RNotSupportException("�o�^�ł��܂���.");
		}
//	IAutoComplete2* pAuto2 = NULL;
//	if (SUCCEEDED(pAutoComp->QueryInterface(IID_IAutoComplete2, (LPVOID*)&pAuto2))){
//		pAuto2->SetOptions(ACO_AUTOSUGGEST);
//		pAuto2->Release();
//	}
	}
	catch(RException e)
	{
		printf("�I�[�g�R���v���[�g�쐬�ŃG���[���������܂���:\n%s\n" , e.getMessage() );
		return false;
	}

	return true;
}

//�R���v���[�g�f�[�^�̓ǂݍ���.
WStringList RInputDialog::LoadComplete(string inName)
{
	string dataes;
	try
	{
		dataes = RDataLoad::Load("filer.dat" , "AutoComplete" , inName);
	}
	catch(RException e)
	{
		printf("�R���v���[�g�f�[�^�̎擾�Ɏ��s\n%s\n",e.getMessage() );
		return WStringList();
	}

	USES_CONVERSION;
	WStringList ret;
	// \t ��؂�Ȃ̂�..
	int findstart = 0;
	int hitpos = 0;
	while(1)
	{
		hitpos = dataes.find('\t' , findstart);
		if (hitpos == -1) break;
		string one = dataes.substr(findstart , hitpos - findstart);

		ret.push_back( T2COLE(one.c_str()) );

		findstart = hitpos + 1;
	}

	return ret;
}

//�R���v���[�g�f�[�^�̏�������
void RInputDialog::SaveComplete(string inName , WStringList inTargetString)
{
	USES_CONVERSION;

	string writeString;
	for(int i = 0; i < inTargetString.size() ; i++)
	{
		 writeString += string( OLE2CT( inTargetString[i].c_str() ) ) + "\t";
	}
	try
	{
		RDataLoad::Save("filer.dat" , "AutoComplete" , inName,writeString);
	}
	catch(RException e)
	{
		printf("�R���v���[�g�f�[�^�̏������݂Ɏ��s\n%s\n",e.getMessage() );
	}
}

