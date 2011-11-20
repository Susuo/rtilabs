// BUserDataEditDlg.h: BUserDataEditDlg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_)
#define AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "REdit.h"
#include "RComboBox.h"
#include "RDialog.h"
#include "RWin32Exception.h"
#include "RGISSmallUserData.h"	
#include "BConf.h"
#include "RGISSmallUserData.h"

class BBanana;


class BCategoListItem
{
	string	Name;
	int		No;
public:
	BCategoListItem()
	{
	}
	BCategoListItem(const string & inName, int inNo)
	{
		Create(inName , inNo);
	}
	void Create(const string & inName, int inNo)
	{
		Name = inName;
		No = inNo;
	}

	const string getName() const
	{
		return Name;
	}
	int getNo() const
	{
		return No;
	}
};

typedef newVector<BCategoListItem> VecBCategoListItems;

class BCategoList
{
	BCategoListItem	Label;
	VecBCategoListItems List;
public:
	BCategoList(const string & inName, int inNo)
	{
		Label.Create(inName , inNo);
	}
	//���X�g�ɒǉ�
	void Insert(const string & inName, int inNo)
	{
		List.push_back(new BCategoListItem(inName , inNo) );
	}
	//���X�g���̂��̂̎擾.
	const VecBCategoListItems * getList() const
	{
		return &List;
	}
	const BCategoListItem* getItem() const
	{
		return &Label;
	}
};


class BUserDataEditDlg  : public RDialog
{
public:
	BUserDataEditDlg();
	virtual ~BUserDataEditDlg();

	void BUserDataEditDlg::Create(BBanana	*	poolBanana,const RGISSmallShareImage* poolShareImage) throw(RWin32Exception);
	void BUserDataEditDlg::OnCancel();

	void Show(const LPOINT & inTargetPoint);
	void Show(const RGISSmallUserDataRead * inUserData);

	//�E�B���h�E�v���V�[�W��.
	LRESULT BUserDataEditDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	//�J�e�S���[�t�@�C���̓ǂݍ���.
	void BUserDataEditDlg::CategoFileRead() throw(RException);
	//�I�����ꂽ�J�e�S���̃T�u�J�e�S���� SubCatego �ɃZ�b�g���܂�
	void BUserDataEditDlg::SelectSubCatego();
	//���ݑI������Ă���J�e�S����ێ����Ă��� BCategoList ���擾
	const BCategoList* BUserDataEditDlg::getNowSelectCatego();
	//���ݑI������Ă���T�u�J�e�S����ێ����Ă��� BCategoListItem ���擾
	const BCategoListItem* BUserDataEditDlg::getNowSelectSubCatego(const BCategoList* inSelectCatego);
	//���[�U��`�f�[�^�̏�������
	void WriteData() throw(RException);
	//�폜
	void DeleteData() throw(RException);
	//�A�C�R���ύX�{�^���������ꂽ.
	void OnIconChangeButtom() throw(RWin32Exception);

	//�`��
	void BUserDataEditDlg::OnPaint();
private:
	REdit	Name;
	REdit	Comment;
	RComboBox	Catego;
	RComboBox	SubCatego;
	unsigned char IconNO;

	//�ҏW,�폜���郆�[�U�[�f�[�^�̃V���A�� (�V�K�쐬�̏ꍇ�� -1 )
	int		Serial;

	LPOINT	TargetPoint;

	newVector<BCategoList> List;

	BBanana * Banana;
	const RGISSmallShareImage* ShareImage;
};

#endif // !defined(AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_)
