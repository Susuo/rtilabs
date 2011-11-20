// RTableEasy.h: RTableEasy �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_)
#define AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RTable.h"




//�t�@�C���������̊ȈՃ\�[�g
class RTableEasy : public RTable  
{
private:
	//�����Ń\�[�g����.
	bool IsAscending;
	//�t�@�C�����̃\�[�g
	typedef deque<Rfile*> FILENAMESORT;
	typedef FILENAMESORT MAINSORT;
	//�e�B���N�g���͗D�悵�ă\�[�g����̂ŕʂɂ���.
	FILENAMESORT	DirectoryArc;
	//�\�[�g���ʂ��i�[����
	MAINSORT		Arc;

private:
	RTableEasy() : RTable(__FILECOLUMN_TYPE_END) {}	//�Ăׂ܂����.
public:
	RTableEasy(__FILECOLUMN_TYPE inSortKey,bool IsAscending) : RTable(inSortKey)
	{
		this->IsAscending = IsAscending;
	}
	virtual ~RTableEasy()
	{
		clear();
	}

	//�V�K�t�@�C���̒ǉ� (Rfile �͕K�� new �ō쐬���邱��)
	virtual void add(Rfile* inRfile)
	{
		if ( inRfile->IsDirectory() )
		{
			DirectoryArc.push_back(inRfile);
		}
		else
		{
			Arc.push_back(inRfile);
		}
		this->FileCount ++;
	}
	//�t�@�C���̈ꗗ���쐬����.
	virtual const void createAllList()
	{
		//�O��c���Ă��� Rfile* ��j��.
		delete [] this->RFilesTable;
		//����̕����m��.
		this->RFilesTable = new Rfile*[FileCount];

		int i = 0;
		if (this->IsAscending)
		{
			//�f�B���N�g���D��
			for(FILENAMESORT::iterator p = DirectoryArc.begin(); p != DirectoryArc.end() ; ++p)
			{
				ASSERT(i < FileCount);
				//Rfile �ǉ�.
				this->RFilesTable[i++] = ( *p );
			}
			//��ʃt�@�C��.
			for(MAINSORT::iterator fnm =  Arc.begin(); fnm != Arc.end() ; ++fnm)
			{
				ASSERT(i < FileCount);
				//Rfile �ǉ�.
				this->RFilesTable[i++] = ( *fnm );
			}
		}
		else
		{
			for(MAINSORT::reverse_iterator fnm =  Arc.rbegin(); fnm != Arc.rend() ; ++fnm)
			{
				ASSERT(i < FileCount);
				//Rfile �ǉ�.
				this->RFilesTable[i++] = ( *fnm );
			}
			//�f�B���N�g���D��
			for(FILENAMESORT::reverse_iterator p = DirectoryArc.rbegin(); p != DirectoryArc.rend() ; ++p)
			{
				ASSERT(i < FileCount);
				//Rfile �ǉ�.
				this->RFilesTable[i++] = ( *p );
			}
		}
		ASSERT(i == FileCount);
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
		//��ʃt�@�C��
		for(MAINSORT::iterator fnm =  Arc.begin(); fnm != Arc.end() ; ++fnm)
		{
			delete (*fnm);
		}
		Arc.clear();
		//�f�B���N�g��
		for(FILENAMESORT::iterator p = DirectoryArc.begin(); p != DirectoryArc.end() ; ++p)
		{
			delete (*p);
		}
		DirectoryArc.clear();

		//���ݕێ����Ă��� Rfile�̒l�̃N���A.
		this->FileCount = 0;
	}

	//�\�[�g 
	virtual void Sort()
	{
		std::sort(Arc.begin() , Arc.end() ,RTableEasy::Compre);
		std::sort(DirectoryArc.begin() , DirectoryArc.end() ,RTableEasy::Compre);
	}

	//�t�@�C�����Ń\�[�g����֐� std::sort ���g�p����.
	static bool Compre(class Rfile *const _X, class Rfile *const _Y)
	{
		return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );
	}

	//�e�X�g.
	static void test();

};

#endif // !defined(AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_)
