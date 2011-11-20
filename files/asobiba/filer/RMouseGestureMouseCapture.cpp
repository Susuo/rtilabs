// RMouseGestureMouseCapture.cpp: RMouseGestureMouseCapture �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMouseGestureMouseCapture.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMouseGestureMouseCapture::RMouseGestureMouseCapture()
{
	this->LineSize = 5;
	this->LineColor = RGB( 0 , 0 , 255);
	Allowance = 10;

	ParentWnidow = NULL;

	State = NO_CAPTURE;
}

RMouseGestureMouseCapture::~RMouseGestureMouseCapture()
{

}



//�E�{�^���������ꂽ
bool RMouseGestureMouseCapture::OnRButtonDown(short x,short y)
{
	ASSERT(ParentWnidow != NULL);

	//�O��̌��ʂ��N���A.
	MousePoints.Clear();
	//�O�Ղ�ǉ�.
	MousePoints.Insert( x, y);
	//�O��̋O�Ղ�o�^.
	LastPos.x = x;
	LastPos.y = y;

	//�L���v�`��������.
	State = PRE_CAPTURE;

	puts("mouse in");

	return true;
}

//�E�{�^���������ꂽ
bool RMouseGestureMouseCapture::OnRButtonUp(short x,short y)
{
	ASSERT(ParentWnidow != NULL);

	//�L���v�`����~
	State = NO_CAPTURE;

	if(	GetCapture()==ParentWnidow)
	{
		puts("mouse out ... ok");

		// �}�E�X�����
		ReleaseCapture();

#if _DEBUG
		//�f�B�X�N�g�b�v�Ɍ��ʂ̐��������܂��B
		POINT base = {0};
		::ClientToScreen(this->ParentWnidow , &base);
		MOUSE_GESTURE_MOUSE_POINTS points = MousePoints.getPoints();
		int size = points.size() - 1;
		for(int i = 0; i < size ; i++)
		{
			DrawLine(	points[i].x + base.x, points[i].y + base.y,
						points[i+1].x + base.x, points[i+1].y + base.y,LineSize , RGB(255,i*10,0) );
		}

#endif
		::InvalidateRect(0,NULL,TRUE);
		::InvalidateRect(this->ParentWnidow,NULL,TRUE);

//		return true;
		return false;
	}
	return false;
}

//�}�E�X���ړ�����
bool RMouseGestureMouseCapture::OnMouseMove(short x,short y)
{
	ASSERT(this->ParentWnidow != NULL);

	//�L���v�`���ҋ@?
	if (State == PRE_CAPTURE)
	{
		//�L���v�`���[���Ă����̂�������A�L���v�`�����܂��B
		if ( IsCaptureOK(x,y) )
		{
			// �}�E�X���L���v�`��
			SetCapture(ParentWnidow);
			State = NOW_CAPTURE;
		}
	}

	if( GetCapture() == this->ParentWnidow )
	{
		puts("mouse move ... ok");

		//�O�Ղ�ǉ�.
		MousePoints.Insert(x,y);

		//�X�N���[�����W�ɕύX.
		POINT base = {0};
		::ClientToScreen(this->ParentWnidow , &base);

		//�O��̋O�Ղ��獡��̋O�Ղ܂ł�`��.
		DrawLine(LastPos.x + base.x, LastPos.y + base.y, x + base.x, y + base.y, LineSize , LineColor);

		LastPos.x = x;
		LastPos.y = y;
		return true;
	}
	return false;
}

//�O��̋O�Ղ��獡��̋O�Ղ܂ł�`��. static 
void RMouseGestureMouseCapture::DrawLine(int sx,int sy, int ex,int ey , int size , COLORREF color)
{
	// �f�X�N�g�b�v�ɂ���̂� HDC �擾
	HDC hScreenDc = GetDC( 0 );

	printf("(%d,%d)-(%d,%d)\n" , sx , sy , ex , ey);

	//���������܂��B�B�B���[�߂�ǂ�.
	//�y�������
	HPEN pen = ::CreatePen(PS_SOLID , size , color);
	HPEN oldpen = (HPEN) ::SelectObject(hScreenDc , pen);
	//�ʒu���ړ�����
	::MoveToEx(hScreenDc , sx , sy , NULL);
	//�҂�[
	::LineTo(hScreenDc,ex,ey);
	//�ȑO�̂��̂ɖ߂���
	::SelectObject(hScreenDc , oldpen);
	//�����܂����ƁB
	DeleteObject(pen);

	ReleaseDC(0,hScreenDc);
}


//�L���v�`�����J�n���Ă悩��?
bool RMouseGestureMouseCapture::IsCaptureOK(short x,short y)
{
	return (abs(LastPos.x - x) >= Allowance ||  abs(LastPos.y - y) >= Allowance);
}
