/*
������̃A�v���P�[�V�����ł��B

BaseThreadInitThunk API ���t�b�N���邱�ƂŁA������ CreateRemoetThread ��h�q�ł��܂��B

*/
#include <windows.h>
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //���C���E�B���h�E
const char * MYNAME = "�hM�����_Anti_RemoteThread";


DWORD WINAPI USOThread(LPVOID lpParameter)
{
	MessageBox( g_MainWnd , "CreateRemoteThread�����o�����̂Ŗ��͉����܂����B\r\n" , "CreateRemoteThread���o" , 0);
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

	//VirtualAlloc ���ꂽ�������� 32bit���� 0x70000000 �ȍ~�ɔz�u�����̂ŁA���̃A�h���X�Ŕ��f���Ă���B
	//�����Ƃ����Ƃ����@�͂���͂��B�������ACOM�Ȃǂ�DLL�̒���thread������Ă���ꍇ�Ȃǂ��ׂĂ̏ꍇ�ɑΏ�����̂͌��\��������B
	if (((unsigned long)_edx) >= 0x70000000)
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

		//BaseThreadInitThunk �̃R�[�h�̈ꕔ �v�����[�O����
		mov         edi,edi  
        push        ebp  
        mov         ebp,esp  

		//���̃��[�`���ɔ�΂�.
		jmp         eax 
//74AB338F  jne         @BaseThreadInitThunk@12+15h (74AB620Ah)    �����ɔ�΂�.
	}
}
SEXYHOOK_END_AS() g_BaseThreadInitThunk;




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
                "BUTTON", "�o���A�[!!", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 100, 300, 60, hWnd, (HMENU)ID_BARRIER, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_BARRIER:
				//API�t�b�N�ɂ��o���A��W�J����.
				g_BaseThreadInitThunk.Hook();

				SetWindowText(g_BarrierButtonWindow , "�o���A�[��W�J��");
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

