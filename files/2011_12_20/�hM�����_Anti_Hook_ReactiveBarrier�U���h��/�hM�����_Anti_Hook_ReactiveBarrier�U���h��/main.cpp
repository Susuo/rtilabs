/*
������̃A�v���P�[�V�����ł��B


LdrLoadDll �� BaseThreadInitThunk API ���t�b�N���邱�ƂŁA������t�b�N����h�q�ł��܂��B
(�J�[�l�����C���[����̃t�b�N�������B)

BaseThreadInitThunk����h�q����Ƃ��ɑ���̃v���Z�X�ɑ΂��āASleep(3��) �𔭍s���邱�ƂŁA
����̃v���Z�X��3���~����Ԃɂ���d�Ԃ������܂��B

*/
#include <windows.h>
#include <Ntsecapi.h>	// for PUNICODE_STRING
#include <tchar.h>
#include <tlhelp32.h>	
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //���C���E�B���h�E
const char * MYNAME = "�hM�����_Anti_ALLHook���G�̃o���A";


DWORD WINAPI USOThread(LPVOID lpParameter)
{
	MessageBox(g_MainWnd , "�hS����̃v���Z�X���U�����d�|���Ă����̂Ń����z��܂��B" , "���₷�݁[",0);
	::Sleep(1000 * 60 * 60 * 24 * 3);
	return 0;
}


SEXYHOOK_BEGIN(void,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
			("kernel32.dll","BaseThreadInitThunk"),(LPVOID threadParama,LPVOID nazo ))
{
	//���̃��W�X�^�̒l�͉󂵂Ă͂����Ȃ�
	void * _ecx = NULL;
	void * _edx = NULL;
	_asm
	{
		mov _ecx , ecx; //�����̃t���O�ɗ��p���Ă���炵��
		mov _edx , edx; //�X���b�h���Ŏ��s����֐��̃A�h���X
	}
	//�I�ʃ��[�`��

	//VirtualAlloc ���ꂽ�������̔��ʂ�����B
	//�Ƃ肠�����A VirtualAlloc����l�́A PAGE_EXECUTE_READWRITE ���Ă����肵����A �R�[�h�̈�Ȃ̂�WRITE���ł���[���Ă������Ƃŗ��Ƃ����Ƃɂ����B
	//���肪�΍􂵂Ă����疳�Ӗ��Ȃ̂ŁA�A�A���{�I�ȑΏ����l���Ȃ��Ƃ����Ȃ��B
	if (!IsBadWritePtr(_edx , 1))
	{
		//�R�̃X���b�h�֐���n���Ă�����B
		_edx = (void*)USOThread;
	}

	//�I���W�i���̊֐� +5 ��sexyhook���j�󂵂Ă���̈���΂����߃A�h���X�����炷.
	void* orignalFunction = (void*)(((unsigned long) sexyhookThis->getOrignalFunctionAddr() ) + 5);
	
	//�Ȃ�W�i���� BaseThreadInitThunk ���Ăяo���܂��B
	_asm
	{
		push nazo;
		push threadParama;

		mov  eax,orignalFunction;    //ebp esp �����ŏ��������邽�� ���[�J���ϐ��ɃA�N�Z�X�ł��Ȃ��Ȃ�̂ŁAeax�ɕێ�.
		mov  ecx,_ecx;
		mov  edx,_edx;

		//���̊֐����Ȃ��������ɂ������̂ŁA �X�^�b�N�t���[���𕜌�����.
		mov esp,ebp
		//ebp�̕���
		pop ebp

		//BaseThreadInitThunk �̃v�����[�O
		mov         edi,edi  
        push        ebp  
        mov         ebp,esp  

		//���̃��[�`���ɔ�΂�.
		jmp         eax 
//74AB338F  jne         @BaseThreadInitThunk@12+15h (74AB620Ah)    �����ɔ�΂�.
	}
}
SEXYHOOK_END_AS() g_BaseThreadInitThunk;


//DLL�Ɠ����ꏊ�ɂ���hS��exe�������I�������܂��B
void killExecute(const wchar_t * dllpath )
{//���\�蔲��
	wchar_t path[MAX_PATH];
	wcscpy( path , dllpath);

	wchar_t* p = wcsrchr(path, L'\\');
	if (!p)
	{
		return ;
	}
	*p = 0;
	wcscat( path , L"\\*.exe");

	bool foundS = false;
	WIN32_FIND_DATAW data;
	HANDLE findHandle = FindFirstFileW( path , &data);
	if (findHandle == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	do
	{
		if ( wcsstr(data.cFileName ,L"�hS" ) != NULL )
		{
			//�hS����Ȃ��Ɖ������Ȃ�.
			foundS = true;
			break;
		}
	}
	while( FindNextFileW(findHandle ,  &data) );
	FindClose(findHandle);

	//�hS��������Ȃ���Ή������Ȃ�.
	if (!foundS)
	{
		return ;
	}

	//�v���Z�X��T��.
	DWORD dwProcessId = NULL;

	HANDLE hSnap;
	if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);
	BOOL bResult = Process32FirstW(hSnap, &pe);
	while(bResult)
	{
		if(wcsstr(pe.szExeFile, data.cFileName ) != NULL)
		{
			dwProcessId = pe.th32ProcessID;
			break;
		}
		bResult = Process32NextW(hSnap, &pe);
	}
	CloseHandle(hSnap);
	if (dwProcessId == NULL)
	{
		return ;
	}

	//�������̂ŎE��.
	HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
	if (!processHandle)
	{
		return ;
	}
	TerminateProcess(processHandle , 0 );
	CloseHandle(processHandle);
}



SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
		("NTDLL.dll","LdrLoadDll"),(PWCHAR PathToFile,ULONG Flags,PUNICODE_STRING ModuleFileName,PHANDLE ModuleHandle))
{
	//�C�ɐH��Ȃ�dll�̓��[�h�����Ȃ�. 
	if (wcsstr(ModuleFileName->Buffer ,L"�hS" ) != NULL)
	{
		//DLL�Ɠ����ꏊ�ɂ���exe �����s����������A�I��������w
		killExecute(ModuleFileName->Buffer);

		MessageBox(g_MainWnd , "�hS����̃v���Z�X��\t\n�U�����d�|���Ă����̂ŁA�l�����܂����B" , "Zap!" , 0);

		return 1; //���[�h�֎~
	}

	//����ȊO�͂ӂ[�Ƀ��[�h�����

	//�ꎞ�I�Ƀt�b�N������
	SEXYHOOK_UNHOOK();
	//���̊֐����Ăяo��
	return CallOrignalFunction(PathToFile,Flags,ModuleFileName,ModuleHandle);
}
SEXYHOOK_END_AS() g_LdrLoadDllHook;



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
	const int ID_NANOHA_STATUS = 1000;
	const int ID_BARRIER = 1001;
	static HWND g_NanohaStatusWindow = NULL;	 //�U������E�B���h�E
	static HWND g_BarrierButtonWindow = NULL;    //�o���A�{�^��
    switch (msg) {
		case WM_CREATE:
			g_NanohaStatusWindow = CreateWindow("STATIC","�Ȃ̃t�F�C�̔����{��������",
                 WS_CHILD|WS_VISIBLE|SS_LEFT,
                 20,20,300,30,
                 hWnd,
                 (HMENU)ID_NANOHA_STATUS, 
				 g_Instance,
                 NULL);
            g_BarrierButtonWindow = CreateWindow(
                "BUTTON", "�U���h��!", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 100, 300, 60, hWnd, (HMENU)ID_BARRIER, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_BARRIER:
				//API�t�b�N�ɂ��o���A��W�J����.
				g_BaseThreadInitThunk.Hook();
				g_LdrLoadDllHook.Hook();

				SetWindowText(g_BarrierButtonWindow , "�U���h�ǓW�J��");
				EnableWindow(g_BarrierButtonWindow , FALSE);
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

