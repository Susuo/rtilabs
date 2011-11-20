// RTaskViewWin32.h: RTaskViewWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_)
#define AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RTaskView.h"
#include "RListView.h"
#include "RLock.h"


class RTaskViewWin32  : public RTaskView
{
public:
	RTaskViewWin32() throw();
	virtual ~RTaskViewWin32();


	//�V�����^�X�N��ǉ����܂��B
	void InsertTask(unsigned long inID,string inMessage);
	//�i���̍X�V
	void UpdateGoing(unsigned long inID,int inParsent);
	//�^�X�N�̍폜
	void DeleteTask(unsigned long inID);
	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnSize() ;
private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		RTaskViewWin32* This = (RTaskViewWin32*)GetWindowLong( hwnd , GWL_USERDATA);
		if (This != NULL ) return This->DlgProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��.
	LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�E�B���h�E�n���h��
	HWND	WindowHandle;
	//�^�X�N��\�����郊�X�g�r���[
	RListView	TaskList;
	//���b�N.
	RLock	Lock;
};

#endif // !defined(AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_)
