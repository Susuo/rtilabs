// RSetupLoad.h: RSetupLoad �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_)
#define AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RCommKeyDown.h"
#include "MenuSentence.h"
#include "RNotFoundException.h"
#include "RParseException.h"
#include "RWin32Exception.h"

//�W���̃L�[�o�C�h�A�}�E�X�_�E���A�c�[���o�[�Ȃǂ̏ꏊ.
const string FILER_DEFAULT_DEFINE_FILENAME = "filer.def";

typedef map<string ,MenuSentence*>	OneMenu;

class RSetupLoad  
{
public:
	RSetupLoad();
	virtual ~RSetupLoad();

	//�ǂݍ���.
	void Load(string inSetupFilename,string inSection) throw(RIOCannotOpenException,RIOCannotReadException,RParseException);

	//�L�[�_�E��.
	bool KeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey) const throw (RParseException);
	//�L�[�_�E��.
	bool KeyDown(unsigned char inKeyCode ) const throw (RParseException);

	//���j���[���o��.
	string Popup(const string inMenuName, POINT inScreenPoint , HWND inOwner) const throw (RParseException,RNotFoundException,RWin32Exception);

private:
	//�L�[�R�[�h�̕ϊ�
	unsigned char ToKey(string inKeyname) const;

private:
	RCommKeyDown	KeyDowns;
	OneMenu			Menus;
};

#endif // !defined(AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_)
