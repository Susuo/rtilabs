// PathInterface.h: PathInterface �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_)
#define AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PathInterface  ;

#include "comm.h"
#include "Rfile.h"
#include "Rfiles.h"
#include "RFixedColumn.h"
#include "Display.h"


//���p�X�̊i�[.
class PathInterface  
{
public:
	PathInterface()	{	}
	virtual ~PathInterface()	{	}

	//�p�X�̎擾.
	string getPath() const
	{
		return this->Path;
	}

	//��ނ�Ԃ�.
	virtual __RFILETYPE getType() const = 0;
	//���̃f�B�X�v���C�̓T�|�[�g���Ă܂���?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const = 0;
	//�ł���������f�B�X�v���C�́A���ł���?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const = 0;
	//�ł���������f�B�X�v���C�ł̃J�����������Ă�������
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const = 0;
	//�Ή����� Rfiles �̍쐬.
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const = 0;
	//��f�B���N�g�����オ��
	virtual string UpDirectory() const = 0;
protected:
	//�p�X��ݒ�.
	void	setPath(string inPath)
	{
		this->Path = inPath;
	}

private:
	string		Path;
};

#endif // !defined(AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_)
