/*
������̃A�v���P�[�V�����ł��B

*/
#include <windows.h>
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //���C���E�B���h�E
const char * MYNAME = "�hM�����_Anti_DLL_Injection_LoadLibraryA";


//LoadLibraryA��ی삷��.
SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
			("kernel32.dll","LoadLibraryA"),( const char * lpLibFileName ))
{

	//�hS�����DLL�������[�h�����Ȃ�.
	if ( strstr(lpLibFileName , "�hS" ))
	{
		//�hS���ē����Ă��邩��A�hS�����dll���낤����A���[�h����.
		MessageBox( g_MainWnd , "�hS���񂩂�DLL��}�����܂ꂻ���ɂȂ����̂Ŗh�䂵�܂���" , "�o���A�[" , 0);

		return 1;
	}
	//����ȊO�̓��[�h����.
	//�S����DLL�����ۂ��Ă͂����Ȃ��B
	//gdiplus �� winsock �Ȃǒx���ǂݍ��݂����API�����邽�߁A������܂ŎE���Ă��܂��B

	//�ꎞ�I�Ƀt�b�N������
	SEXYHOOK_UNHOOK();
	//���̊֐����Ăяo��
	return CallOrignalFunction(lpLibFileName);
}
SEXYHOOK_END_AS() g_LoadLibraryAHook;




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
				g_LoadLibraryAHook.Hook();

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

