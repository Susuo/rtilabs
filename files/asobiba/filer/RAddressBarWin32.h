// RAddressBarWin32.h: RAddressBarWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_)
#define AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RAddressBar.h"
#include "WindowsMain.h"


class RAddressBarWin32 : public RAddressBar  
{
public:
	RAddressBarWin32(FilerSub* inFilerSub);
	virtual ~RAddressBarWin32();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize() ;
	//�p�X���ύX���ꂽ�Ƃ��ɌĂяo�����.
	virtual void OnAddressChange(string inNewPath);
private:

	//�w�肵���p�X���{������.
	void PathView(string inPath);
	//�V�����E�B���h�E�Ńp�X���{������.
	void NewView(string inPath);

	//�E�B���h�E�v���V�[�W��.(static)
	static LRESULT CALLBACK AddressWindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{	//������ static �ł͂Ȃ���ɏ��n����B
		RAddressBarWin32* classRet = 
			(RAddressBarWin32*) GetWindowLong( hwnd , GWL_USERDATA );
		if (classRet)
		{
			return classRet->AddressWindowProc(hwnd,msg,wparam,lparam);
		}
		return DefWindowProc(hwnd , msg , wparam,lparam);
	}
	//�E�B���h�E�v���V�[�W��.
	LRESULT CALLBACK AddressWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND		WindowHandle;
	HWND		EditHandle;

	//Edit�̃I���W�i���̃E�B���h�E�v���V�[�W��.
	WNDPROC		AddressOrignalWindowProc;
};

#endif // !defined(AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_)
