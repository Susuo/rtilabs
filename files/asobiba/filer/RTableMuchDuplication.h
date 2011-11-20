// RTableMuchDuplication.h: RTableMuchDuplication �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_)
#define AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RTable.h"


//�t�@�C�������\�[�g���邽�߂̃N���X. (STL�̃R�s�[less)
struct FilenameLess : binary_function<Rfile, Rfile, bool> {
bool operator()(class Rfile *const _X, class Rfile *const _Y) const
//	{return (_X->getName() < _Y->getName() ); }
	{return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );}
};

//�\�[�g�L�[�ɏd���������\�[�g.
template<class _T>
class RTableMuchDuplication : public RTable  
{
public:
	//�����Ń\�[�g����.
	bool IsAscending;
	//�t�@�C�����̃\�[�g
	typedef deque<Rfile*> FILENAMESORT;
	//�\�[�g�{�w
	typedef map< _T , FILENAMESORT* > MAINSORT;
	//�e�B���N�g���͗D�悵�ă\�[�g����̂ŕʂɂ���.
	FILENAMESORT	DirectoryArc;
	//�\�[�g���ʂ��i�[����
	MAINSORT Arc;
private:
	RTableMuchDuplication() : RTable(__FILECOLUMN_TYPE_END) {}	//�Ăׂ܂����.
public:
	RTableMuchDuplication(__FILECOLUMN_TYPE inSortKey,bool IsAscending) : RTable(inSortKey)
	{
		this->IsAscending = IsAscending;
	}
	virtual ~RTableMuchDuplication()
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
			FILENAMESORT* p = Arc[ *(_T*)(inRfile->getSortKey()) ];
			if (p == NULL)
			{
				p = new FILENAMESORT;
				Arc[ *(_T*)(inRfile->getSortKey()) ] = p;
			}
			p->push_back(inRfile);
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
			for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
			{
				FILENAMESORT* name = (*ext).second;
				for(FILENAMESORT::iterator p = name->begin(); p != name->end() ; ++p)
				{
					ASSERT(i < FileCount);
					//Rfile �ǉ�.
					this->RFilesTable[i++] = ( *p );
				}
			}
		}
		else
		{
			for(MAINSORT::reverse_iterator ext =  Arc.rbegin(); ext != Arc.rend() ; ++ext)
			{
				FILENAMESORT* name = (*ext).second;
				for(FILENAMESORT::reverse_iterator p = name->rbegin(); p != name->rend() ; ++p)
				{
					ASSERT(i < FileCount);
					//Rfile �ǉ�.
					this->RFilesTable[i++] = ( *p );
				}
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
		for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
		{
			FILENAMESORT* name = (*ext).second;
			for(FILENAMESORT::iterator p = name->begin(); p != name->end() ; ++p)
			{
				//��������.
				delete (*p);
			}
			name->clear();
			delete name;
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
		//��ʃt�@�C��
		for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
		{
			FILENAMESORT* name = (*ext).second;
			std::sort(name->begin() , name->end() ,RTableMuchDuplication::Compre);
		}
		//�f�B���N�g��
		std::sort(DirectoryArc.begin() , DirectoryArc.end() ,RTableMuchDuplication::Compre);
	}

	//�t�@�C�����Ń\�[�g����֐� std::sort ���g�p����.
	static bool Compre(class Rfile *const _X, class Rfile *const _Y)
	{
		return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );
	}

	//�e�X�g.
	static void test()
	{
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");
		RTableMuchDuplication<string> a(__FILECOLUMN_TYPE_EXT,true);
		RfileLocalReport* rlr;

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ggg.dat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag4.dat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag3.pat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag2.aat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.eat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.gat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.mat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.nat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.pat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.fat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.bat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		a.Sort();

		a.createAllList(  );

		DisplayDebugPrintf d;
		d.setRfileItems( a.getAllSize() , a.getAllList( ) );
 
		string DebugString = "ag.bat �� �t�@�C�� ag.eat �� �t�@�C�� ag.fat �� �t�@�C�� ag.gat �� �t�@�C�� ag.mat �� �t�@�C�� ag.nat �� �t�@�C�� ag.pat �� �t�@�C�� ag2.aat �� �t�@�C�� ag3.pat �� �t�@�C�� ag4.dat �� �t�@�C�� ggg.dat �� �t�@�C�� ";
		if ( d.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",d.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
};

#endif // !defined(AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_)
