// DisplayBlank.h: DisplayBlank �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_)
#define AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayBlank;
#include "comm.h"
#include "Display.h"

class DisplayBlank : public Display  
{
public:
	DisplayBlank(Window* inWindow,__RFILETYPE inNavigateType)
		: Display(inWindow,inNavigateType)
	{
	}
	virtual ~DisplayBlank()
	{
	}

	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_BLANK;
	}


	//�A�N�e�B���ɂȂ�܂���
	virtual void OnActive() const
	{
	}

	//��A�N�e�B���ɂȂ�܂���
	virtual void OnNoActive() const
	{
	}

	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems) 
	{
	}

	//���݃J�[�\��������t�@�C�������擾���܂��B
	virtual string getActiveFilename() const 
	{
		return "";
	}

	//���݃}�[�N����Ă���t�@�C����n���܂��B
	virtual StringList getMarkFilenames() const 
	{
		return StringList();	//����Ȃ��̂͂Ȃ�
	}

	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	virtual void GetWindowRect(RECT * outRC) const
	{
		ZeroMemory(outRC , sizeof(RECT) );
	}

	//�w�肵�����j���[���|�b�v�A�b�v����.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//���ƂŎ�������.
	}

	//�^�C�g�����擾����
	virtual string getTitle() const 
	{
		return "";
	}
	//�\��������̂̍쐬.
	virtual	void CreateShowObject(const PathInterface* inPath) 
	{
	}

protected:
	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum() 
	{
	}

};

#endif // !defined(AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_)
