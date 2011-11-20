// WndProcCallable.h: WndProcCallable �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_)
#define AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//interface class
class WndProcCallable  
{
protected:
	WndProcCallable()
	{
	}
	virtual ~WndProcCallable()
	{
	}
public:
	//WindowProc���󂯎��
	//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
	//�����͏������Ȃ��z�� false ��Ԃ��B
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet) = 0;
};

#endif // !defined(AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_)
