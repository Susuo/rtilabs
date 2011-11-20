// RAddressBarWin32.cpp: RAddressBarWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RAddressBarWin32.h"
#include "RAutoLibrary.h"
#include "shlwapi.h"

#define SHACF_DEFAULT                   0x00000000  // Currently (SHACF_FILESYSTEM | SHACF_URLALL)
#define SHACF_FILESYSTEM                0x00000001  // This includes the File System as well as the rest of 
                                                    // the shell (Desktop\My Computer\Control Panel\)
#define SHACF_URLALL                    (SHACF_URLHISTORY | SHACF_URLMRU)
#define SHACF_URLHISTORY                0x00000002  // URLs in the User's History
#define SHACF_URLMRU                    0x00000004  // URLs in the User's Recently Used list.
#define SHACF_USETAB                    0x00000008  // URLs in the User's Recently Used list.
#define SHACF_FILESYS_ONLY              0x00000010  // Don't AutoComplete non-File System items.

#define SHACF_AUTOSUGGEST_FORCE_ON      0x10000000  // Ignore the registry default and force the feature on.
#define SHACF_AUTOSUGGEST_FORCE_OFF     0x20000000  // Ignore the registry default and force the feature off.
#define SHACF_AUTOAPPEND_FORCE_ON       0x40000000  // Ignore the registry default and force the feature on.
                                                    // (Also know as AutoComplete)
#define SHACF_AUTOAPPEND_FORCE_OFF      0x80000000  // Ignore the registry default and force the feature off.
                                                    // (Also know as AutoComplete)


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RAddressBarWin32::RAddressBarWin32(FilerSub* inFilerSub) : RAddressBar(inFilerSub )
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//�쐬.
	this->WindowHandle = 
		::CreateWindow( "STATIC" , "" ,WS_CHILD | WS_VISIBLE , 
		CW_USEDEFAULT , CW_USEDEFAULT , CW_USEDEFAULT , CW_USEDEFAULT ,
	    mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
		(HMENU)mainSystem->getID(),mainSystem->getInstance() , NULL);
	this->EditHandle = 
		::CreateWindow( "EDIT" , "" ,WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, 
		CW_USEDEFAULT , CW_USEDEFAULT , CW_USEDEFAULT , CW_USEDEFAULT ,
		this->WindowHandle ,(HMENU)mainSystem->getID(),mainSystem->getInstance() , NULL);
	//Edit�̃T�u�N���X��.
	AddressOrignalWindowProc = 
		(WNDPROC) GetWindowLong( this->EditHandle, GWL_WNDPROC);
	SetWindowLong( this->EditHandle , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->EditHandle , GWL_WNDPROC, (LONG)AddressWindowProcStatic);

	//�I�[�g�R���v���[�g�̎���
	//http://www.runan.net/program/tips/sdk.shtml
	typedef HRESULT (WINAPI *SHAUTOCOMPLETE)    (HWND hwndEdit, DWORD dwFlags);
	RAutoLibrary Lib;
	Lib.Create("SHLWAPI.DLL");
	SHAUTOCOMPLETE SHAutoCompleteAPI = (SHAUTOCOMPLETE) Lib.GetProcAddress("SHAutoComplete");
	if (SHAutoCompleteAPI)
	{
		HRESULT ret = SHAutoCompleteAPI(this->EditHandle, SHACF_DEFAULT);
		if (ret == S_OK)
		{
			TRACE("SHAutoCompleteAPI error\n");
		}
	}

	OnSize();
}

RAddressBarWin32::~RAddressBarWin32()
{
	::DestroyWindow(this->EditHandle);
	::DestroyWindow(this->WindowHandle);
}

//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
void RAddressBarWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	//�A�h���X�o�[
	MoveWindow(this->WindowHandle , itemRC.left , itemRC.top + ADDRESS_BAR_START , itemRC.right , itemRC.top + ADDRESS_BAR_SIZE , TRUE);
	//�A�h���X�o�[�̃G�f�B�b�g�{�b�N�X.
	MoveWindow(this->EditHandle , 0 , 0 , itemRC.right , ADDRESS_BAR_SIZE , TRUE);
}

//�E�B���h�E�v���V�[�W��.
LRESULT CALLBACK RAddressBarWin32::AddressWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		if (wparam == VK_RETURN)
		{
			char pathBuffer[MAX_PATH];
			GetWindowText(this->EditHandle ,pathBuffer ,  MAX_PATH);

			if (GetAsyncKeyState( VK_SHIFT) & 0x8000)
				NewView(pathBuffer);
			else
				PathView(pathBuffer);

			return 0;
		}
		break;
	}
	return ( CallWindowProc( AddressOrignalWindowProc ,
												hwnd,msg,wparam,lparam) );
}

//�w�肵���p�X���{������.
void RAddressBarWin32::PathView(string inPath)
{
	Window* win = FSub->getActiveWindow();
	if (win)
	{
		//���݂̃E�B���h�E�ŉ{������.
		win->PreviewOrLs( inPath );
	}
	else
	{
		//�V�����E�B���h�E�Ńp�X���{������.
		NewView(inPath);
	}
}

//�V�����E�B���h�E�Ńp�X���{������.
void RAddressBarWin32::NewView(string inPath)
{
	FSub->setActiveWindow( FSub->NewWindow( inPath ) );
}

//�p�X���ύX���ꂽ�Ƃ��ɌĂяo�����.
void RAddressBarWin32::OnAddressChange(string inNewPath)
{
	SetWindowText(this->EditHandle , inNewPath.c_str() );
}

