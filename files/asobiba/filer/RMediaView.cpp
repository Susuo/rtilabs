// RMediaView.cpp: RMediaView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMediaView.h"

#define	ID_MEDIA_START			1	//�Đ�
#define	ID_MEDIA_STOP			2	//��~
#define	ID_MEDIA_FASTFORWARD	3	//������
#define	ID_MEDIA_FASTREWIND		4	//�����߂�
#define	ID_MEDIA_FORWARD		5	//�ŏ���
#define	ID_MEDIA_REWIND			6	//�I����

#define ID_MEDIA_SEEN			11	//�V�[��
#define ID_MEDIA_VOLUM			12	//�{�����[��

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMediaView::RMediaView()
{
}

RMediaView::~RMediaView()
{
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RMediaView::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{

    static WNDCLASS	wc = {0};
	if (! wc.lpszClassName )
	{
		wc.lpszClassName = "MediaControl";
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)WndProcStatic;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = inINstance;
		wc.hIcon         = NULL;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName  = NULL;
		if(!RegisterClass(&wc))
		{
			throw RDoNotInitaliseException("�E�B���h�E�N���X�̍쐬�Ɏ��s���܂���");
		}

	}

	RWindow::Create(inINstance,wc.lpszClassName,isParentWindow,inID,
		inFlg | WS_EX_ACCEPTFILES | WS_CLIPCHILDREN ,0);

	//��[���p�p�A�T�u�N���X�ɂ����Ⴄ���[
	SetWindowLong(this->getWindowHandle() , GWL_USERDATA , (long)this);

	try
	{
		//�ݒ�̃��[�h
		SetupLoad.Load(FILER_DEFAULT_DEFINE_FILENAME,"MediaView");
	}
	catch(RException e)
	{
		printf("�ݒ胍�[�h�Ɏ��s���܂���:%s \n",e.getMessage() );
	}


	//�����A�݂Ă��Ȃ��̂ŁA�{�^�����쐬
	Start.Create(inINstance , this->getWindowHandle() , ID_MEDIA_START , WS_VISIBLE);
	Stop.Create(inINstance , this->getWindowHandle() , ID_MEDIA_STOP , WS_VISIBLE);
	FastForward.Create(inINstance , this->getWindowHandle() , ID_MEDIA_FASTFORWARD , WS_VISIBLE);
	FastRewind.Create(inINstance , this->getWindowHandle() , ID_MEDIA_FASTREWIND , WS_VISIBLE);
	Forward.Create(inINstance , this->getWindowHandle() , ID_MEDIA_FORWARD , WS_VISIBLE);
	Rewind.Create(inINstance , this->getWindowHandle() , ID_MEDIA_REWIND , WS_VISIBLE);

	Seen.Create(inINstance , this->getWindowHandle() , ID_MEDIA_SEEN , WS_VISIBLE);
	Volum.Create(inINstance , this->getWindowHandle() , ID_MEDIA_VOLUM , WS_VISIBLE | PBS_VERTICAL);

	//���܂��͖{���ɂ���Ō����ڂ��E�́E�j��!!�Ƃ������Ă���̂���.
	Start.CreateImage("resource\\�Đ�.bmp" , true);
	Stop.CreateImage("resource\\��~.bmp", true);
	FastForward.CreateImage("resource\\������.bmp", true);
	FastRewind.CreateImage("resource\\�����߂�.bmp", true);
	Forward.CreateImage("resource\\�ŏ���.bmp", true);
	Rewind.CreateImage("resource\\�Ō��.bmp", true);

	//�E���Ƃ��Ă���̂ŊG�̃T�C�Y�Ƀ{�^���̃T�C�Y�����킹�܂�,
	Start.FitPictureTo();
	Stop.FitPictureTo();
	FastForward.FitPictureTo();
	FastRewind.FitPictureTo();
	Forward.FitPictureTo();
	Rewind.FitPictureTo();

	Seen.setRange(0,255);
	Volum.setRange(0,255);

	Seen.setGradationRange(RGB(255,0,0) , RGB(0,255,0) );
	Volum.setGradationRange(RGB(0,255,0) , RGB(255,0,0));

	Seen.setPos(0);
	Volum.setPos(0);

	Seen.setOrignalExtendsStyle(ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE);
	Volum.setOrignalExtendsStyle(ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE);

}
//���[�h
bool RMediaView::Load(const string inFilename , HINSTANCE inINstance )
{
	if ( ! Media.Load(inFilename , inINstance , this->getWindowHandle() ) ) return false;

	const RWindow* win = Media.getWindow();
	win->MoveWindow(10,10,300,300);

	Seen.setPos(0);
	Volum.setPos( Media.getVolum() );

	Media.Play();
	//�g���b�N�o�[��i�߂邽�߂̃^�C�}�[
	Timer.Set( this->getWindowHandle() , 0, 1000);
	return true;
}

//�E�B���h�E�v���V�[�W��(none static)
LRESULT RMediaView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_COMMAND:
		{
			int p = LOWORD(wparam);
			switch (LOWORD(wparam)) 
			{
			case ID_MEDIA_START:
				Media.Play();
				Media.setVolum( Volum.getPos() );
				break;
			case ID_MEDIA_STOP:
				Media.Stop();
				break;
			case ID_MEDIA_FASTFORWARD:
				Media.setPos( Seen.getPos() + 2);
				break;
			case ID_MEDIA_FASTREWIND:
				Media.setPos( Seen.getPos() - 2);
				break;
			case ID_MEDIA_FORWARD:
				Media.setPos( 255);
				break;
			case ID_MEDIA_REWIND:
				Media.setPos( 0);
				break;
			case ID_MEDIA_SEEN:
				if ( HIWORD(wparam) == PB_ORIGNAL_EXTENDS_MOUSE_DOWN)
				{
					Media.setPos( Seen.getPos() );
				}
				break;
			case ID_MEDIA_VOLUM:
				if ( HIWORD(wparam) == PB_ORIGNAL_EXTENDS_MOUSE_DOWN)
				{
					Media.setVolum( Volum.getPos() );
				}
				break;
			default:
				return FALSE; 
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint( hwnd , &ps);
//			this->Draw(ps.hdc);
			::EndPaint(hwnd , &ps);
			return 0;
		}
		break;
	case WM_TIMER:
		Seen.setPos( Media.getPos() );
		printf("%d/%d\n" , Media.getPos() , 255);
		break;
	case WM_SIZE:
		{
			RECT clientRect;
			this->GetClientRect(&clientRect);

			//�{�^����z�u�A���ꂪ�ʂ̗��ݕ�.
			Start.MoveWindow		(100	,clientRect.bottom - 50);
			Stop.MoveWindow			(150	,clientRect.bottom - 50);
			FastForward.MoveWindow	(200	,clientRect.bottom - 50);
			FastRewind.MoveWindow	(250	,clientRect.bottom - 50);
			Forward.MoveWindow		(300	,clientRect.bottom - 50);
			Rewind.MoveWindow		(350	,clientRect.bottom - 50);
			Seen.MoveWindow			(50,clientRect.bottom - 100			, 350 , 10);
			Volum.MoveWindow		(50,clientRect.bottom - 100 + 20	, 10 , 50 );
		}
		break;
	case WM_KEYDOWN:
		try
		{
			this->SetupLoad.KeyDown(wparam);
		}
		catch(RParseException e)
		{
			printf("�G���[���������܂���:\n%s\n", e.getMessage() );
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;

			switch(nmhdr->code)
			{
			case LVN_KEYDOWN:
				{
					puts("hit");
				}
				break;
			}
		}
		break;
	}
	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}

//�^�C�g�����擾����
string RMediaView::getTitle() const 
{
	return Media.getTitle();
}

