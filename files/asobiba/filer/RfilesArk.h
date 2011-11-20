// RfilesArk.h: RfilesArk �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_)
#define AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableEasy.h"

#include "RfileLocalBigIcon.h"
#include "RfileLocalReport.h"

//���k�f�B�X�N�t�@�C���ꗗ�̎擾.
template <class _RFILE>
class RfileArk : public Rfiles  
{
private:
	//��ׂ܂����
	RfilesLocal() : Rfiles("") {}
public:
	RfilesLocal(string inPath) : Rfiles(inPath) {}
	virtual ~RfilesLocal(){}

	//�ꗗ�擾
	virtual void ls()
	{
		WIN32_FIND_DATA data;
		HANDLE p;

		RTable * rt = getWritebleTable();

		rt->clear();

		string wildpath = this->getPath() + "*.*";
		__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
		p = ::FindFirstFile( wildpath.c_str() , &data);
		if (p != INVALID_HANDLE_VALUE)
		{
			do
			{
				// . �܂��� .. �̃t�@�C���͖���.
				if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
					(strcmp(data.cFileName , ".") ==0 ||
					 strcmp(data.cFileName , "..")==0) )	continue;

				_RFILE* p = new _RFILE;
				p->Attach(this , data.cFileName , 
					INT64CONVERT(data.nFileSizeHigh , data.nFileSizeLow)  ,
					INT64CONVERT(data.ftCreationTime.dwHighDateTime , data.ftCreationTime.dwLowDateTime) ,
					data.dwFileAttributes);
				p->SortAttach( sortkey );
				rt->add( p );
			}
			while( ::FindNextFile( p , &data) );
			::FindClose(p);
		}


		rt->Sort();
		rt->createAllList( );
	}
	//�ꗗ�擾
	virtual void ls(string inFilter)
	{
	}
	//�폜
	virtual void rm(string inFilename)
	{
	}
	//���l�[��
	virtual void rename(string inSrcFilename,string inDestFilename)
	{
	}
	//��ނ�Ԃ�.
	virtual __RFILETYPE		getType()	const
	{
		return __RFILETYPE_LOCALFILE_REPORT;
	}

	//RTable���쐬���܂��B
	//ls ���g���O�ɌĂ�ł��������B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
	{
		switch(inKey)
		{
		case __FILECOLUMN_TYPE_NAME:
			setTable( new RTableEasy(inKey,IsAscending) );
			break;
		case __FILECOLUMN_TYPE_EXT:
			setTable( new RTableMuchDuplication<string>(inKey,IsAscending) );
			break;
		case __FILECOLUMN_TYPE_SIZE:
//			setTable( new RTableLittleDuplication<__int64>(inKey,IsAscending) );
			break;
		case __FILECOLUMN_TYPE_DATE:
//			setTable( new RTableLittleDuplication<__int64>(inKey,IsAscending) );
			break;
		default:
			throw RNotSupportException("�w�肳�ꂽ�^�C�v�̃J���� %d �̓T�|�[�g����Ă��܂���",inKey);
			break;
		}
		return;
	}

	//�e�X�g.
	static void test()
	{
		{
			RfilesLocal<RfileLocalReport> p("c:\\test\\");

			//�ꗗ�擾
			p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
			p.ls(  );

			const RTable* rt = p.getTable();

			//�ꗗ���ƍ�
			DisplayDebugPrintf ddp;
			ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

			string DebugString = "1039.jpg �� JPG�t�@�C�� 2get.jpg �� JPG�t�@�C�� giko2a.jpg �� JPG�t�@�C�� NHK�Ƀ��i�[.jpg �� JPG�t�@�C�� NHK�Ƀ��i�[2.jpg �� JPG�t�@�C�� ���ɂ��������傢.jpg �� JPG�t�@�C�� �T�b�J�[�Łi߄t߁jʧ.jpg �� JPG�t�@�C�� �z�[���������̃o�J.jpg �� JPG�t�@�C�� �����]�[�}.jpg �� JPG�t�@�C�� ��_�����o�[.jpg �� JPG�t�@�C�� ����������.jpg �� JPG�t�@�C�� �����ėǂ��P�[�L.jpg �� JPG�t�@�C�� ���X���̗\��.jpg �� JPG�t�@�C�� ����������@�S�[�S�[�S�[.jpg �� JPG�t�@�C�� ";
						
			if ( ddp.DebugBuffer != DebugString )
			{
				TRACE("\n%s\n",ddp.DebugBuffer.c_str());
				ASSERT(0);
			}

		}
		{
			//�擾�ł��Ȃ��f�B���N�g���[��ݒ肵�Ă݂�
			RfilesLocal<RfileLocalReport> p("c:\\t����ō�!!");

			//�ꗗ�擾
			p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
			p.ls();

			const RTable* rt = p.getTable();

			//�ꗗ���ƍ�
			DisplayDebugPrintf ddp;
			ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

			string DebugString = "";

			if ( ddp.DebugBuffer != DebugString )
			{
				TRACE("\n%s\n",ddp.DebugBuffer.c_str());
				ASSERT(0);
			}
		}
	}
};

#endif // !defined(AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_)
