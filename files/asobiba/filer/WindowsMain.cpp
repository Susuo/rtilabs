// WindowsMain.cpp: WindowsMain �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WindowsMain.h"
#include "DisplayFactory.h"
#include "FileModifyChacheLargeWin32.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"MPR.LIB")


#include "RTabBarWin32.h"
#include "RTaskManager.h"
#include "RNotify.h"
#include "AtlActiveXBrige.h"
#include "RThreadExecuteOnMainThread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

WindowsMain::WindowsMain()
{
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
//��O			���������s�̏ꍇ�� RDoNotInitaliseException ����������.
int WindowsMain::Run()
{
    STARTUPINFO StatUpInfo;
    HINSTANCE hPrevInstance;
    int nCmdShow;

	//�X�^�[�g�A�b�v����.
    GetStartupInfo(&StatUpInfo);
    this->Instance = GetModuleHandle(0);
    hPrevInstance = 0;
    nCmdShow = (StatUpInfo.dwFlags & STARTF_USESHOWWINDOW)?
                StatUpInfo.wShowWindow:SW_SHOWNORMAL;

    WNDCLASS	wc;
    MSG			msg = {0};
	//���C���E�B���h�N���X�̍쐬.
	wc.lpszClassName = "SuperFiler";
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WndProcStatic;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = this->Instance;
    wc.hIcon         = LoadIcon(this->Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;

    if(!RegisterClass(&wc))
	{
        throw RDoNotInitaliseException("�E�B���h�E�N���X�̍쐬�Ɏ��s���܂���");
    }
	//���C���E�B���h�E�̍쐬.
    this->WindowHandle = CreateWindow(wc.lpszClassName,
				            wc.lpszClassName,WS_OVERLAPPEDWINDOW,
						    CW_USEDEFAULT, CW_USEDEFAULT,
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL,this->Instance, NULL);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("���C���E�B���h�E�̍쐬�Ɏ��s���܂���");
	}

	//�p�l����2����܂��B
	//---------------------------------------
	//|  BasePanelLeft  |  BasePanelRight   |
	//|                 |                   |
	//|                 |                   |
	//|                 |                   |
	//|                 |                   |
	//|-------------------------------------|

	//�����̃p�l���̍쐬
	Panel[0].Create(true);
	//�E���̃p�l���̍쐬
	Panel[1].Create(true);

	//�X�v���b�^�[�̍쐬.
	Spliter.setParentWnidow( this->getWindowHandle() );
	Spliter.setRubberBandSize(5);
	Spliter.setLeftWindowSize(0.50);


	//CommonControl�̏�����
	InitCommonControls();
	//Ole�̏�����
	::CoInitialize(NULL);
	//ATL�̏�����
	_Module.Init(0, Instance );

	//�T�C�Y����.
	OnSize();
	//�쐬.
	Filer* filer = Filer::getFiler();
	filer->OnCreate();
	//�E�B���h�E�\���J�n.
    ShowWindow(this->WindowHandle, nCmdShow);
    UpdateWindow(this->WindowHandle);

	//C++�̃N�Z static �̏�����.
	FileModifyChacheLargeWin32::getInstance();


	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	} 
	//ATL�̏I��
	_Module.Term();
	//COM�̏I������
	CoUninitialize();
	//�v���O�����I��.
	return msg.wParam;	
}


//�T�C�Y�ύX
void WindowsMain::OnSize()
{
	RECT theRC;

	//Left
	Spliter.getWindowRect(true,&theRC);
	Panel[0].MoveWindow(theRC.left , theRC.top , theRC.right-theRC.left , theRC.bottom-theRC.top);

	//Right
	Spliter.getWindowRect(false,&theRC);
	Panel[1].MoveWindow(theRC.left , theRC.top , theRC.right-theRC.left , theRC.bottom-theRC.top);
}

//�E�B���h�E�v���V�[�W��(none static)
LRESULT WindowsMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
	{
	case WM_CREATE:
		//�����E�B���h�E�ݒ�
		break;
	case WM_LBUTTONDOWN:
		if ( Spliter.OnLButtonDown(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_LBUTTONUP:
		if ( Spliter.OnLButtonUp(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_MOUSEMOVE:
		if ( Spliter.OnMouseMove(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_SIZE:
		OnSize();
		Filer::getFiler()->OnWindowResize();
		break;
	case WM_KEYDOWN:
		Filer::getFiler()->OnKeyDown( (unsigned char)wparam );
		break;
	case WM_CLOSE:
		Filer::getFiler()->OnDestory();
		DestroyWindow(hwnd);
		return 0;
	case WM_TASKEND:	//�^�X�N���I�����܂����B
		RTaskManager::getInstance()->CallEndOfTask((RTask*)wparam,(Window*)lparam);
		return 0;
	case RTI_WM_INTERRUPT:	//���C���X���b�h�Ɋ��荞��
		((RThreadExecuteOnMainThread*)(wparam))->_Start();
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

