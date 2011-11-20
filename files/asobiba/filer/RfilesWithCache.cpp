// RfilesWithCache.cpp: RfilesWithCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesWithCache.h"
#include "regex.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RfilesWithCache::RfilesWithCache(CacheFileFnctional* inCff , __RFILETYPE inRfileType , const string inPath) throw()
{
	ASSERT(inCff != NULL);

	this->Cff = inCff;
	this->RfileType = inRfileType;
	this->Path = inPath;

	//�\�[�X�p�X�̎擾
	this->SrcPath = Cff->getSrcPath( Path );

	//�I���W�i������
	RCacheFS* cfs = RCacheFS::getInstance();
	Rfiles* orignal = cfs->Search( inRfileType,this->SrcPath );
	//�Ȃ��ꍇ�͍쐬.
	if ( orignal == NULL )
	{
		orignal = Cff->New(inRfileType,this->SrcPath);
		cfs->Append( orignal );
		//�V�K�쐬�Ȃ̂� MakeTable ���Ă΂ꂽ�Ƃ��Ƀe�[�u�����쐬���Ă����Ȃ��Ƃ����Ȃ��B
		this->MostMakeRTable = true;
	}
	else
	{
		this->MostMakeRTable = false;
	}
	this->OrignalFiles = orignal;

	ASSERT(this->OrignalFiles != NULL);
}

RfilesWithCache::~RfilesWithCache()
{
	//�����g��Ȃ��̂ŃL���b�V���ɔj���𖽂���.
	RCacheFS* cfs = RCacheFS::getInstance();
	cfs->Remove( this->OrignalFiles );

	delete Cff;
}


//�ꗗ�擾
void RfilesWithCache::ls()
{
	//���� RTable �Ɋi�[���Ă����܂��B
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//�L���b�V�����Ă��� RTable ���擾.
	const RTable * table = OrignalFiles->getTable();
	int len = table->getAllSize();
	if (len <= 0)
	{
		//ls�������s�Ȃ̂Ŏ��s���Ă�����.
		OrignalFiles->ls();
		//������x�T�C�Y�̎擾.
		len = table->getAllSize();
	}
	const Rfile** file = table->getAllList();

	//���΃p�X�̎擾.
	const string relativePath = Cff->getRelativePath(this->Path);

	for(int i = 0; i < len ; i++)
	{
		if ( relativePath == file[i]->getVars(__FILECOLUMN_TYPE_VIRTUALPATH) )
		{
			//�R�s�[.
			Rfile* p = RfileFactory::Clone(file[i]);
			p->SortAttach( sortkey );
			rt->add( p );
		}
	}
	rt->Sort();
	rt->createAllList( );
}

//�ꗗ�擾
void RfilesWithCache::ls(string inFilter)
{
	//���� RTable �Ɋi�[���Ă����܂��B
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//�����p���K�\����ݒ�.
	boost::reg_expression<char> filterRegex  = inFilter.c_str();
	boost::match_results<const char*> results;

	//�L���b�V�����Ă��� RTable ���擾.
	const RTable * table = OrignalFiles->getTable();
	int len = table->getAllSize();
	if (len <= 0)
	{
		//ls�������s�Ȃ̂Ŏ��s���Ă�����.
		OrignalFiles->ls();
		//������x�T�C�Y�̎擾.
		len = table->getAllSize();
	}
	const Rfile** file = table->getAllList();

	//���΃p�X�̎擾.
	const string relativePath = Cff->getRelativePath(this->Path);

	for(int i = 0; i < len ; i++)
	{
		if ( relativePath == file[i]->getVars(__FILECOLUMN_TYPE_VIRTUALPATH) )
		{
			if ( boost::regex_search(file[i]->getVars(__FILECOLUMN_TYPE_NAME) , results,filterRegex) == 0) continue;

			//�R�s�[.
			Rfile* p = RfileFactory::Clone(file[i]);
			p->SortAttach( sortkey );
			rt->add( p );
		}
	}
	rt->Sort();
	rt->createAllList( );
}
//�폜
void RfilesWithCache::rm(string inFilename)
{
}
//���l�[��
void RfilesWithCache::rename(string inSrcFilename,string inDestFilename)
{
}
//��ނ�Ԃ�.
__RFILETYPE		RfilesWithCache::getRfileType()	const
{
	return this->OrignalFiles->getRfileType();
}
//��ނ�Ԃ�.
string		RfilesWithCache::getPath()	const
{
	return this->Path;
}
//�p�X�̐ݒ�
void		RfilesWithCache::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTable���쐬���܂��B
//ls ���g���O�ɌĂ�ł��������B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
void RfilesWithCache::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	//�����ŕێ����Ă��� Rfiles* �͐V�K�ɍ�����̂�
	//RTable �������Ă��Ȃ������肵�܂�?
	if ( this->MostMakeRTable )
	{
		this->OrignalFiles->MakeTable(inKey,IsAscending);
		this->MostMakeRTable = false;
	}
	//�I���W�i���� RTable ���쐬������.
	setTable( Cff->ProxyCreateTable( this->OrignalFiles, inKey,IsAscending) );
}

//�e�X�g.
void RfilesWithCache::test()
{
	{
		ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
		acff->ArchiveType = __ARCHIVETYPE_LZH;
		RfilesWithCache rfwc(acff, __RFILETYPE_ARC_BIGICON , "c:\\test3\\test.lzh::\\");
		rfwc.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		rfwc.ls();

		const RTable* rt = rfwc.getTable();

		//�ꗗ���ƍ�
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "OpenOffice.org Calc.lnk �� �V���[�g�J�b�g OpenOffice.org Draw.lnk �� �V���[�g�J�b�g OpenOffice.org Writer.lnk �� �V���[�g�J�b�g abc �� �t�H���_ bb b �� �t�H���_ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
	{
		ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
		acff->ArchiveType = __ARCHIVETYPE_LZH;
		RfilesWithCache rfwc(acff, __RFILETYPE_ARC_BIGICON , "c:\\test3\\test.lzh::\\abc\\");
		rfwc.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		rfwc.ls();

		const RTable* rt = rfwc.getTable();

		//�ꗗ���ƍ�
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1039.jpg �� JPG�t�@�C�� 2get.jpg �� JPG�t�@�C�� �����]�[�}.jpg �� JPG�t�@�C�� ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
}
