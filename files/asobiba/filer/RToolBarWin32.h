// RToolBarWin32.h: RToolBarWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTOOLBARWIN32_H__3804F5C4_C979_4407_B170_1428CD5DB8ED__INCLUDED_)
#define AFX_RTOOLBARWIN32_H__3804F5C4_C979_4407_B170_1428CD5DB8ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RToolBar.h"
#include "RToolBarControl.h"
#include "RTableControl.h"

class RToolBarWin32  : public RToolBar
{
public:
	RToolBarWin32(FilerSub* inFilerSub);
	virtual ~RToolBarWin32();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize();

	//�E�B���h�E�v���V�[�W��.
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	RToolBarControl					ToolBar;
	RTableControl<RToolBarWin32>	Table;
};

#endif // !defined(AFX_RTOOLBARWIN32_H__3804F5C4_C979_4407_B170_1428CD5DB8ED__INCLUDED_)
