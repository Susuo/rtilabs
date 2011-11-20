// BUserDataIconDlg.h: BUserDataIconDlg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_)
#define AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RDialog.h"
#include "RGISSmallShareImage.h"
#include "RScrollBar.h"
#include "Rconv.h"
#include "REdit.h"

class BToolBitText
{
private:
	string	Message;
	RStringList MatchList;
public:
	void Create(const string & inMessage , const string & inMatchList)
	{
		this->Message = inMessage;
		this->MatchList = RConv::Explode(inMatchList);
	}

	const string getMessage() const
	{
		return this->Message;
	}
	const RStringList* getMatchList() const
	{
		return &this->MatchList;
	}
};


class BUserDataIconDlg : public RDialog  
{
public:
	BUserDataIconDlg();
	virtual ~BUserDataIconDlg();

	void Create(const RGISSmallShareImage* poolShareImage, int inNowIconNumber,
		const string & inName , const string & inCatego , const string & inSubCatego);

	//���ݑI������Ă���A�C�R�����擾���܂�.
	int getNowIconNumber() const
	{
		return NowIconNumber;
	}

	//�E�B���h�E�v���V�[�W��.
	LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnInit();
	void OnPaint();
	void OnScroll(WPARAM inWParam);

	//�����̓ǂݍ���
	void Load();
	//�����I��.
	int AutoMatch(const string & inName , const string & inCatego , const string & inSubCatego) const;
	//���X�g�̒��̕������ inName �̕�������v���Ă��邩����.
	bool TextFind(const RStringList* inList , const string & inName) const;
	//�w�肳�ꂽ�A�C�R���̐����̎擾
	const string getIconMessage(int inIconSelect) const;

private:
	const RGISSmallShareImage* PoolShareImage;		//�g�p����摜�ꗗ.
	int			NowIconNumber;						//���ݑI������Ă���A�C�R���ԍ�.
	RScrollBar	ScrollBar;
	REdit		IconMessageEdit;

	string Name;
	string Catego;
	string SubCatego;

	newVector<BToolBitText>	Text;

};

#endif // !defined(AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_)
