/*
CreateRemoteThread �ɂ�� DLL INJECTION �� �hM���U������T���v���ł��B

*/
#include <windows.h>
#include "xreturn.h"
#include <tchar.h>
//#include <imagehlp.h>
#include <tlhelp32.h>	

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //���C���E�B���h�E
const char * MYNAME = "�hS�U��HookWindowsEx��DLL_Injection";

//�hM�v���Z�X��������R�[�h�B
//"�hM����" �Ƃ����v���Z�X���ōŏ��Ƀ}�b�`�������̂��^�[�Q�b�g�Ƃ���B
xreturn::r<DWORD> findTargetProcess()
{
	DWORD dwProcessId = NULL;

	HANDLE hSnap;
	if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "CreateToolhelp32Snapshot�Ɏ��s" , lastError);
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	BOOL bResult = Process32First(hSnap, &pe);
	while(bResult)
	{
		if(strstr(pe.szExeFile, "�hM����") != NULL)
		{
			CloseHandle(hSnap);
			return pe.th32ProcessID;
		}
		bResult = Process32Next(hSnap, &pe);
	}
	CloseHandle(hSnap);
	return 0;
}

//�v���Z�XID����Ή�����HWND ���擾����.
//�o�T��: http://d.hatena.ne.jp/yu-hr/20100323/1269355469
HWND GetWindowHandle(	// �߂�l: ���� �]�݂�HWND / ���s NULL
	const DWORD TargetID)	// �v���Z�XID
{
	HWND hWnd = GetTopWindow(NULL);
	do {
		if(GetWindowLong( hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible( hWnd))
		{
			continue;
		}
		DWORD ProcessID;
		GetWindowThreadProcessId( hWnd, &ProcessID);
		if(TargetID == ProcessID)
		{
			return hWnd;
		}
	} while((hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

	return NULL;
}



xreturn::r<int> Attach()
{
	//�hM����̃v���Z�X��T���܂��B
	auto processID = findTargetProcess();
	if (processID.isError())
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "GetModuleFileName�Ɏ��s�B" , lastError);
	}
	if ( (DWORD)processID == 0)
	{
		return xreturn::error(std::string() + "�U���Ώۂ����܂���" ,0);
	}
	//�hM����̃��C���E�B���h�E�������܂��B
	HWND targetWindow = GetWindowHandle(processID);
	if (!targetWindow)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "���C���E�B���h�E��������܂���B" , lastError);
	}

	HMODULE dll = LoadLibrary("�hS�U���pDLL.dll");
	if (!dll)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError("�U���pDLL��LoadLibrary�Ɏ��s",lastError );
	}
	typedef bool (*INJECTDLLDEF)( HWND targetwindow );
	INJECTDLLDEF injcetdllFunction = (INJECTDLLDEF)::GetProcAddress(dll,"InjectDll");
	if (!injcetdllFunction)
	{
		DWORD lastError = ::GetLastError();
		FreeLibrary(dll);
		return xreturn::windowsError("�U���pDLL�� InjectDll������܂���",lastError );
	}
	bool r = injcetdllFunction(targetWindow);
	if (!r)
	{
		FreeLibrary(dll);
		return xreturn::error(std::string() + "�t�b�N�Ɏ��s���܂���" ,0);
	}

	FreeLibrary(dll);
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
                   LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS myProg;

	g_Instance = hInstance;

    if (!hPreInst) {
        myProg.style            =CS_HREDRAW | CS_VREDRAW;
        myProg.lpfnWndProc      =WndProc;
        myProg.cbClsExtra       =0;
        myProg.cbWndExtra       =0;
        myProg.hInstance        =hInstance;
        myProg.hIcon            =NULL;
        myProg.hCursor          =LoadCursor(NULL, IDC_ARROW);
        myProg.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
        myProg.lpszMenuName     =NULL;
        myProg.lpszClassName    =MYNAME;
        if (!RegisterClass(&myProg))
            return FALSE;
    }
    g_MainWnd = CreateWindow(MYNAME
        ,MYNAME
        ,WS_OVERLAPPEDWINDOW
        ,CW_USEDEFAULT
        ,CW_USEDEFAULT
        ,400
        ,400
        ,NULL
        ,NULL
        ,hInstance
        ,NULL);
    ShowWindow(g_MainWnd, nCmdShow);
    UpdateWindow(g_MainWnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�e��A�E�B���h�E����p �ʓ|�̂�static�ł��������H
	const int ID_ATTACH = 1000;
	static HWND g_AttachButtonWindow = NULL;	 //�U���{�^��
    switch (msg) {
		case WM_CREATE:
            g_AttachButtonWindow = CreateWindow(
                "BUTTON", "�{�^���������ƃhM������U�������B", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 20, 350, 60, hWnd, (HMENU)ID_ATTACH, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_ATTACH:
				auto r = Attach();
				if (!r)
				{
					std::string errorMessage = std::string() + "�U���Ɏ��s���܂����B" + r.getErrorMessage();
					MessageBox( NULL , errorMessage.c_str()  , "���s" , 0);
					break;
				}

				SetWindowText(g_AttachButtonWindow , "�U������!?");
				EnableWindow(g_AttachButtonWindow , FALSE);
				break;
			}
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}
