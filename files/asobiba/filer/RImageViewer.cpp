// RImageViewer.cpp: RImageViewer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImageViewer.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RImageViewer::RImageViewer()
{
}

RImageViewer::~RImageViewer()
{
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RImageViewer::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	RNormalWindow::Create(inINstance,isParentWindow,inID,
		inFlg | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL ,0);

	try
	{
		//�ݒ�̃��[�h
		SetupLoad.Load(FILER_DEFAULT_DEFINE_FILENAME,"ImageView");
	}
	catch(RException e)
	{
		printf("�ݒ胍�[�h�Ɏ��s���܂���:%s \n",e.getMessage() );
	}


}

//�G�̃��[�h
bool RImageViewer::Load(const string inFilename)
{
	if ( ! LoadImage.Load(inFilename) ) return false;
	//�X�N���[���̎d�؂�Ȃ���.
	Reset();

	return true;
}



//�E�B���h�E�v���V�[�W��(none static)
LRESULT RImageViewer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint( hwnd , &ps);
			this->OnDraw(ps.hdc);
			::EndPaint(hwnd , &ps);
			return 0;
		}
	case WM_SIZE:
		OnSize();
		break;
	case WM_VSCROLL:
		OnScroll(WM_VSCROLL , LOWORD(wparam) , HIWORD(wparam));
		break;
	case WM_HSCROLL:
		OnScroll(WM_HSCROLL , LOWORD(wparam) , HIWORD(wparam));
		break;
	case WM_KEYDOWN:
		OnKeyDown(wparam);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}



//�����X�N���[���o�[�͂���܂���?
bool RImageViewer::IsHSrcollBar() const
{
	return (WindowWidth < LoadImage.getContentsWidth());
}

//�����X�N���[���o�[�͂���܂���?
bool RImageViewer::IsVSrcollBar() const
{
	return (WindowHeight < LoadImage.getContentsHeight() );
}

//�X�N���[���̎d�؂�Ȃ���.
void RImageViewer::Reset()
{
	RECT baseWindowRC ;
	this->GetClientRect( &baseWindowRC)			;
	this->WindowWidth = baseWindowRC.right;
	this->WindowHeight = baseWindowRC.bottom;

	ScrollInfoY.nPos = 0;
	ScrollInfoX.nPos = 0;

	OnSize();
}

//�T�C�Y�ύX
void RImageViewer::OnSize()
{
	RECT rc;
	this->GetClientRect( &rc);
	this->WindowWidth = rc.right;
	this->WindowHeight = rc.bottom;

	//V
	{
		int dep = LoadImage.getContentsHeight() - rc.bottom;
		ScrollInfoY.cbSize = sizeof(SCROLLINFO);
		ScrollInfoY.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		ScrollInfoY.nMin = 0;
		ScrollInfoY.nMax = dep;
//		ScrollInfoY.nPos = ScrollY;

		if (rc.bottom == 0)	ScrollInfoY.nPage = 0;
		else ScrollInfoY.nPage = LoadImage.getContentsHeight() / rc.bottom;

		SetScrollInfo(this->getWindowHandle(), SB_VERT, &ScrollInfoY, TRUE); 
	}
	//H
	{
		int dep = LoadImage.getContentsWidth() - rc.right;
		ScrollInfoX.cbSize = sizeof(SCROLLINFO);
		ScrollInfoX.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		ScrollInfoX.nMin = 0;
		ScrollInfoX.nMax = dep;
//		ScrollInfoX.nPos = ScrollX;

		if (rc.right == 0)	ScrollInfoX.nPage = 0;
		else ScrollInfoX.nPage = LoadImage.getContentsWidth() / rc.right;

		SetScrollInfo(this->getWindowHandle(), SB_HORZ, &ScrollInfoX, TRUE); 
	}
}

//�X�N���[���̌v�Z
void RImageViewer::OnScroll(int inSw,int inFlg,int inValue)
{
	SCROLLINFO &si  = (inSw == WM_HSCROLL) ? ScrollInfoX : ScrollInfoY;

	int val;
	switch(inFlg)
	{
		case SB_LINEUP:
			val = -1;
			break;
		case SB_LINEDOWN:
			val = 1;
			break;
		case SB_PAGEUP:
			val = -1 * si.nPage;
			break;
		case SB_PAGEDOWN:
			val = si.nPage;
			break;
		case SB_THUMBTRACK:
			val = inValue - si.nPos;
			break;
		case SB_TOP:
//		case SB_LEFT:
			val = si.nMin - si.nPos;
			break;
		case SB_BOTTOM:
//		case SB_RIGHT:
			val = si.nMax - si.nPos;
			break;
		default:
			val = 0;
			break; 
	}
	val = max(-1 * si.nPos, min(val, si.nMax - si.nPos));
	if (val != 0)
	{
		si.nPos += val;
		if (inSw == WM_HSCROLL)
		{
			SetScrollInfo(this->getWindowHandle(), SB_HORZ , &si, TRUE);
		}
		else
		{
			SetScrollInfo(this->getWindowHandle(), SB_VERT , &si, TRUE);
		}
		::InvalidateRect( this->getWindowHandle() , NULL , FALSE);
	} 
}



//�`��
void RImageViewer::OnDraw(HDC inPaintDC) const
{
	BitBlt(inPaintDC ,0,0, 
		WindowWidth ,
		WindowHeight ,
		LoadImage.getHDC(),
		ScrollInfoX.nPos,
		ScrollInfoY.nPos,
		SRCCOPY);
}

	//�^�C�g�����擾����
string RImageViewer::getTitle() const 
{
	return LoadImage.getTitle();
}

//�L�[�������ꂽ�Ƃ��̏���
void RImageViewer::OnKeyDown(unsigned char inKeyCode ) const
{
	const HWND hwnd = this->getWindowHandle();

	switch (inKeyCode) 
	{
	case '1':
		{
			RImageFilter filter;
			filter.Create(&LoadImage);
			break;
		}
	case VK_UP:
		if ( IsVSrcollBar() )
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)	SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			else								SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
		}
		break;
	case VK_DOWN:
		if ( IsVSrcollBar() )
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)	SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			else								SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
		}
		break;
	case VK_RIGHT:
		if ( IsHSrcollBar() )
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)	SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);
			else								SendMessage(hwnd, WM_HSCROLL, MAKELONG(SB_LINEDOWN, 0), 0);
		}
		break;
	case VK_LEFT:
		if ( IsHSrcollBar() )
			if (GetKeyState(VK_SHIFT) & 0x8000)	SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0);
			else								SendMessage(hwnd, WM_HSCROLL, MAKELONG(SB_LINEUP, 0), 0);
		break;
	case VK_PRIOR:
		if ( IsVSrcollBar() ) SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0); 
		break;
	case VK_NEXT:
		if ( IsVSrcollBar() ) SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0); 
		break; 
	default:
		try
		{
			this->SetupLoad.KeyDown(inKeyCode);
		}
		catch(RParseException e)
		{
			printf("�G���[���������܂���:\n%s\n", e.getMessage() );
		}
		break;
	}
	return ;
}
