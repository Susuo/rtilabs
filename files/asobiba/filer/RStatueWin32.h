// RStatueWin32.h: RStatueWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_)
#define AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStatue.h"
#include "RStatueControl.h"
#include "RTableControl.h"
#include "RProgressBar.h"

class RStatueWin32 : public RStatue
{
public:
	RStatueWin32(FilerSub* inFilerSub);
	virtual ~RStatueWin32();

	//�X�e�[�^�X�̃Z�b�g.
	virtual void setStatue(string inStatue);
	//�v���O���X�o�[
	virtual void setProgressBar(int inParsent);

	//�E�B���h�E�v���V�[�W��.
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize();

private:
	RStatueControl		StatueControl;
	RTableControl<RStatueWin32>	Table;

	//�v���O���X�o�[
	RProgressBar		ProgressBar;

};

#endif // !defined(AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_)
