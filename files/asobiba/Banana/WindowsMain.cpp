// WindowsMain.cpp: WindowsMain �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WindowsMain.h"
#include "resrc1.h"
#include "RDoNotInitaliseException.h"

#ifndef _WIN32_WCE
	#pragma comment(lib,"comctl32.lib")
	#pragma comment(lib,"winmm.lib")
	//#pragma comment(lib,"MPR.LIB")
#endif


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

WindowsMain::WindowsMain()
{
	Instance = 0;
}

WindowsMain::~WindowsMain()
{

}

WindowsMain* WindowsMain::getMainSystem()
{
	static WindowsMain	main;
	return &main;
}

//�K����ID��U��o��(w
int WindowsMain::getID()
{
	static id = 1000;
	return id++;
}

//���s.
//�߂�l		�v���O�����̃��^�[���l.
//��O			���������s�̏ꍇ�� RException ����������.
int WindowsMain::Run() throw(RException)
{
//    STARTUPINFO StatUpInfo;
    HINSTANCE hPrevInstance;
//    int nCmdShow;

	//�X�^�[�g�A�b�v����.
//    GetStartupInfo(&StatUpInfo);
    this->Instance = GetModuleHandle(0);
    hPrevInstance = 0;
//    nCmdShow = (StatUpInfo.dwFlags & STARTF_USESHOWWINDOW)?
//                StatUpInfo.wShowWindow:SW_SHOWNORMAL;

    MSG			msg = {0};
	//CommonControl�̏�����
	InitCommonControls();
	//Ole�̏�����
//	::CoInitialize(NULL);
	//ATL�̏�����
//	_Module.Init(0, Instance );

	//�쐬.
	BananaDlg.Create();

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if ( !BananaDlg.IsDialogMessage(&msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	} 
	//ATL�̏I��
//	_Module.Term();
	//COM�̏I������
//	CoUninitialize();
	//�v���O�����I��.
	return msg.wParam;	
}


//�E�B���h�E�v���V�[�W��(none static)
LRESULT WindowsMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(hwnd, msg, wparam, lparam);
}


//��ʂ̍X�V.
void WindowsMain::Update()
{
	InvalidateRect( getMainSystem()->getWindowHandle() , NULL , TRUE);
	
}

