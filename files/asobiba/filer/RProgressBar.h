// RProgressBar.h: RProgressBar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_)
#define AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"


//real:len = full:x �� x �����߂�.
#define CHANGE_SCALE(real,len,full)	( (real) * (full) / (len) )
//real:len = full:x �� real �����߂�.
#define REV_CHANGE_SCALE(x,len,full)	( (x) * (len) / (full) )


#define ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE	0x01	//�}�E�X�ŃN���b�N�����ʒu�ɗe�ʂ����킹��

#define PB_ORIGNAL_EXTENDS_MOUSE_DOWN	(100+1)							//ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE�̒ʒm.

class RProgressBar : public RWindow  
{
public:
	RProgressBar();
	virtual ~RProgressBar();
public:
	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�ő�l�����߂�.
	void setRange(int inStart , int inEnd)
	{
		Start = inStart;
		End = inEnd;
		SendMessage( this->getWindowHandle() , PBM_SETRANGE , 0 , MAKELPARAM(inStart, inEnd)  ) ;
	}
	//���݂̈ʒu���擾.
	int getPos()
	{
		return SendMessage( this->getWindowHandle() , PBM_GETPOS , 0 , 0 ) ;
	}
	//�ʒu��ݒ�.
	void setPos(int inNewPos)
	{
		SendMessage( this->getWindowHandle() , PBM_SETPOS , (WPARAM) inNewPos , 0 ) ;
	}
	//���݂̈ʒu���炱�񂾂��ړ�������.
	void DeltaPos(int inIncrement)
	{
		SendMessage( this->getWindowHandle() , PBM_DELTAPOS , (WPARAM) inIncrement  , 0 ) ;
	}
	//�J�n�̐F�ƏI���̐F�����߂�.
	void setGradationRange(COLORREF inStartColor,COLORREF inEndColor)
	{
		StartColor = inStartColor;
		EndColor = inEndColor;
	}
	//�O���f�[�V����������P�ʂ��擾
	//�傫������΂���قǁ@�݂��͗����܂������x�͑����Ȃ�܂��B
	void setBockSize(unsigned int inBockSize)
	{
		ASSERT(inBockSize >= 0);
		BockSize = inBockSize;
	}
	//�I���W�i���g���X�^�C���̐ݒ�
	void setOrignalExtendsStyle(DWORD inStyle)
	{
		OrignalExtends = inStyle;
	}
	//�I���W�i���g���X�^�C���̎擾
	DWORD getOrignalExtendsStyle() const
	{
		return OrignalExtends;
	}
private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		RProgressBar* p = (RProgressBar*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void OnPaint(HDC hdc);
	//�o�[�̃T�C�Y�̎擾.
	int getBarSize(int inFullSize);

	//�w�肳�ꂽ�J���[���g���� inRC ��h��Ԃ�.
	void FillRectWithSolidBrush(HDC hdc , const RECT * inRC , COLORREF colorFill)
	{
		HBRUSH brush = ::CreateSolidBrush(colorFill);
		::FillRect(hdc, inRC , brush);
		::DeleteObject(brush);
	}

private:
	//�T�u�N���X��.
	WNDPROC			OrignalWindowProc;
private:
	COLORREF			StartColor;
	COLORREF			EndColor;
	int					Start;
	int					End;

	unsigned int		BockSize;
	DWORD				OrignalExtends;

};

#endif // !defined(AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_)
