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
const char * MYNAME = "�hS�U��CreateRemoteThread��DLL_Injection";

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

//CreateRemoteThread �� �hM ���U������
xreturn::r<bool> Attach()
{
	HMODULE kernel32dll = ::GetModuleHandle("Kernel32");

	//���ߍ��ރt�b�N�p��dll
	char injectionDLLName[MAX_PATH];
	if( !GetModuleFileName( g_Instance ,injectionDLLName,_MAX_PATH) )
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "GetModuleFileName�Ɏ��s�B" , lastError);
	}
	_tcscpy(_tcsrchr(injectionDLLName, '\\') + 1, "�hS�U���pDLL_CreateRemoteThread.dll" );

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

	//�hM����̃v���Z�X���Ђ炫�܂��B
	HANDLE processHandle = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ  | PROCESS_VM_WRITE , FALSE,processID );
	if (!processHandle)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "OpenProcess�Ɏ��s�B" , lastError);
	}

	//������p�̃��������m�ۂ��邽�߂ɁA�hM���񖼋`�Ń��������m�ۂ��Ă����܂��B
	//���ꂨ��hM�Ȃ񂾂��ǃ�������������ƐU�荞��ł���Ȃ����ȁ[�H
	void * remoteMemory = VirtualAllocEx( processHandle, NULL , lstrlen(injectionDLLName) , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		return xreturn::windowsError(std::string() + "VirtualAllocEx�Ɏ��s�B" , lastError);
	}
	::WriteProcessMemory(processHandle, remoteMemory, (void*)injectionDLLName,lstrlen(injectionDLLName),NULL);

	//�^�[�Q�b�g�v���Z�X��ɃX���b�h���쐬���Adll�𒍓����܂��B

	HANDLE remoteThreadHandle;
	//�t�b�N
	remoteThreadHandle = ::CreateRemoteThread( processHandle, NULL, 0,	
					(LPTHREAD_START_ROUTINE) ::GetProcAddress(kernel32dll,"LoadLibraryA"), 
					remoteMemory, 0, NULL );

	if( !remoteThreadHandle )
	{
		DWORD lastError = ::GetLastError();

		::VirtualFreeEx( processHandle, injectionDLLName, lstrlen(injectionDLLName) , MEM_RELEASE );
		CloseHandle(processHandle);

		return xreturn::windowsError(std::string() + "CreateRemoteThread�Ɏ��s�B" , lastError);
	}

	//�����X���b�h���I������܂ő҂��܂�.
	::WaitForSingleObject( remoteThreadHandle, INFINITE );

	DWORD hLibModule;
	::GetExitCodeThread( remoteThreadHandle, &hLibModule );

	::VirtualFreeEx( processHandle, injectionDLLName, lstrlen(injectionDLLName) , MEM_RELEASE );
	::CloseHandle( processHandle );
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
