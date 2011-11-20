// MenuSentence.cpp: MenuSentence �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "MenuSentence.h"
#include "Filer.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

StringMap	MenuSentence::RunnbleScripts;


MenuSentence::MenuSentence()
{
	PopupMenu = ::CreatePopupMenu();
}
MenuSentence::~MenuSentence()
{
	::DestroyMenu(PopupMenu);
}


//�m�[�h���쐬.
void MenuSentence::CreateNode(string inLabel , string inScript)
{
	ASSERT(PopupMenu != NULL);

	//�E�B���h�E"�Ǘ�"�l
	int id = WindowsMain::getID();
	::AppendMenu(PopupMenu, MF_STRING, id , inLabel.c_str() );
	RunnbleScripts[id] = inScript;
}

//����q�ɂȂ��Ă���m�[�h�̍쐬.
void MenuSentence::CreateIncludeMenu(string inLabel , HMENU inNewPopup)
{
	ASSERT(PopupMenu != NULL);
	::AppendMenu(PopupMenu, MF_POPUP, (UINT_PTR)inNewPopup , inLabel.c_str() );
}

//�|�b�v�A�b�v.
HMENU MenuSentence::getMenu() const
{
	ASSERT(PopupMenu != NULL);
	return PopupMenu;
}
//���s
string MenuSentence::Run(int inID) const throw(RNotFoundException,RParseException)
{
	StringMap::const_iterator i = RunnbleScripts.find(inID);
	if (i == RunnbleScripts.end() ) throw RNotFoundException("�I�����ꂽ ID : %d �ɑΉ����Ă���X�N���v�g�͂���܂���",inID );

	RFunctionScript rsf;
	return rsf.Interpreter((*i).second);
}