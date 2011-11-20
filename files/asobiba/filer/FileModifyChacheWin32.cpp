// FileModifyChacheWin32.cpp: FileModifyChacheWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FileModifyChacheWin32.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

FileModifyChacheWin32::FileModifyChacheWin32(int inWidth,int inHegith ,int inSystemChacheSize)
{
	this->ImageList.Create(inWidth , inHegith ,  inSystemChacheSize , 1000);

	//�V�X�e���L���b�V���̗̈�̍쐬.
	this->NoneChacheSystem = new FileModifyAdministrant*[inSystemChacheSize];
	this->NoneChacheSystemSize = inSystemChacheSize;
}

FileModifyChacheWin32::~FileModifyChacheWin32()
{
	Clear();
	this->ImageList.Destroy();
}

//����.
//������Ȃ��ꍇ�� NULL
FileModifyData* FileModifyChacheWin32::Search(string inExt)
{
	return Chache[inExt];
}

//�V�X�e�����擾
FileModifyData* FileModifyChacheWin32::SearchSystem(int inPosition) const
{
	ASSERT(inPosition >= 0 && inPosition < this->NoneChacheSystemSize);
	return this->NoneChacheSystem[inPosition];
}

//������
FileModifyData* FileModifyChacheWin32::Tearch
				(string inExt,HICON inIcon , string inFileType)
{
	//���͂��łɒN���������Ă����ꍇ...
	FileModifyData* p = this->Chache[inExt];
	if ( p != NULL ) return p;

	const int index = this->ImageList.Add(inIcon);
	ASSERT(index != -1);
#ifdef _DEBUG
	//�d������index�����蓖�ĂȂ��悤�ɊĎ�.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ( (*it).second != NULL)
			{
				ASSERT( (*it).second->getIndex() != index );
			}
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT( (*t)->getIndex() != index );
		}
	}
#endif
	FileModifyAdministrant* r = new FileModifyAdministrant(	index,inFileType	);
#ifdef _DEBUG
	//�d�������l�����蓖�ĂȂ��悤�ɊĎ�.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			ASSERT(r != (*it).second );
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT(r != (*t) );
		}
	}
#endif
	this->Chache[inExt] = r;
	return r;
}

//������
FileModifyData* FileModifyChacheWin32::Tearch(HICON inIcon , string inFileType)
{
	const int index = this->ImageList.Add( inIcon);
	ASSERT(index != -1);
#ifdef _DEBUG
	//�d������index�����蓖�ĂȂ��悤�ɊĎ�.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ( (*it).second != NULL)
			{
				ASSERT( (*it).second->getIndex() != index );
			}
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT( (*t)->getIndex() != index );
		}
	}
#endif
	FileModifyAdministrant* r = new FileModifyAdministrant( index ,inFileType	);
#ifdef _DEBUG
	//�d�������l�����蓖�ĂȂ��悤�ɊĎ�.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			ASSERT(r != (*it).second );
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT(r != (*t) );
		}
	}
#endif
	NoneChache.push_back(r);

	return r;
}


//������  �L���b�V���ł��Ȃ��V�X�e���n�ȃf�[�^
FileModifyData* FileModifyChacheWin32::TearchSystem(int inPosition , HICON inIcon , string inFileType)
{
	ASSERT(inPosition >= 0 && inPosition < this->NoneChacheSystemSize);

	FileModifyAdministrant* r = new FileModifyAdministrant
		(	this->ImageList.Add( inIcon),inFileType	);
	this->NoneChacheSystem[inPosition] = r;

	return r;
}


//�g�p����
void FileModifyChacheWin32::Use(FileModifyData* inFileModifyData)
{
	((FileModifyAdministrant*)inFileModifyData)->Use();
}

//�폜����
void FileModifyChacheWin32::Delete(FileModifyData* inFileModifyData)
{
	FileModifyAdministrant* p =((FileModifyAdministrant*)inFileModifyData);
	
	p->Needless();
	if (p->IsDelete())
	{
		//�L���b�V����茟�����č폜���܂��B
		if ( SearchDelete(p) )
		{//�L���b�V���Ƀq�b�g���Ȃ��Ƃ������Ƃ� system�̃L���b�V�����Ƃ������Ƃł��B
			
			//�C���[�W���X�g���폜.
			this->ImageList.Remove(p->getIndex() );
			//���Ԃ��폜.
			delete p;
		}
	}
}

//�^�[�Q�b�g���������āA�폜����
//������Ȃ��ꍇ�� false .... �܂�V�X�e���n�̃L���b�V���ł��B
bool FileModifyChacheWin32::SearchDelete(FileModifyAdministrant* inTarget)
{
	ASSERT(inTarget != NULL);

	//�������� �L���b�V�����폜
	for ( ICONCHACHE::iterator i = this->Chache.begin() ;	i != this->Chache.end() ; ++i)
	{
		if ( ((*i).second) == inTarget)
		{
			this->Chache.erase(i);
			return true;
		}
	}
	//�������� ���j�[�N�L���b�V�����폜
	for ( ICONONECHACHE::iterator t = this->NoneChache.begin() ;	t != this->NoneChache.end() ; ++t)
	{
		if ( (*t) == inTarget)
		{
	#ifdef _DEBUG
			for ( ICONCHACHE::iterator x = this->Chache.begin() ;	x != this->Chache.end() ; ++x)
			{
				ASSERT ((*x).second != inTarget) ;
			}
	#endif
			this->NoneChache.erase(t);
			return true;
		}
	}
#ifdef _DEBUG
	//�{���ɃV�X�e���Ȃ̂��`�F�b�N���܂��B
	for ( int n = 0 ;	n < this->NoneChacheSystemSize ; n++ )
	{
		if ( this->NoneChacheSystem[n] == inTarget)
		{
			//OK
			return false;
		}
	}
	puts("�V�X�e���ɂ��Ȃ��t�@�C���C���q���폜���悤�Ƃ��܂����B\n�L���b�V���̓����������Ă��܂��B");
	ASSERT(0);
#else
	return false;
#endif
}

//�V�X�e���n�̃L���b�V�����܂߂� ALL ���ځ[��B
void FileModifyChacheWin32::Clear()
{
	//�L���b�V�������Ă悵
	for ( ICONCHACHE::iterator i = this->Chache.begin() ;	i != this->Chache.end() ; ++i)
	{
		delete (*i).second;
	}
	this->Chache.clear();

	//���j�[�N�ȃL���b�V���������Ă悵
	for ( ICONONECHACHE::iterator t = this->NoneChache.begin() ;	t != this->NoneChache.end() ; ++t)
	{
		delete (*t);
	}
	this->NoneChache.clear();

	for ( int n = 0 ;	n < this->NoneChacheSystemSize ; n++ )
	{
		delete this->NoneChacheSystem[n];
	}

	//�V�X�e���������Ă悵
	delete [] this->NoneChacheSystem;

	//�C���[�W���X�g���N���A.
	this->ImageList.RemoveAll();
}

//�f�o�b�O
void FileModifyChacheWin32::Dump()
{
	{
		TRACE("\n============== icon cache dump ================-\n");
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ((*it).second != NULL)
			{
				printf( "--------ext as %s " , ((*it).first).c_str() );
				((*it).second)->Dump();
			}
		}
	}
		TRACE("\n============== icon nonecache dump ================-\n");
	{
		for ( ICONONECHACHE::iterator it = NoneChache.begin() ;	it != NoneChache.end() ; it++)
		{
			(*it)->Dump();
		}
		TRACE("\n============== ============= ================-\n");
	}

}

