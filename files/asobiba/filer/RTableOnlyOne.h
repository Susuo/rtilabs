// RTableOnlyOne.h: RTableOnlyOne �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_)
#define AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RTable.h"

//������̗v�f�̃e�[�u��
//�����������\�[�g�������Ȃ��ł��B
class RTableOnlyOne : public RTable  
{
private:
	//�B��̃f�[�^
	Rfile*		UniqueConstituent;
public:
	RTableOnlyOne() : RTable(__FILECOLUMN_TYPE_END)
	{
		this->UniqueConstituent = NULL;
	}
	virtual ~RTableOnlyOne()
	{
		clear();
	}

	//�V�K�t�@�C���̒ǉ� (Rfile �͕K�� new �ō쐬���邱��)
	virtual void add(Rfile* inRfile)
	{
		ASSERT(this->FileCount < 1);

		this->UniqueConstituent = inRfile;
		this->FileCount = 1;
	}
	//�t�@�C���̈ꗗ���쐬����.
	virtual const void createAllList()
	{
		//�O��c���Ă��� Rfile* ��j��.
		delete [] this->RFilesTable;

		ASSERT(this->FileCount == 1);

		//����̕����m��.
		this->RFilesTable = new Rfile*[1];

		//Rfile �ǉ�.
		this->RFilesTable[0] = ( this->UniqueConstituent );
	}

	//�t�@�C���̈ꗗ���擾����.
	//createAllList ��ǂ񂾌�ŌĂяo���Ă�������!!
	virtual const Rfile** getAllList() const
	{
		ASSERT(this->RFilesTable != NULL);

		//const Rfile* �Ȃ̂ŏ��������͂ł��܂����B
		return (const Rfile**)this->RFilesTable;
	}

	//����.
	virtual void clear()
	{
		//�f�[�^�̔j��.
		delete this->UniqueConstituent;
		//���ݕێ����Ă��� Rfile�̒l�̃N���A.
		this->FileCount = 0;
	}

	//�\�[�g 
	virtual void Sort()
	{
		//������v�f���Ȃ����B
		//��邱�Ƃˁ[.
	}
};

#endif // !defined(AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_)
