// RfilesLzh.cpp: RfilesLzh �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesLzh.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//�T�|�[�g���Ă��Ȃ���� RNotSupportException
//!�������ł��Ȃ������� RDoNotInitaliseException
//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
RfilesLzh::RfilesLzh(__ARCHIVETYPE inArchiveType,__RFILETYPE inRfileType,const string inPath) throw()
{
	RfileType = inRfileType;
	Path = inPath;

	ArchiveEngine = RArchiveFactory::New(inArchiveType);

	switch(RfileType)
	{
	case __RFILETYPE_ARC_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesLzh::�w�肳�ꂽ �t�@�C���`�� %d �� �T�|�[�g���Ă��܂���B",RfileType);
		break;
	}
}
//final
RfilesLzh::~RfilesLzh()
{
	delete ArchiveEngine;
}

//�ꗗ�擾
void RfilesLzh::ls()
{
	INDIVIDUALINFO data;
	HARC p;

	RTable * rt = getWritebleTable();

	rt->clear();

	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//���łɓo�^�����f�B���N�g��.
	set<string>	knownPath;

	try
	{
		ArchiveEngine->Create();

		//�J���܂��B �J���Ȃ������ꍇ��O
		p = ArchiveEngine->OpenArchive(WindowsMain::getMainSystem()->getWindowHandle(), this->getPath().c_str() , M_ERROR_MESSAGE_OFF | M_RECOVERY_ON | M_NO_BACKGROUND_MODE | M_CHECK_FILENAME_ONLY);
		if ( ArchiveEngine->FindFirst(p , "*.*" , &data) )
		{
			do
			{

				string fullname = data.szFileName;

				// / �� \\ �ɂ���.
				replace( fullname.begin() , fullname.end() , '/','\\');
				
				if (fullname[0] != '\\')	fullname = string("\\") + fullname;
				else			     		fullname = data.szFileName;

				//�Ō�� ��؂蕶�����擾.
				int lastpos = MultiString::getLastDirectoryPos( fullname  );

				RfileArc* p = (RfileArc*) RfileFactory::New(RfileType);
				p->Attach(
					fullname.substr(0 ,lastpos + 1).c_str()   , 
					fullname.substr(lastpos + 1).c_str()	,
					data.dwOriginalSize ,
					MAKELONG(data.wDate , data.wTime) ,
					data.wRatio,
					data.szMode,
					data.szAttribute
					);
				p->SortAttach( sortkey );
				rt->add( p );

				//���܂Ƀf�B���N�g�������k���Ă���Ȃ��������̂�
				//���̃p�X�̃f�B���N�g�����i�[����Ă��邩���ׂāA�i�[����Ă��Ȃ���Βǉ�����.
				int dilimiter[MAX_DIMLIMITERES+1] ;	//+1�͔ԕ�
				int maxDilimiter = MultiString::setPathDelimiteres(fullname.substr(0 ,lastpos ).c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
				ASSERT(maxDilimiter <= MAX_DIMLIMITERES);
				dilimiter[maxDilimiter] = lastpos ;

				for( int pos = 0; pos <= (maxDilimiter-1) ; pos++)
				{
					const string folderdir  = fullname.substr(0 ,  dilimiter[pos] + 1 );
					const string foldername = fullname.substr(dilimiter[pos] + 1 , dilimiter[pos+1] - dilimiter[pos] - 1);

					const string unknownPath  = folderdir + foldername;

					if ( (!unknownPath.empty()) && (knownPath.find(unknownPath) == knownPath.end()) )
					{//�Ȃ��炵��...
						knownPath.insert(unknownPath);
						
						RfileArc* p = (RfileArc*) RfileFactory::New(RfileType);
						p->Attach(
							folderdir.c_str()   , 
							foldername.c_str()	,
							0 ,
							0,
							0,
							"",
							"DIR"
							);
						p->SortAttach( sortkey );
						rt->add( p );
					}
				}
			}
			while( ArchiveEngine->FindNext( p , &data) );
		}
		ArchiveEngine->CloseArchive(p);
	}
	catch(RDoNotInitaliseException e)
	{
		TRACE("�擾���� RDoNotInitaliseException ���������܂��� ���R:%s\n" , e.getMessage() );
	}
	catch(RNotSupportException e)
	{
		TRACE("�擾���� RNotSupportException ���������܂��� ���R:%s\n" , e.getMessage() );
	}
	catch(RIOCannotOpenException e)
	{//OpenArchive�Ɏ��s
		TRACE("�擾���� RIOCannotOpenException ���������܂��� ���R:%s\n" , e.getMessage() );
	}
	catch(RIOCannotCloseException e)
	{//CloseArchive�Ɏ��s
		TRACE("�擾���� RIOCannotCloseException ���������܂��� ���R:%s\n" , e.getMessage() );
	}
	catch(RArchiveException e)
	{//�擾���ɃG���[
		TRACE("�擾���� RArchiveException ���������܂��� ���R:%s\n" , e.getMessage() );
	}

	rt->Sort();
	rt->createAllList( );
}
//�ꗗ�擾
void RfilesLzh::ls(string inFilter)
{
	//��ʑw������Ă����̂ł����ł͓��ɋ�ʂ��܂���.
	ls();
}
//�폜
void RfilesLzh::rm(string inFilename)
{
}
//���l�[��
void RfilesLzh::rename(string inSrcFilename,string inDestFilename)
{
}
//��ނ�Ԃ�.
__RFILETYPE		RfilesLzh::getRfileType()	const
{
	return RfileType;
}
//�p�X�̎擾
string RfilesLzh::getPath() const 
{
	return Path;
}
//�p�X�̐ݒ�
void		RfilesLzh::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTable���쐬���܂��B
//ls ���g���O�ɌĂ�ł��������B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
void RfilesLzh::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	setTable( CreateTable(inKey,IsAscending) );
}

//RTable���쐬���܂��B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
RTable* RfilesLzh::CreateTable(__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() 
{
	switch(inKey)
	{
	case __FILECOLUMN_TYPE_NAME:
		return new RTableEasy(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_EXT:
		return new RTableMuchDuplication<string>(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_SIZE:
//			return new RTableLittleDuplication<__int64>(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_DATE:
//			return new RTableLittleDuplication<__int64>(inKey,IsAscending) ;
		break;
	default:
		throw RNotSupportException("�w�肳�ꂽ�^�C�v�̃J���� %d �̓T�|�[�g����Ă��܂���",inKey);
		break;
	}
	return NULL;	//nop
}


//�e�X�g.
void RfilesLzh::test()
{
	{
		RfilesLzh p(__ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON,"c:\\test3\\a.lzh");

		//�ꗗ�擾
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls(  );

		const RTable* rt = p.getTable();

		//�ꗗ���ƍ�
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1027728372.jpg �� JPG�t�@�C�� a �� �t�H���_ b �� �t�H���_ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
	{
		RfilesLzh p(__ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON,"c:\\test3\\test.lzh");

		//�ꗗ�擾
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls(  );

		const RTable* rt = p.getTable();

		//�ꗗ���ƍ�
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1039.jpg �� JPG�t�@�C�� 2get.jpg �� JPG�t�@�C�� �����]�[�}.jpg �� JPG�t�@�C�� �����ėǂ��P�[�L.jpg �� JPG�t�@�C�� ���X���̗\��.jpg �� JPG�t�@�C�� OpenOffice.org Calc.lnk �� �V���[�g�J�b�g OpenOffice.org Draw.lnk �� �V���[�g�J�b�g OpenOffice.org Writer.lnk �� �V���[�g�J�b�g abc �� �t�H���_ bb b �� �t�H���_ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
}
