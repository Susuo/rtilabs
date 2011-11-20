// DisplayListViewBigIconWin32.h: DisplayListViewBigIconWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_)
#define AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayListViewBigIconWin32;

#include "comm.h"
#include "Display.h"
#include "WindowsMain.h"
#include "RListView.h"
#include "RfileLocalReport.h"
#include "RListViewWithMouseGesture.h"
#include "WndProcCallable.h"
#include "RSetupLoad.h"

class DisplayListViewBigIconWin32  : 
	public Display,				//�f�B�X�v���C�͂�����p�����Ȃ��Ă͂Ȃ�Ȃ��B DisplayListViewBigIconWin32 �� Display�ł���B
	private WndProcCallable		//WndProc���󂯎�鎖���ł��� Inteface�N���X.
{
public:
	DisplayListViewBigIconWin32(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayListViewBigIconWin32();

	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum();
	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_LISTVIEW_BIGICON;
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
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RParseException,RNotFoundException) ;

	//�^�C�g�����擾����
	virtual string getTitle() const 
	{
		return "";	//�^�C�g���Ȃ�ĂȂ��ł�.
	}

	//�\��������̂̍쐬.
	virtual	void CreateShowObject(const PathInterface* inPath) ;


protected:
	//WindowProc���󂯎��
	//��������������ꍇ�� true ��Ԃ��B �߂�l�� outRet �ɑ������B
	//�����͏������Ȃ��z�� false ��Ԃ��B
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet);
private:
	//�L�[�������ꂽ�Ƃ��̏���
	bool OnKeyDown(unsigned char inKeyCode  , int inModifyKey ) const;
	//�E�B���h�E�𔼕��̈ʒu�Ɉړ�����.
	void HarfWindow() const;
	//�_�u���N���b�N
	bool OnDoubleClick() const;
	//�J�����N���b�N
	bool OnColumnClick(LPDRAWITEMSTRUCT inItem) const;
	//�E�N���b�N
	bool OnRClick(const NMHDR * inNmhdr) ;

	bool OnGetDispInfo(NMLVDISPINFO* inInfo) const;

	bool OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw ) const;

	const Rfile** RfileItems;
	int			RfileItemsSize;

	RListViewWithMouseGesture	ListView;

	//�ݒ�t�@�C�����烍�[�h�ł��܂��B
	RSetupLoad			SetupLoad;

};

#endif // !defined(AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_)
