/*
SetWindowHook ���s�����߂� �t�b�N�pDLL
*/
#include <Windows.h>

HINSTANCE g_DLLInstance = NULL;
HHOOK g_Hook = NULL;
UINT WM_OREORE_HOOK = NULL;
const int ID_MI = 1000;



BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{	
	
	switch(fdwReason)
    {
        case    DLL_PROCESS_ATTACH:
			g_DLLInstance = hinstDLL;
			WM_OREORE_HOOK = ::RegisterWindowMessage("WM_OREORE_HOOK");
			break;

        case    DLL_PROCESS_DETACH:
            break;

        case    DLL_THREAD_ATTACH:
            break;

        case    DLL_THREAD_DETACH:
            break;
    }
    return  TRUE;
}

//�t�b�N�ŌĂяo�����R�[���o�b�N
//�^�[�Q�b�g�̃v���Z�X�̒��ŌĂяo�����B
LRESULT CALLBACK HookProc(UINT code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* cw = ((CWPSTRUCT*)lParam);
	if (cw->message == WM_OREORE_HOOK)
	{
		//�����Â������.
		SetWindowText( GetDlgItem(cw->hwnd , ID_MI) , "�Ȃ̂͊���" );

		//�����ƃt�b�N���Â��Ă��������A�^�[�Q�b�g�̕��ׂ����炷���߂ɂ����Ńt�b�N�𔲂�.
		::UnhookWindowsHookEx( g_Hook );
		g_Hook = NULL;
	}
	return ::CallNextHookEx(g_Hook, code, wParam, lParam);
}

//�t�b�N�̃X�^�[�g
extern "C" __declspec(dllexport)  int  InjectDll( HWND targetwindow )
{	
	g_Hook = SetWindowsHookEx( WH_CALLWNDPROC
		,(HOOKPROC)HookProc
		,g_DLLInstance
		,GetWindowThreadProcessId(targetwindow,NULL)
		);
	if (g_Hook == NULL)
	{
		return false;
	}
	//���b�Z�[�W�𓊂��āA�t�b�N���ł̍ŏ��̏���������.
	SendMessage(targetwindow,WM_OREORE_HOOK , 0 , 0);
	return true;
}

