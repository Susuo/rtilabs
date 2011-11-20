// DisplayWebBrowserView.h: DisplayWebBrowserView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYWEBBROWSERVIEW_H__6AA188E4_9FF7_4041_9449_3615EE2B780E__INCLUDED_)
#define AFX_DISPLAYWEBBROWSERVIEW_H__6AA188E4_9FF7_4041_9449_3615EE2B780E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayWebBrowserView;

#include "comm.h"
#include "Display.h"
#include "WndProcCallable.h"
#include "RWebBrowser.h"
#include "RfilePreviewOne.h"

class DisplayWebBrowserView  :
	public Display,				//�f�B�X�v���C�͂�����p�����Ȃ��Ă͂Ȃ�Ȃ��B DisplayTextView �� Display�ł���B
	private WndProcCallable		//WndProc���󂯎�鎖���ł��� Inteface�N���X.
{
public:
	DisplayWebBrowserView(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayWebBrowserView();
	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_PREVIEW_WEBBROWSER;
	}

	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum()
	{
	}

	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//�A�N�e�B���ɂȂ�܂���
	virtual void OnActive() const;

	//��A�N�e�B���ɂȂ�܂���
	virtual void OnNoActive() const;

	//���݃J�[�\��������t�@�C�������擾���܂��B
	virtual string getActiveFilename() const 
	{
		return this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);
	}

	//���݃}�[�N����Ă���t�@�C����n���܂��B
	virtual StringList getMarkFilenames() const 
	{
		return StringList();	//����Ȃ��̂͂Ȃ�
	}
	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	virtual void GetWindowRect(RECT * outRC) const
	{
		WebBrowser.GetWindowRect(outRC);
	}
	//�w�肵�����j���[���|�b�v�A�b�v����.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//���ƂŎ�������.
	}

	//�����炩��w�������t�@�C���ȊO�̃t�@�C�����I�[�v�����悤�Ƃ��ꂽ�ꍇ
	//�u���E�U�̏ꍇ�A�ʂ̃����N�𓥂񂾏ꍇ�ł�
	void OnQuickChange(string inPath);
	//�^�C�g�����ύX���ꂽ�ꍇ
	void OnTitleChange();

	//
	const RWebBrowser*	getBrowser() const
	{
		return &WebBrowser;
	}
	//�^�C�g�����擾����
	virtual string getTitle() const 
	{
		return WebBrowser.getTitle();
	}
	//�\��������̂̍쐬.
	virtual	void CreateShowObject(const PathInterface* inPath) 
	{
	}

protected:
	//WindowProc���󂯎��
	//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
	//�����͏������Ȃ��z�� false ��Ԃ��B
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet);
private:
	//�E�B���h�E�����T�C�Y����
	void WindowResize() const;
	//�ʏ�N���b�N�Ƃ��A���^�[���L�[�Ƃ��ł̃A�N�V����
	void NormalAction();
	//��O�̃f�B���N�g���ɖ߂�
	void ReturnAction();

	const Rfile* RfileOne;
	RWebBrowser	WebBrowser;
};

#endif // !defined(AFX_DISPLAYWEBBROWSERVIEW_H__6AA188E4_9FF7_4041_9449_3615EE2B780E__INCLUDED_)
