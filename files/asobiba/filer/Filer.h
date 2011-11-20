// Filer.h: Filer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_)
#define AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Filer;
#include "comm.h"
#include "FilerSub.h"
#include "RTaskView.h"

//���C���N���X.
class Filer
{
private:
	RTaskView*	TaskView;

private:
	Filer();
public:
	virtual ~Filer();

	//singleton
	static Filer* getFiler();
	//TaskView�̎擾.
	RTaskView* getTaskView() const
	{
		ASSERT(TaskView != NULL);
		return TaskView;
	}
	//���E�̃E�B���h�E���擾.
	FilerSub* getFilerSub(bool IsLeft) const
	{
		if (IsLeft)	return Left;
		return		Right;
	}
	//�����Ƀt�H�[�J�X������܂���?
	bool IsLeftFocus() const;

	//�E�B���h�E�����T�C�Y���ꂽ�Ƃ��ɌĂ΂��
	void OnWindowResize();
	//�L�[�������ꂽ�Ƃ�
	void OnKeyDown(unsigned char inKey);
	//�J�n����
	void OnCreate();
	//�I������
	void OnDestory();
	//�t�H�[�J�X�����E�B���h�E���ړ������Ƃ��ɌĂ΂��.
	void OnChangeFocus(bool isLeftWindow);
	//�L���ȃE�B���h�E���ǂ������ׂ�.
	bool IsValidWindow(const Window* inWin);

	//�t�H�[�J�X���ς�������Ƃ�ʒm.
	void OnChangeFocus();

	
private:
	//���̃E�B���h�E.
	FilerSub*	Left;
	//�E�̃E�B���h�E.
	FilerSub*	Right;
	//���E�B���h�E���t�H�[�J�X�������Ă���.
	bool		LeftWindowFocus;

};

#endif // !defined(AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_)
