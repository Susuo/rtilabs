#ifndef ___GAMEMAINH
#define ___GAMEMAINH
#include <.\VCL\rti_vcl_form.h>

#define FULLWINDOW_STYLE	(WS_POPUP  | WS_SYSMENU)
#define WINDOW_STYLE		(WS_CAPTION | WS_SYSMENU)

#define ABUOUT_LEVEL1 1
#define ABUOUT_LEVEL2 2
#define ABUOUT_LEVEL3 3
#define SUCCESS_MITION 0

char main_init(HWND hwnd);
void main_rtn(HWND hwnd);
void main_destory(HWND hwnd);
void main_scroll(HWND hwnd,int ScrollFlg,short sUdLr,short pos);

//���C���t�H�[��
extern TForm* GameForm;
//�Q�[�������ꍇ�� WinMain �ł�.
//�ʓ|�Ȃ̂ŁA��̊֐��ɂ��Ă��܂��܂�.
//�Ȃ��A Application �́A�Œ����Ă���Ă�ł�������.
int GameMain(HINSTANCE hInstance , HINSTANCE hPrevInstance,
			 LPSTR lpszArgs , int nWinMode, int ScreenX,int ScreenY);

#endif
