#define TIME_CHECK		//���Ԍv�����s��
#define INTERVAL 110	//�t���[�����[�g(ms)

#include <rti_lib.h>
#pragma comment(lib,"Winmm.lib")

//�R�[���o�b�N.
static LRESULT CALLBACK GameProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam);
//�_�~�[�R�[���o�b�N.
static LRESULT CALLBACK DummyProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam);
//�A�N�e�B�u�t���O
static BOOL fActive = true;
//���C���t�H�[��
TForm *GameForm;

//�Q�[�������ꍇ�� WinMain �ł�.
//�ʓ|�Ȃ̂ŁA��̊֐��ɂ��Ă��܂��܂�.
//�Ȃ��A Application �́A�Œ����Ă���Ă�ł�������.
int GameMain(HINSTANCE hInstance , HINSTANCE hPrevInstance,LPSTR lpszArgs , int nWinMode,
			 int ScreenX,int ScreenY)
{
	MSG       msg;      //���b�Z�[�W�擾�p�̍\����
    HWND      hwnd;     //���C���E�B���h
	DEBUG("================================�����J�n==========================================");
    lookup_making(); //���b�N�A�b�v�e�[�u���쐬

    try
    {
	    if (lstrcmp(lpszArgs,"DDX")==0)
	    {   //�G��G���W���̃x�[�X�� DDX
			DDXAdoption();
	    }
		else
	    {
		    //�G��G���W���̃x�[�X�� GDI
			GDIAdoption();
	    }
	    //���C���t�H�[���쐬
	    GameForm = new TForm(NULL,"�ۂ育�� ��ׂ�",WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN	,
    		CW_USEDEFAULT,CW_USEDEFAULT,ScreenX,ScreenY,
	    	NULL,DummyProc);
        if (! GameForm)
                throw MemoryDeficient("GameMain","���C���t�H�[�� �쐬",sizeof(TForm) );
        if (! GameForm->GetKind() )
        {   
            delete GameForm; 
            throw FriendlyError("GameMain","���C���t�H�[�� �쐬","?","?" );
        }
	    hwnd = GameForm->GethWnd();
	    //�������́A����Ȃ��ƁA�������Ȍ��ʂɂȂ�. ���[��. �����낵��.
        ShowWindow(hwnd ,SW_SHOWDEFAULT);  //�E�B���h�쐬�J�n
        //��ʉ𑜓x�ύX
        GameForm->ReClasSize(ScreenX,ScreenY);
	    if (!CommDraw->Init(hwnd,ScreenX,ScreenY,24) )	
            throw FriendlyError("GameMain","�O���t�B�b�N�X�G���W��������","?","���̃��[�h�ł̋N���͖���" );
	    if (main_init(hwnd) != SUCCESS_MITION) 
        {
            main_destory(hwnd);
            throw FriendlyError("GameMain","���C������������","?","?" );
        }
		//�����ŁA���������I�������̂ŁA�����ȃv���V�[�W�����E�B���h�N�ɑ���.
		SetWindowLong(hwnd, GWL_WNDPROC,(long)GameProc);

	    UpdateWindow(hwnd);         //�E�B���h�\���J�n
	    SetFocus(hwnd);
	    //ShowCursor(0);//�}�E�X��\�����Ȃ�
    }
    catch(...)
    {
       	CommDraw->End();
        MSGOUT("�v���O�����ً̋}��~!!","�v���O�����̏������ɖ�肪����܂�.\n �v���O�������ً}��~���܂�.");
        return -1;
    }
    //���b�Z�[�W���[�v��
   	while( 1 )
    {
   		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    	{
	    	if( msg.message == WM_QUIT )	break;
		    TranslateMessage( &msg );
   			DispatchMessage( &msg );
        } 
	    else 
		{
			if (fActive)	PostMessage( hwnd, WM_TIMER, 0, 0 );
			else            WaitMessage();
		}
    }
  	CommDraw->End();
	return msg.wParam;
}

//�_�~�[�R�[���o�b�N.
//�Q�[�����������ɂ܂�Ȃ����b�Z�[�W���o�J(�Q�C�c��)�������Ă����̂ŁA
//�����������邽�߂Ƀ_�~�[�̃v���V�[�W��������Ă���.
LRESULT CALLBACK DummyProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:          //�E�B���h�̏I�����b�Z�[�W
		PostQuitMessage(0);   //�I�����b�Z�[�W�̔��s
		break;
	default:
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

//�R�[���o�b�N.
LRESULT CALLBACK GameProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam)
{
    unsigned long TempTimeGet;
	static unsigned long NowTime = 0;

	switch(message)
	{
	case WM_ACTIVATEAPP:
		fActive = (BOOL) wParam;	
		break;
	case WM_PAINT:             //�G�惁�b�Z�[�W
		Backsurface->OnPaint();
		break;
	case WM_MOVE:				//�E�B���h��������������Ƃ�.
		Backsurface->OnMove(lParam);
		break;
	case WM_MOUSEMOVE:     //�}�E�X���������ꂽ�Ƃ�
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
		if (wParam & MK_LBUTTON	) 
		{
              	        MouseEL  = true;    //�����ꑱ���Ă���𗧂Ă�
		}
		else
		{
			if (wParam & MK_RBUTTON	) 
			{
				        MouseER  = true;
			}
			else
			{
			 	MouseER  = false;
				MouseEL  = false;
			}
		}
		break;
	case WM_LBUTTONDOWN:   //���{�^��
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
        MouseCL  = true ;  //��񂾂��̃_�E���̃N���A
		if (wParam & MK_RBUTTON) MouseCR  = true ;  //��񂾂��̃_�E���̃N���A
		MouseEL  = false ;
		MouseER  = false ;
		break;
	case WM_RBUTTONDOWN:  //�E�{�^��
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
        MouseCR  = true ;  //��񂾂��̃_�E���̃N���A
		if (wParam & MK_LBUTTON) MouseCL  = true ;  //��񂾂��̃_�E���̃N���A
		MouseEL  = false ;
		MouseER  = false ;
		break;
	case WM_LBUTTONUP:   //���{�^��
		MouseEL  = false ;
		MouseCL = false;
		break;
	case WM_RBUTTONUP:  //�E�{�^��
		MouseER  = false ;
		MouseCR = false;
		break;

	case WM_TIMER:            //�^�C�}�[����[��
        TempTimeGet = timeGetTime();
		NOW_TIME(TempTimeGet - NowTime);	//�v��
		if (TempTimeGet - NowTime >= INTERVAL)
		{
			//1�t���[���i��
			NowTime = TempTimeGet;

			MousePX = MouseRX;
			MousePY = MouseRY;

			main_rtn(hwnd);

			MouseCR = false;
			MouseCL = false;
		}
		break;
	case WM_KEYDOWN:
		LastKey = (unsigned char) wParam;
		break;
    case WM_CLOSE:
		main_destory(hwnd);   //���C�����[�`���̔j��
		END_TIME(INTERVAL);	//�v���I��
        DestroyWindow(hwnd);
		break;
	case WM_DESTROY:          //�E�B���h�̏I�����b�Z�[�W
		PostQuitMessage(0);   //�I�����b�Z�[�W�̔��s
		break;
 	default:
        return Application->DefProc(hwnd,message,wParam,lParam);
	}
	return 0;
}
