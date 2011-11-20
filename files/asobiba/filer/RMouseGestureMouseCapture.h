// RMouseGestureMouseCapture.h: RMouseGestureMouseCapture �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_)
#define AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMouseGestureMousePoints.h"

//�}�E�X�̓������L���v�`���[
class RMouseGestureMouseCapture  
{
	enum MouseCaptureState
	{
		NO_CAPTURE,
		PRE_CAPTURE,
		NOW_CAPTURE,
	};
public:
	RMouseGestureMouseCapture();
	virtual ~RMouseGestureMouseCapture();

	//�E�{�^���������ꂽ	short �ɂ��Ȃ��ƕ����̊֌W�ł͂܂�.
	bool OnRButtonDown(short x,short y);
	//�E�{�^���������ꂽ
	bool OnRButtonUp(short x,short y);
	//�}�E�X���ړ�����
	bool OnMouseMove(short x,short y);

	//�|�C���g���擾
	//�C�����C���W�J�������������.
	const MOUSE_GESTURE_MOUSE_POINTS	getPoints() const
	{
		return MousePoints.getPoints();
	}

//�A�N�Z�b�T
	//�e�E�B���h�E
	void setParentWnidow(HWND inParentWnidow)
	{
		ASSERT(ParentWnidow == NULL);
		ASSERT(inParentWnidow != NULL);
		ParentWnidow = inParentWnidow;
	}


private:
	//�O��̋O�Ղ��獡��̋O�Ղ܂ł�`��.
	static void DrawLine(int sx,int sy, int ex,int ey , int size , COLORREF color) ;

	//�L���v�`�����J�n���Ă悩��?
	bool IsCaptureOK(short x,short y);

private:
	//�O�Ղ̃T�C�Y
	int							LineSize;
	//�O�Ղ̐F
	COLORREF					LineColor;
	//�L���v�`���J�n�܂ł̗V��
	int							Allowance;
	//�e�E�B���h�E.
	HWND						ParentWnidow;

	POINT						LastPos;
	RMouseGestureMousePoints	MousePoints;

	MouseCaptureState			State;
};

#endif // !defined(AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_)
