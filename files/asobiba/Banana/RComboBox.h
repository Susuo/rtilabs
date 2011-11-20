// RComboBox.h: RComboBox �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_)
#define AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindowBase.h"

class RComboBox : public RWindowBase  
{
public:
	RComboBox();
	virtual ~RComboBox();

	//�쐬. �I�[�o�[���C�h
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�A�C�e���̒ǉ�.
	bool AddItem(const string & inString);
	//�A�C�e���̑}��.
	bool InsertItem(int inItem,const string & inString);
	//�A�C�e���̕ύX.
	bool SetItem(int inItem,const string & inString);
	//�A�C�e���̎擾
	const string GetItem(int inItem);

	//���ݑI������Ă���Ƃ���̎擾.
	int	getNowSelected() const;

	//�w�肵���ꏊ��I��������
	void Select(int inItem);

	//�A�C�e���̐��̎擾
	int GetItemCount() const;
	//�A�C�e���̍폜
	bool DeleteItem(int inItem);
	//�A�C�e���̍폜
	void DeleteItemAll();
};

#endif // !defined(AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_)
