// RImageButton.cpp: RImageButton �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImageButton.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RImageButton::RImageButton()
{

}

RImageButton::~RImageButton()
{

}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RImageButton::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,"BUTTON",isParentWindow,inID, inFlg | WS_CHILD ,0);

	//��[���p�p�A�T�u�N���X�ɂ����Ⴄ���[
	this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
	SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WndProcStatic);
}

//�C���[�W�̃Z�b�g
bool RImageButton::CreateImage(const string inFilename , bool isRegion)
{
	LoadImage.Release();
	return LoadImage.Load(inFilename) ;
}

//�G�̑傫���ɃE�B���h�E�����킹��.
void RImageButton::FitPictureTo()
{
	this->ResizeWindow( LoadImage.getContentsWidth() , LoadImage.getContentsHeight());
}


//�E�B���h�E�v���V�[�W��(none static)
LRESULT RImageButton::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint( hwnd , &ps);
			BitBlt(ps.hdc ,
				ps.rcPaint.left,ps.rcPaint.top, 
				ps.rcPaint.right  - ps.rcPaint.left ,
				ps.rcPaint.bottom - ps.rcPaint.top ,
				LoadImage.getHDC(),
				ps.rcPaint.left,
				ps.rcPaint.top,
				SRCCOPY);
			::EndPaint(hwnd , &ps);
			return 0;
		}

	case BM_SETSTATE:
		{
			HDC dc = GetDC(hwnd);
			if (wparam == 1)
			{
				BitBlt(dc ,
					0 , 0, 
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					LoadImage.getHDC(),
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					BLACKNESS);
			}
			else
			{
				BitBlt(dc ,
					0 , 0, 
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					LoadImage.getHDC(),
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					SRCCOPY);
			}
			//return 0;
		}

	}
	return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
}
