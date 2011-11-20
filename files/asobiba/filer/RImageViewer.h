// RImageViewer.h: RImageViewer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_)
#define AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNormalWindow.h"
#include "RImage.h"
#include "RImageFilter.h"
#include "RSetupLoad.h"

class RImageViewer  : public RNormalWindow
{
public:
	RImageViewer();
	virtual ~RImageViewer();

	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�G�̃��[�h
	bool Load(const string inFilename);

	//�^�C�g�����擾����
	string getTitle() const ;
private:
	//�E�B���h�E�v���V�[�W��(none static)
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	//�����X�N���[���o�[�͂���܂���?
	bool IsVSrcollBar() const;
	//�����X�N���[���o�[�͂���܂���?
	bool IsHSrcollBar() const;

private:
	//�ݒ�̓ǂݍ���
	void ReloadSetting(string inReadName);
	//�T�C�Y�ύX����܂����B
	void OnSize();
	//�X�N���[���̌v�Z
	void OnScroll(int inSw,int inFlg,int inValue);
	//�`��
	void OnDraw(HDC inPaintDC) const;

	//�L�[�������ꂽ�Ƃ��̏���
	void OnKeyDown(unsigned char inKeyCode ) const;
protected:
	//�X�N���[���̎d�؂�Ȃ���.
	void				Reset();
private:
	int					WindowWidth;
	int					WindowHeight;
	SCROLLINFO			ScrollInfoX;
	SCROLLINFO			ScrollInfoY;

	RImage				LoadImage;

	//�ݒ�t�@�C�����烍�[�h�ł��܂��B
	RSetupLoad			SetupLoad;

};

#endif // !defined(AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_)
