// RScrollBar.h: RScrollBar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_)
#define AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindowBase.h"

class RScrollBar  : public RWindowBase  
{
public:
	RScrollBar();
	virtual ~RScrollBar();

	//�쐬. �I�[�o�[���C�h
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//��Ԃ̃Z�b�g.
	int SetScrollInfo(int inFlg ,const SCROLLINFO * inInfo , BOOL inRedraw );
	//��Ԃ̎擾.
	int GetScrollInfo(int inFlg ,SCROLLINFO * outInfo ) const;

//�T�[�r�X.
	//���݈ʒu�̎擾
	int getPos(int inFlg) const;
	//�ʒu�̐ݒ�.
	void setPos(int inPos , int inFlg) ;
	//���ۂ̃X�N���[��(WM_VSCROLL / WM_HSCROLL) �����������Ƃ��� �X�N���[���o�[�̈ʒu�𒲐����܂�.
	//true ���A���Ă����ꍇ�́A �����ɓ��ꂽ ioTargetScrollInfo �� SetScrollInfo �Őݒ肷��Ƃ����ł��傤.
	bool CalsScroll(SCROLLINFO * ioTargetScrollInfo , WPARAM inWParam) const;

};

#endif // !defined(AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_)
