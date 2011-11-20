// Window.h: Window �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_)
#define AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Window;

#include "comm.h"


#include "Rfiles.h"
#include "Display.h"
#include "RTaskManager.h"
#include "FilerSub.h"
#include "AddressHistory.h"

//��E�B���h�E
class Window
{
private:
	//�t�@�C���ꗗ�Ǘ�
	Rfiles*		Files;
	//�f�B�X�v���C
	Display*	Disp;
	//�e��FilerSub.
	FilerSub*	ParentFSub;
	//�A�h���X�̗����Ǘ�
	AddressHistory	History;
	//�T�[�`���ʂ��}�X�N����
	string			Mask;
	//���O�̃p�X
	string			LastPath;

public:
	//���ɒu�����A�E�ɒu���������肵�܂��B
	Window(FilerSub* inFilerSub,string inPath);
	virtual ~Window();

	//�A�b�v�f�[�g���܂��B
	void UpdateWindow(Rfiles* inFiles , Display* inDisp);

	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	void GetWindowRect(RECT * outRC) const;

	//���ɂ����?
	bool getIsLeft() const;
	//�A�N�e�B���ɂȂ�܂���.
	void OnActive();
	//��A�N�e�B���ɂȂ�܂���.
	void OnNoActive() const;
	//�f�B���N�g������オ��.
	void UpDirectory();
	//��O�ɐi�߂��?
	bool IsHistoryNext() const			{	return History.IsNext();	}

	//��O�ɖ߂��?
	bool IsHistoryBack() const			{	return History.IsBack();	}

	//�v���r���[���ă_����������f�B���N�g���Ƃ��ăX�L����.
	bool PreviewOrLs(string inPath,bool isNOTRecodeHistory = false)
	{
		//���O�̃p�X��ۑ�
		this->LastPath = this->getNowPath();
		//�v���r���[���ă_����������f�B���N�g���Ƃ��ăX�L����.
		if ( Preview(inPath,isNOTRecodeHistory) )	return true;
		if ( ls(inPath,isNOTRecodeHistory) )	return true;
		return false;
	}

	//���݂̃f�B�X�v���C��ł̃p�X���ύX�ɂȂ�܂����B
	//���Ƃ��΃u���E�U�Ń����N���N���b�N���ꂽ�Ƃ��A
	//���݂̃f�B�X�v���C���g���ăp�X�ƃq�X�g���[�����L�^���܂��B
	void OnQuickChange(string inPath,bool isNOTRecodeHistory = false);
	//�^�C�g�����ύX���ꂽ�ꍇ
	void OnTitleChange();

	//���ݕێ����Ă���t�@�C���V�X�e���ł̌��݃p�X���擾.
	string getNowPath() const
	{
		if (this->Files) return this->Files->getPath();
		return "";
	}
	//�^�C�g���̎擾.
	string getTitle() const;

	//���݃J�[�\��������t�@�C�������擾���܂��B
	string getActiveFilename() const
	{
		return Disp->getActiveFilename();
	}
	//���݃}�[�N����Ă���t�@�C����n���܂��B
	StringList getMarkFilenames() const
	{
		return Disp->getMarkFilenames();
	}
	//�ǂݒ���.
	void Refresh() 
	{
		PreviewOrLs( getNowPath() );
	}
	//�w�肵�����j���[���|�b�v�A�b�v����.
	string PopupMenu(const string inMenuName) throw(RWin32Exception,RNoComplateException,RNotFoundException)
	{
		return Disp->PopupMenu(inMenuName);
	}
	//�}�X�N�̎擾
	string getMask() const
		{		return this->Mask;	}
	//�}�X�N���Z�b�g����.
	void setMask(const string inMask)
		{		this->Mask = inMask;	}

	//���������L���Ă���^�X�N�����ׂĂƂ߂܂��B
	void StopTasks() const;

	//�߂�!
	void HistoryBack();
	//�i��!
	void HistoryNext();

	//��O�̃p�X���擾���܂��B
	string getLastPath() const
		{		return this->LastPath;	}

private:
	//�p�X�œ����r���[���[�Ńv���r���[���܂��B
	bool Preview(string inPath,bool isNOTRecodeHistory = false) ;
	//�f�B���N�g���X�L����.
	bool ls(string inPath,bool isNOTRecodeHistory = false) ;

};

#endif // !defined(AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_)
