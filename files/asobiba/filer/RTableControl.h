// RTableControl.h: RTableControl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_)
#define AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"

template<class _CallBackClass>
class RTableControl : public RWindow  
{
public:
	RTableControl()
	{
	}
	virtual ~RTableControl()
	{
	}

	typedef LRESULT (_CallBackClass::*CALLBACK_WNDPROC)(HWND,UINT,WPARAM,LPARAM);

	//�쐬
	void Create(HINSTANCE inINstance,HWND isParentWindow,
		int inID , DWORD inFlg,
		_CallBackClass* inThis,CALLBACK_WNDPROC inCallback) throw()
	{
		//�쐬.
		RWindow::Create(inINstance,"STATIC",isParentWindow,inID,inFlg,WS_EX_TRANSPARENT);
		//�R�[���o�b�N��̕ۑ�
		this->This = inThis;
		this->Callback = inCallback;
		//�T�u�N���X��.
		this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
		SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
		SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WindowProcStatic);
	}
	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{	//������ static �ł͂Ȃ���ɏ��n����B
		RTableControl* classRet = 
			(RTableControl*) GetWindowLong( hwnd , GWL_USERDATA );
		if (classRet)
		{	//�w�肳�ꂽ�N���X�̊֐����R�[���o�b�N�f�X!!
			return (classRet->This->*(classRet->Callback))(hwnd,msg,wparam,lparam);
		}
		return DefWindowProc(hwnd , msg , wparam,lparam);
	}
	
	//OrignalWindowProc ���ĂԁB
	LRESULT CallWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
	}

private:
	//�I���W�i����Window�v���V�[�W��
	WNDPROC				OrignalWindowProc;
	CALLBACK_WNDPROC	Callback;
	_CallBackClass*		This;

};

#endif // !defined(AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_)
