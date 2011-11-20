// RComboBox.cpp: RComboBox �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RComboBox.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RComboBox::RComboBox()
{

}

RComboBox::~RComboBox()
{

}

//�A�C�e���̒ǉ�.
bool RComboBox::AddItem(const string & inString)
{
#ifndef _WIN32_WCE
	SendMessage( CB_ADDSTRING,0L,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_ADDSTRING,0L,(LPARAM)_A2W(inString.c_str() ) );
#endif
	return true;
}

//�A�C�e���̑}��.
bool RComboBox::InsertItem(int inItem,const string & inString)
{
#ifndef _WIN32_WCE
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)_A2W(inString.c_str()));
#endif
	return true;
}

//�A�C�e���̕ύX.
bool RComboBox::SetItem(int inItem,const string & inString)
{
	SendMessage( CB_DELETESTRING,(WPARAM)inItem,0L);
#ifndef _WIN32_WCE
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)_A2W(inString.c_str()));
#endif
	return true;
}

//�A�C�e���̎擾
const string RComboBox::GetItem(int inItem)
{
	//�I������Ă��镶����̒������擾.
	int size = SendMessage( CB_GETLBTEXTLEN,(WPARAM)inItem,0);
#ifndef _WIN32_WCE
	_myAutoPtr<char> buffer ;
	SendMessage( CB_GETLBTEXT,(WPARAM)inItem,(LPARAM)buffer.Alloc(size + 1));

	return buffer.getCore();
#else
	_myAutoPtr<unsigned short> buffer ;
	SendMessage( CB_GETLBTEXT,(WPARAM)inItem,(LPARAM)buffer.Alloc(size + 1) );

	MY_USES_CONVERSION;
	return _W2A( buffer.getCore() );
#endif
}



//���ݑI������Ă���Ƃ���̎擾.
int	RComboBox::getNowSelected() const
{
	return SendMessage( CB_GETCURSEL, 0L, 0L);
}

//�w�肵���ꏊ��I��������
void RComboBox::Select(int inItem)
{
	SendMessage( CB_SETCURSEL,(WPARAM)inItem,0L);
}

//�A�C�e���̐��̎擾
int RComboBox::GetItemCount() const
{
	return SendMessage( CB_GETCOUNT,0L,0L);
}

//�A�C�e���̍폜
bool RComboBox::DeleteItem(int inItem)
{
	SendMessage( CB_DELETESTRING,(WPARAM)inItem,0L);
	return true;
}

//�A�C�e���̍폜
void RComboBox::DeleteItemAll()
{
	SendMessage( CB_RESETCONTENT,0L,0L);
}

