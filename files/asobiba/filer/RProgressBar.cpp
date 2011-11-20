// RProgressBar.cpp: RProgressBar �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RProgressBar.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RProgressBar::RProgressBar()
{
	BockSize = 5;
	OrignalExtends = 0;
}

RProgressBar::~RProgressBar()
{

}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RProgressBar::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,"msctls_progress32",isParentWindow,inID, inFlg | WS_CHILD | WS_VISIBLE ,0);

	//��[���p�p�A�T�u�N���X�ɂ����Ⴄ���[
	this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
	SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WndProcStatic);

	setRange(0,100);
}





//�E�B���h�E�v���V�[�W��(none static)
LRESULT RProgressBar::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint( hwnd , &ps);
			OnPaint(ps.hdc);
			::EndPaint(hwnd , &ps);
			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		{
			if (!(OrignalExtends & ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE))
			{
				break;
			}
			if (msg == WM_MOUSEMOVE && wparam != MK_LBUTTON)
			{
				break;
			}
			RECT rc;
			this->GetWindowRect(&rc);
			if (getStyle() & PBS_VERTICAL)
			{
				const int     yPos    = HIWORD(lparam);
				setPos( (End - Start) - ((End - Start) * yPos / (rc.bottom - rc.top))  );
			}
			else
			{
				const int     xPos    = LOWORD(lparam);
				setPos( (End - Start) * xPos / (rc.right - rc.left)  );
			}
			DWORD p = ::GetWindowLong(this->getWindowHandle() , GWL_ID);

			SendMessage( ::GetParent( this->getWindowHandle() ) , WM_COMMAND , 
				MAKEWPARAM(::GetWindowLong(this->getWindowHandle() , GWL_ID) , PB_ORIGNAL_EXTENDS_MOUSE_DOWN) ,
				(LPARAM)this->getWindowHandle() );
		}
	}
	return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
}

void RProgressBar::OnPaint(HDC hdc)
{
	RECT ClientRC;
	this->GetClientRect( &ClientRC );

	//�X�^�[�g����I���܂ł̍������߂�.
	int r = (GetRValue(EndColor) - GetRValue(StartColor));
	int g = (GetGValue(EndColor) - GetGValue(StartColor));
	int b = (GetBValue(EndColor) - GetBValue(StartColor));

	int colorStep;
	if (getStyle() & PBS_VERTICAL)		colorStep	=	ClientRC.bottom;
	else								colorStep	=	ClientRC.right;

	//�X�e�b�v
	float rStep = (float)r/colorStep;
	float gStep = (float)g/colorStep;
	float bStep = (float)b/colorStep;

	COLORREF colorFill;
	RECT	rc = ClientRC;
	if (getStyle() & PBS_VERTICAL)
	{
		//�J���[���ЂƂς��Ƃ��A���A���ł����i�ނ�.
		int barSize = getBarSize(ClientRC.bottom);
		for(int i=1 ; i < barSize ; i+= BockSize)
		{
			colorFill = RGB(r + (int)(i * rStep),
							g + (int)(i * gStep),
							b + (int)(i * bStep));

			rc.top = ClientRC.bottom - (i-BockSize);
			rc.bottom = ClientRC.bottom - i;
			FillRectWithSolidBrush(hdc , &rc , colorFill);
		}
		//�Ō�̈ʒu�u���b�N��h��Ԃ�.
		rc.top = ClientRC.bottom - (barSize-BockSize);
		rc.bottom = ClientRC.bottom - barSize;
		FillRectWithSolidBrush(hdc , &rc , colorFill);
	}
	else
	{
		//�J���[���ЂƂς��Ƃ��A���A���ł����i�ނ�.
		int barSize = getBarSize(ClientRC.right);
		for(int i=0 ; i < barSize ; i+= BockSize)
		{
			colorFill = RGB(r + (int)(i * rStep),
							g + (int)(i * gStep),
							b + (int)(i * bStep));

			rc.left = ClientRC.left + (i-BockSize);
			rc.right = ClientRC.left + i;
			FillRectWithSolidBrush(hdc , &rc , colorFill);
		}
		//�Ō�̈ʒu�u���b�N��h��Ԃ�.
		rc.left = ClientRC.left + (barSize-BockSize);
		rc.right = ClientRC.left + barSize;
		FillRectWithSolidBrush(hdc , &rc , colorFill);
	}

	return ;
}

int RProgressBar::getBarSize(int inFullSize)
{
	const int range = End - Start;
	int pos= getPos();
	return (inFullSize * pos / range);
}

