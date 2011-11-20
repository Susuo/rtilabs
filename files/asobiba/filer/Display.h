// Display.h: Display �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_)
#define AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Display;
class Window;
class PathInterface;
#include "comm.h"
#include "Rfile.h"
#include "RFixedColumnArray.h"
#include "StringList.h"
#include "ReferenceCount.h"

#include "RWin32Exception.h"
#include "RNoComplateException.h"
#include "RNotFoundException.h"

enum __DISPLAYTYPE
{
	__DISPLAYTYPE_DEBUG,
	__DISPLAYTYPE_BLANK,
	__DISPLAYTYPE_LISTVIEW_REPORT,
	__DISPLAYTYPE_LISTVIEW_BIGICON,
	__DISPLAYTYPE_PREVIEW_TEXT,
	__DISPLAYTYPE_PREVIEW_BITMAP,
	__DISPLAYTYPE_PREVIEW_WEBBROWSER,
	__DISPLAYTYPE_PREVIEW_MEDIA,
	__DISPLAYTYPE_PREVIEW_PDF,
	__DISPLAYTYPE_PREVIEW_NISEOLE,
};

class Display  : public ReferenceCount
{
protected:
	//���̃N���X���Ǘ�����Ă���E�B���h�E.
	Window*				Win;
	//�ǂ̃J�������ǂ��ɔz�u���邩�����߂�N���X.
	RFixedColumnArray	FixedColumn;
	//���ݕ\�����Ă��� RFILE �̎��
	__RFILETYPE			NavigateType;
	//�Q�ƃJ�E���g.
	int					UseCount;
protected:
	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum() = 0;
public:
	Display(Window* inWindow,__RFILETYPE inNavigateType) : ReferenceCount()
	{
		this->Win = inWindow;
		this->NavigateType = inNavigateType;
	}
protected:
	virtual ~Display()
	{
	}

public:
	//�J�����̃R�s�[.
	void setFixedCloumn(const RFixedColumn* inReadOnlyColumnes)
	{
		this->FixedColumn.setFixedCloumn(inReadOnlyColumnes);
		//�Œ�J�����̏�Ԃ��ς����
		this->OnChangeFixedColum();
	}

	//���ݕ\�����Ă���@RFILE �̎�ނ�Ԃ��܂��B
	__RFILETYPE getNowNavigateType() const
	{
		return NavigateType;
	}
	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const = 0;

	//�A�N�e�B���ɂȂ�܂���
	virtual void OnActive() const = 0;

	//��A�N�e�B���ɂȂ�܂���
	virtual void OnNoActive() const = 0;

	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems) = 0;

	//���݃J�[�\��������t�@�C�������擾���܂��B
	virtual string getActiveFilename() const = 0;

	//���݃}�[�N����Ă���t�@�C����n���܂��B
	virtual StringList getMarkFilenames() const = 0;

	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	virtual void GetWindowRect(RECT * outRC) const = 0;

	//�w�肵�����j���[���|�b�v�A�b�v����.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) = 0;

	//�^�C�g�����擾����
	virtual string getTitle() const = 0;

	//�\��������̂̍쐬.
	virtual	void CreateShowObject(const PathInterface* inPath) = 0;

};

#include "Window.h"
#include "PathInterface.h"


#endif // !defined(AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_)
