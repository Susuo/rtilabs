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
const char * MYNAME = "�hS�U��CreateRemoteThread����";

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


typedef BOOL (__stdcall  *__SetWindowTextA)(HWND hWnd,LPCSTR lpString);
typedef HWND (__stdcall  *__GetDlgItem)(HWND hWnd,int nIDDlgItem);
struct INJECTIONDATA
{
	HWND             targetWindow;
	__SetWindowTextA funcSetWindowTextA;
	__GetDlgItem     funcGetDlgItem;
	char             nanohaKanbaiString[MAX_PATH];
};

const int ID_MI = 1000;
//����v���Z�X�œ��삷�郋�[�`��
//�K�R�I�ɑ��̊֐����ĂׂȂ��̂ŁA�����R�[�h�Ȃ�ł��傤�ˁB
DWORD __stdcall InjectionThread(LPVOID lpParameter)
{
	//�W�����v������R�[�h�����ƃA�h���X�������̂Œ��ӂ��Ă��B
	//�A�h���X�v�Z�͂߂�ǂ��̂ŃR���p�C���ɂ���Ă��炤��w
	INJECTIONDATA* data = (INJECTIONDATA*)lpParameter;
	HWND targetWindow = data->targetWindow;
	__SetWindowTextA funcSetWindowTextA = data->funcSetWindowTextA;
	__GetDlgItem funcGetDlgItem = data->funcGetDlgItem;
	const char * nanohaKanbaiString = data->nanohaKanbaiString;
	int id_mi = ID_MI;

//�����������Z���������B
//	data->funcSetWindowTextA( data->funcGetDlgItem(data->targetWindow , ID_MI ) , data->nanohaKanbaiString );
	__asm
	{
		push id_mi;
		push targetWindow;
		call funcGetDlgItem;

		push nanohaKanbaiString;
		push eax;

		call funcSetWindowTextA;

		xor eax,eax; //return 0;

		mov         esp,ebp   //�G�s���[�O
        pop         ebp  

		ret;
	};

	//�f�o�b�O���[�h�ɂ���ẮA esp�`�F�b�J�[������A����ɂ�鑊�΃W�����v���������Ă��܂��Ă܂Ƃ��ɓ����Ȃ��B
	//����āA���� return �͎g�p���Ă͂����Ȃ��B
	return 0;
}

//�֐����J�n�����ʒu�����߂�.
uintptr_t CalcSemeFunctionAddress(void * inSemeFunctionProc )
{
	//�t�b�N�֐����J�n������΃A�h���X
	if (*((unsigned char*)inSemeFunctionProc+0) == 0xe9)
	{
		//�t�b�N�֐��� ILT�o�R�Ŕ��ł���ꍇ
		//0xe9 call [4�o�C�g���΃A�h���X]          4�o�C�g���΂�64bit �ł��ς�炸
		uintptr_t jmpaddress = *((unsigned long*)((unsigned char*)inSemeFunctionProc+1));
		return (((uintptr_t)inSemeFunctionProc) + jmpaddress) + 5;
	}
	else
	{
		//���A�v���O�����̈�ɔ��ł���ꍇ
		return (uintptr_t)inSemeFunctionProc ;
	}
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

	//RemoteThread�ɓn���f�[�^�����B
	INJECTIONDATA injectionSrc = {0};
	injectionSrc.targetWindow = targetWindow;
	
	//�K�{API�n�̃��[�h.
	HMODULE user32dll = ::GetModuleHandle("User32");
	injectionSrc.funcSetWindowTextA = (__SetWindowTextA)::GetProcAddress(user32dll,"SetWindowTextA");
	injectionSrc.funcGetDlgItem = (__GetDlgItem)::GetProcAddress(user32dll,"GetDlgItem");
	strcpy(injectionSrc.nanohaKanbaiString , "�Ȃ̂͊���");

	//�t�b�N�p�֐��̃A�h���X�����߂܂��B
	//�����͂킩��Ȃ��̂ŁA�Ă��Ɓ[�Ɏ��܂��B�����]�v�ɂƂ��Ă����ʂ킯�ł͂Ȃ��̂ő��v����A�A���Ԃ�B
	uintptr_t functionStart = CalcSemeFunctionAddress(InjectionThread);
	uintptr_t functionSize = 256; //�֐��̒������� 256�o�C�g������΂����񂶂�ˁH

	//�hM����̃v���Z�X���Ђ炫�܂��B
	HANDLE processHandle = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ  | PROCESS_VM_WRITE , FALSE,processID );
	if (!processHandle)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "OpenProcess�Ɏ��s�B" , lastError);
	}

	//���s���ߕ����hM���񖼋`�Ń��������m�ۂ��Ă����܂��B
	//���ꂨ��hM�Ȃ񂾂��ǃ�������������ƐU�荞��ł���Ȃ����ȁ[�H
	void * remoteCodeMemory = VirtualAllocEx( processHandle, NULL , functionSize , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteCodeMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		return xreturn::windowsError(std::string() + "VirtualAllocEx�Ɏ��s�BremoteCodeMemory���m�ۂł��ʁB" , lastError);
	}
	DWORD dwNumBytesXferred;
	::WriteProcessMemory(processHandle, remoteCodeMemory, (void*)functionStart,functionSize,&dwNumBytesXferred);


	//���Ƀ��[�N���������m�ۂ��邽�߂ɁA�hM���񖼋`�Ń��������m�ۂ��Ă����܂��B
	//���ꂨ��hM�Ȃ񂾂��ǃ�������������ƐU�荞��ł���Ȃ����ȁ[�H
	void * remoteDataMemory = VirtualAllocEx( processHandle, NULL , functionSize , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteDataMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
		return xreturn::windowsError(std::string() + "VirtualAllocEx�Ɏ��s�BremoteDataMemory���m�ۂł��ʁB" , lastError);
	}
	::WriteProcessMemory(processHandle, remoteDataMemory, (void*)&injectionSrc,sizeof(INJECTIONDATA) ,&dwNumBytesXferred);

	HANDLE remoteThreadHandle;
	remoteThreadHandle = ::CreateRemoteThread( processHandle, NULL, 0,	
					(LPTHREAD_START_ROUTINE) remoteCodeMemory, 
					remoteDataMemory, 0, NULL );

	if( !remoteThreadHandle )
	{
		DWORD lastError = ::GetLastError();

		::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
		::VirtualFreeEx( processHandle, remoteDataMemory, functionSize , MEM_RELEASE );
		CloseHandle(processHandle);

		return xreturn::windowsError(std::string() + "CreateRemoteThread�Ɏ��s�B" , lastError);
	}

	//�����X���b�h���I������܂ő҂��܂�.
	::WaitForSingleObject( remoteThreadHandle, INFINITE );

	DWORD hLibModule;
	::GetExitCodeThread( remoteThreadHandle, &hLibModule );

	::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
	::VirtualFreeEx( processHandle, remoteDataMemory, functionSize , MEM_RELEASE );
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
