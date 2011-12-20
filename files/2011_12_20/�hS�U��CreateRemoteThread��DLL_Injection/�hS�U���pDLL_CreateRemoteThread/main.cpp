#include <windows.h>

const int ID_MI = 1000;


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


BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{	
	HWND targetWindowhandle;
	switch(fdwReason)
    {
        case    DLL_PROCESS_ATTACH:
			targetWindowhandle = GetWindowHandle( ::GetCurrentProcessId() );
			if (targetWindowhandle)
			{
				//�����Â������.
				SetWindowText( GetDlgItem(targetWindowhandle , ID_MI) , "�Ȃ̂͊���" );
			}
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