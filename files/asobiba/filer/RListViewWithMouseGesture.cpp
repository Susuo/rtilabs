// RListViewWithMouseGesture.cpp: RListViewWithMouseGesture �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RListViewWithMouseGesture.h"
#include "filer.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RListViewWithMouseGesture::RListViewWithMouseGesture()
{

}

RListViewWithMouseGesture::~RListViewWithMouseGesture()
{

}

//�쐬.	�I�[�o�[���C�h
void RListViewWithMouseGesture::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RListView::Create(inINstance , isParentWindow , inID , inFlg);

	//�}�E�X�W�F�X�`���[�Ƀ^�[�Q�b�g�̃E�B���h�E��������.
	MouseGestureMouseCapture.setParentWnidow( this->getWindowHandle() ) ;
	//��[���p�p�A�T�u�N���X�ɂ����Ⴄ���[
	this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
	SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WndProcStatic);
}

//�E�B���h�E�v���V�[�W��(none static)
LRESULT RListViewWithMouseGesture::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		if ( MouseGestureMouseCapture.OnRButtonDown(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_RBUTTONUP:
		if ( MouseGestureMouseCapture.OnRButtonUp(LOWORD(lparam) , HIWORD(lparam)) ) return 0;

		//�W�F�X�`���[�Ɏ��s
		{
			const HWND win = this->getWindowHandle() ;
			const int id = ::GetWindowLong( win , GWL_ID);
			//WM_NOTIFY  --> NM_RCLICK  ���U��
			NMHDR nmhdr;
			nmhdr.code = NM_RCLICK;
			nmhdr.hwndFrom = win;
			nmhdr.idFrom = id;
			SendMessage( ::GetParent( win ) , WM_NOTIFY , (WPARAM) id ,(LPARAM)&nmhdr );
			return 0;
		}
		break;
	case WM_MOUSEMOVE:
		if ( MouseGestureMouseCapture.OnMouseMove(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		return 0;	//���Ȃ��Ƃǂ����ŃL���v�`���[������ςȂ��ɂȂ�?
		break;
	}
	return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
}
