// MenuSentence.h: MenuSentence �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_)
#define AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include "RFunctionScript.h"
#include "RNotFoundException.h"


class MenuSentence
{
public:
	MenuSentence();
	~MenuSentence();
	//�m�[�h���쐬.
	void CreateNode(string inLabel , string inScript);
	//����q�ɂȂ��Ă���m�[�h�̍쐬.
	void CreateIncludeMenu(string inLabel , HMENU inNewPopup);
	//�|�b�v�A�b�v.
	HMENU getMenu() const;
	//���s
	string Run(int inID) const throw(RNotFoundException,RParseException);
private:
	static StringMap	RunnbleScripts;
	HMENU		PopupMenu;
};


#endif // !defined(AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_)
