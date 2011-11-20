// RfilesLocal.cpp: RfilesLocal �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileFactory.h"
#include "RfilesLocal.h"
#include "RNotFoundException.h"
#include "regex.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�������ł��Ȃ������ꍇ�� RNotSupportException �𓊂���.
RfilesLocal::RfilesLocal(__RFILETYPE inRfileType , const string inPath) 
{
	RfileType = inRfileType;
	Path = inPath;

	switch(RfileType)
	{
	case __RFILETYPE_LOCALFILE_REPORT:
	case __RFILETYPE_LOCALFILE_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesLocal::�w�肳�ꂽ �t�@�C���`�� %d �� �T�|�[�g���Ă��܂���B",RfileType);
		break;
	}
}
//final
RfilesLocal::~RfilesLocal()
{
}

//�ꗗ�擾
void RfilesLocal::ls()
{
	WIN32_FIND_DATA data;
	HANDLE p;

	RTable * rt = getWritebleTable();

	rt->clear();

	string wildpath = this->Path + "*.*";
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
	p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			// . �܂��� .. �̃t�@�C���͖���.
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
				(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) )
			)	continue;

			RfileLocal* p = (RfileLocal*) RfileFactory::New(RfileType);
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
void RfilesLocal::ls(string inFilter)
{
	WIN32_FIND_DATA data;
	HANDLE p;

	RTable * rt = getWritebleTable();

	//�����p���K�\����ݒ�.
	boost::reg_expression<char> filterRegex  = inFilter.c_str();
	boost::match_results<const char*> results;

	rt->clear();

	string wildpath = this->Path + "*.*";
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
	p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			// . �܂��� .. �̃t�@�C���͖���.
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
				(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) )
			)	continue;

			if ( boost::regex_search(data.cFileName , results,filterRegex) == 0) continue;

			RfileLocal* p = (RfileLocal*) RfileFactory::New(RfileType);
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
//�폜
void RfilesLocal::rm(string inFilename)
{
}
//���l�[��
void RfilesLocal::rename(string inSrcFilename,string inDestFilename)
{
}
//��ނ�Ԃ�.
__RFILETYPE		RfilesLocal::getRfileType()	const
{
	return RfileType;
}
//�p�X�̎擾
string RfilesLocal::getPath() const 
{
	return Path;
}
//�p�X�̐ݒ�
void		RfilesLocal::setPath(const string inPath)  
{
	this->Path = inPath;
}
//RTable���쐬���܂��B
//ls ���g���O�ɌĂ�ł��������B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
void RfilesLocal::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
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
void RfilesLocal::test()
{
	{
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");

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
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\t����ō�!!");

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
