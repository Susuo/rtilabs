// BasePanel.h: BasePanel �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_)
#define AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "WndProcCallable.h"
#include "RDoNotInitaliseException.h"


class BasePanel
{
public:
	BasePanel();
	virtual ~BasePanel();

	//�E�B���h�E�̍쐬
	//���s����RDoNotInitaliseException��O.
	void Create(bool isLeft) throw();
	//WndProc�̏����̉������ɓo�^����.
	void registeCallbackWndProc(WndProcCallable * inProc)
	{
		this->WndProcCallOnList.push_back(inProc);
	}
	//WndProc�̏����̉������̉���
	void UnregisteCallbackWndProc(WndProcCallable * inProc)
	{
		for ( WNDPROCCALLABLE::iterator it = WndProcCallOnList.begin() ; it != WndProcCallOnList.end() ; ++it)
		{
			if ( (*it) == inProc) 
			{
				WndProcCallOnList.erase(it);
				return ;
			}
		}
	}
	//�ړ�.
	void MoveWindow(int x,int y,int width,int height)
	{
		::MoveWindow(getWindowHandle() , x ,y , width , height , TRUE);
	}
	HWND getWindowHandle() const
	{
		return WindowHandle;
	}
private:
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//������ static �ł͂Ȃ���ɏ��n����B
		BasePanel* p = (BasePanel*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//�E�B���h�E�v���V�[�W��(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//WndProc�̉���������]���郊�X�g.
	typedef deque<WndProcCallable*> WNDPROCCALLABLE;
	WNDPROCCALLABLE WndProcCallOnList;

	HWND		WindowHandle;

	WNDPROC		OrignalWndProc;
};

#endif // !defined(AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_)
