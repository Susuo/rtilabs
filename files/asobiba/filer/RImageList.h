// RImageList.h: RImageList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_)
#define AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include <SHLOBJ.H>

#include "RDoNotInitaliseException.h"

class RImageList  
{
public:
	RImageList();
	virtual ~RImageList();

	//�C���[�W���X�g�̍쐬
	//�쐬���s�� RDoNotInitaliseException .
	void Create(int inWidth,int inHegith,int inInitCount,int inMax) throw();
	//�C���[�W���X�g�̔j��
	void Destroy();

	//�ǉ�
	int Add(HICON inIcon);
	//�w�肵�� index ���폜
	//����ɂ߂��肵�Ȃ��̂ŁA�}�}�����S�ł��B
	void Remove(int inIndex);
	//�S������.
	void RemoveAll();

	//�C���[�W���X�g�̎擾.
	//����ɂ��̃N���X�O���珑�����݂����ꍇ�͌��ʂ͕ۏ؂��Ȃ��B
	const HIMAGELIST getImageList() const
	{
		return this->ImageList;
	}

private:
	//�����ꂽ�ʒu���L�^���Ă���.
	typedef deque<int>	REMOVELIST;
	REMOVELIST			RemoveList;

	HIMAGELIST			ImageList;
};

#endif // !defined(AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_)
