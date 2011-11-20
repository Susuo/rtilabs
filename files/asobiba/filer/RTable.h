// RTable.h: RTable �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_)
#define AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTable;
#include "Rfile.h"


enum __SORTOPTION
{
	__SORTOPTION_WITH_MUCH_DUPLICATION,		//�d��������
	__SORTOPTION_WITH_LITTLE_DUPLICATION,	//�d�������Ȃ�
};

//�t�@�C���ꗗ���i�[����N���X(���ۃN���X)
class RTable  
{
public:
	RTable(__FILECOLUMN_TYPE inSortKey)
	{
		RFilesTable = NULL;
		FileCount = 0;
		SortKey = inSortKey;
	}
	virtual ~RTable()
	{
		delete [] RFilesTable;
		RFilesTable = NULL;
	}
	//�V�K�t�@�C���̒ǉ� (Rfile �͕K�� new �ō쐬���邱��)
	virtual void add(Rfile* inRfile) = 0;
	//�t�@�C���̈ꗗ���擾����.
	virtual const Rfile** getAllList() const = 0;
	//�t�@�C���̈ꗗ���쐬����.
	virtual const void createAllList() = 0;
	//����.
	virtual void clear() = 0;
	//�\�[�g
	virtual void Sort() = 0;
	//�t�@�C���ꗗ�̐���ς���.
	int getAllSize() const { return FileCount; }
	//�\�[�g���鎞�ɃL�[�ɂ�����
	__FILECOLUMN_TYPE GetSortKey(){	return SortKey; }

protected:
	//�\������Ƃ��Ɏg���@���ׂĂ� Rfile �Ƀ����_���A�N�Z�X�ł���悤�ɂ��邽�߂̔z��
	//getAllList �ō쐬����.
	Rfile**	RFilesTable;
	//���݉��̗v�f�������Ă��邩
	int		FileCount;
	//�\�[�g�L�[
	__FILECOLUMN_TYPE SortKey;
};

#endif // !defined(AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_)
