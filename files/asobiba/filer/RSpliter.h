// RSpliter.h: RSpliter �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_)
#define AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



//���o�[�o���h
//�Q�l http://www7.big.or.jp/~hina/zidney/programing/sample/rubberband/
class RSpliter  
{
public:
	RSpliter();
	virtual ~RSpliter();

	//�Ђ���{�^���������ꂽ
	bool OnLButtonDown(int x,int y);
	//�Ђ���{�^���������ꂽ
	bool OnLButtonUp(int x,int y);
	//�}�E�X���ړ�����
	bool OnMouseMove(int x,int y);

	//�e�E�B���h�E
	void setParentWnidow(HWND inParentWnidow);
	//���o�[�o���h�̃T�C�Y�̐ݒ�
	void setRubberBandSize(int inRubberBandSize);
	//���E�B���h�E�̕��̐ݒ�
	void setLeftWindowSize(int inNewLeftWidth);
	//���E�B���h�E�̕��̐ݒ�
	void setLeftWindowSize(double inNewLeftParsent);

	//���o�[�����h�̍�/�E�̃T�C�Y�̎擾
	void getWindowRect(bool IsLeft,RECT* outRect) const;

private:
	// ���̃E�B���h�E�̃T�C�Y�𐳋K�����ĕԂ�
	int RegularWidth(int inWidth) const;
	//�X�v���b�^�[��`��.
	void DrawSpliter(int inWidth) const;

private:
	//�e�E�B���h�E.
	HWND		ParentWnidow;
	//���o�[�o���h�̃T�C�Y.
	int			RubberBandSize;
	//���E�B���h�E�̃T�C�Y.
	int			LeftWindowSize;

	// �X�v���b�^�[��ō��{�^���������ꂽ�Ƃ��̈ʒu
	int			MouseDiff;
	// �O��X�v���b�^�[�����������W
	int			OldWidth;
};

#endif // !defined(AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_)
