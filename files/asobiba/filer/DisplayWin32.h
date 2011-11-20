// DisplayWin32.h: DisplayWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_)
#define AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayWin32;
#include "comm.h"
#include "Display.h"
#include "WindowsMain.h"
#include "RListViewWithMouseGesture.h"
#include "RfileLocalReport.h"
#include "WndProcCallable.h"

class DisplayWin32  :
	public Display,				//�f�B�X�v���C�͂�����p�����Ȃ��Ă͂Ȃ�Ȃ��B DisplayListViewBigIconWin32 �� Display�ł���B
	private WndProcCallable		//WndProc���󂯎�鎖���ł��� Inteface�N���X.
{
public:
	DisplayWin32(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayWin32();

	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum();
	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_LISTVIEW_REPORT;
	}
	//�A�N�e�B���ɂȂ�܂���
	virtual void OnActive() const;

	//��A�N�e�B���ɂȂ�܂���
	virtual void OnNoActive() const;

	//���݃J�[�\��������t�@�C�������擾���܂��B
	virtual string getActiveFilename() const ;

	//���݃}�[�N����Ă���t�@�C����n���܂��B
	virtual StringList getMarkFilenames() const ;

	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	virtual void GetWindowRect(RECT * outRC) const
	{
		ListView.GetWindowRect(outRC);
	}
	//�w�肵�����j���[���|�b�v�A�b�v����.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//���ƂŎ�������.
	}
	//�^�C�g�����擾����
	virtual string getTitle() const 
	{
		return "";	//�^�C�g���Ȃ�ĂȂ��ł�.
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
	//�E�B���h�E�𔼕��̈ʒu�Ɉړ�����.
	void HarfWindow() const;
	//�_�u���N���b�N
	bool OnDoubleClick() ;
	//�J�����N���b�N
	bool OnColumnClick(LPDRAWITEMSTRUCT inItem) ;
	//�L�[���Ȃ񂩉�����܂���
	bool OnKeyDown(LPNMKEY inKey) ;
	bool OnGetDispInfo(NMLVDISPINFO* inInfo);
	bool OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw );

	const Rfile** RfileItems;
	int			RfileItemsSize;
	RListViewWithMouseGesture	ListView;
};

#endif // !defined(AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_)
