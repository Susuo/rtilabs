// RListView.h: RListView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_)
#define AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RListView;

#include "comm.h"
#include "RWindowBase.h"

class RListView  : public RWindowBase
{
public:
	RListView();
	virtual ~RListView();

	//�쐬. �I�[�o�[���C�h
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//�g�����ڂ̃Z�b�g.
	void setExtended(DWORD inStyle);
	//�g�����ڂ̃Z�b�g.
	DWORD getExtended();
	//���z���X�g�̏ꍇ�̍��ڐ��̐ݒ�
	void 	SetItemCountEx(int inCount,DWORD inFlg);
	//���z���X�g�̏ꍇ�̍��ڐ��̐ݒ�
	void 	SetItemCount(int inCount);

	//�J���������ׂč폜����.
	void ClearColumn();
	//�J�����A�C�e���̒ǉ�.
	bool InsertColumn(int inItem,int inSubItem,char* inString,int inWidth,UINT inMask,int inFmt);

	//�A�C�e���̒ǉ� / �ύX.
	bool SetItem(int inItem,int inSubItem,const char* inString);
	//�A�C�e���̒ǉ� / �ύX.
	bool SetItem(int inItem,int inSubItem,int inInt);
	//�A�C�e���̎擾
	string GetItem(int inItem,int inSubitem);

	//���ݑI������Ă���Ƃ���̎擾.
	int	getNowSelected() const;

	//�w�肵���A�C�e���̏�Ԃ�ύX���܂��B
	void SetItemState(int inItem , int inState,int inMask);
	//�w�肵���A�C�e���̏�Ԃ��擾���܂��B
	int GetItemState(int inItem,int inMask) const;

	//�A�C�e���̐��̎擾
	int GetItemCount() const;
	//�A�C�e���̍폜
	bool DeleteItem(int inItem);
	//�A�C�e���̍폜
	void DeleteItemAll();

	//�C���[�W���X�g�Z�b�g.
	bool setImageList(const HIMAGELIST inImageLIst, int inType);

	//�A�C�e���Ƃ̃q�b�g�e�X�g
	int HitTest(const POINT inPoint) const;

	//���ׂĂ̑I��������.
	void ResetSelect();

	//���݃}�E�X�J�[�\�����̂������Ă���A�C�e�����擾���܂��B
	int HitItem() const;
};

#endif // !defined(AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_)
