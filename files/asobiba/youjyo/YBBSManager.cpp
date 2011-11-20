// YBBSManager.cpp: YBBSManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YBBSManager.h"
#include "RStreamMemory.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YBBSManager::YBBSManager()
{
}



YBBSManager::~YBBSManager()
{
	ClearBBS();
}

void YBBSManager::Read(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer )
				throw(RNotFoundException,RIOCannotOpenException)
{
	this->getBBS( inRequestPath->getBoardName()  )->Read( outShareBuffer , inRHTTPHeader, inRequestPath , outBuffer );
}

void YBBSManager::Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer )
				throw(RNotFoundException,RIOCannotOpenException)
{
	YBBS* bbs = this->getBBS( ioPost["bbs"] );

	const string submit = ioPost["submit"];
	if (submit == "��������" )
	{
		bbs->Write( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
	else if (submit == "�V�K�X���b�h�쐬��ʂ�")
	{
		bbs->NewThread( outShareBuffer , ioPost , outBuffer );
	}
	else if (submit == "�V�K�X���b�h�쐬")
	{
		bbs->New( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
	else if (!ioPost["saku"].empty() )
	{	//�폜�˗���
		bbs->NewSakujyoIrai(outShareBuffer , inRHTTPHeader ,ioPost, outBuffer);
	}
	else
	{//�Ƃ肠������������.
		bbs->Write( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
}

/**
 * SystemWrite:				�V�X�e������̃��b�Z�[�W
 *
 * @param outShareBuffer	TLS�o�b�t�@.
 * @param ioPost			�������ޓ��e
 * @return void 
 */
void YBBSManager::SystemWrite(char* outShareBuffer,POST& ioPost)
				throw(RNotFoundException,RIOCannotOpenException,RNotFoundException)
{
	this->getBBS( ioPost["bbs"] )->SystemWrite(outShareBuffer , ioPost);
}

//�폜
void YBBSManager::Aborn(const string inTargetIta ,DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete)
																throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
	this->getBBS(inTargetIta)->Aborn(inDeleteTarget , inDeleteSize , inDelete);
}


//����̔̓���̃X���b�h�̃^�C�g���̎擾.
const string YBBSManager::getSubject(const string inBoardName , DATNUMBER inDatNumber) throw(RNotFoundException)
{
	return this->getBBS(inBoardName)->getSubject(inDatNumber);
}


YBBSManager* YBBSManager::getInstance()
{
	static	YBBSManager	ybbsmanager;
	return &ybbsmanager;
}

//�폜�l�^�X�N.
void YBBSManager::KillerTask(char* outShareBuffer)
{
	//�폜�˗��̏�Ԃ����āA
	//�폜����ׂ��ł���΍폜����.
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->KillerTask(outShareBuffer);
	}
}

//DAT�K�y�[�W�R���N�^
void YBBSManager::GCTask()
{
	//�폜�˗��̏�Ԃ����āA
	//�폜����ׂ��ł���΍폜����.
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->GCTask();
	}
}

//�����o��.
void YBBSManager::Dump()
{
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->Dump();
	}
}
///���ׂĂ̏����f�B�X�N�Ƀt���b�V������.
void YBBSManager::Flush()
{
	puts("��Flush�J�n");
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->Flush();
	}
	puts("��Flush�I��");
}

//BBS���ēǂݍ���
void YBBSManager::ReloadBBS() throw(RException)
{
	ClearBBS();

	const string htdocs = YHttpdConf::getInstance()->getDocumentRoot() + "/";
	const string findpath = htdocs + "*";

	WIN32_FIND_DATA data;
	HANDLE p = ::FindFirstFile( findpath.c_str() , &data);
	if (p == INVALID_HANDLE_VALUE)
	{
		throw RNotFoundException("�w�肳�ꂽ�f�B���N�g��(%s)�ɂ́A�f���炵���f�B���N�g��������܂���" , htdocs.c_str() );
	}

	do
	{
		//�f�B���N�g���łȂ���Ζv.
		if ( !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) continue;
		// . �܂��� .. �̃t�@�C���͖���.
		if (data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') )) 
			continue;
		//�f���f�B���N�g�����ǂ����́A SETTING.TXT �Ŕ��f���܂�(�蔲��)
		const string fullname = htdocs + data.cFileName + "/SETTING.TXT";
		if (! FILE_EXIST(fullname.c_str() ) )	continue;

		YBBS* bbs = NULL;
		try
		{
			//�o�^.
			bbs = new YBBS( string(data.cFileName) );
			this->AllBBS[ string(data.cFileName) ] = bbs;
		}
		catch(RException e)
		{
			delete bbs;
			throw ;
		}

		//�o�^���b�Z�[�W.
		printf("�f���o�^... %s\r\n",data.cFileName);
	}
	while( ::FindNextFile( p , &data) );
	::FindClose(p);

}

//BBS�����ׂĔj������.
void YBBSManager::ClearBBS()
{
	//���ׂĉ��.
	for(BBSS::iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		delete (*i).second;
	}
	AllBBS.clear();
}

//�폜�˗����ꂽ�������݂̂�΂��P����}�X�N����.
void YBBSManager::MaskReWrite(const string inTargetIta  ,DATNUMBER inDatNumber ,const string & password ,
	 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
	 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
	this->getBBS(inTargetIta)->MaskReWrite(inDatNumber , password , inDeleteSize , inDelete , inMaskSize , inMask);
}


/**
 * getBBS:					�w�肳�ꂽ�ɑ΂���BBS���擾.
 *
 * @param inTargetIta 
 * @return YBBS* 
 */
YBBS* YBBSManager::getBBS(const string &inTargetIta) throw(RNotFoundException)
{
	BBSS::const_iterator i = AllBBS.find( inTargetIta );
	if (i == AllBBS.end() )
	{
		throw RNotFoundException(EXCEPTIONTRACE + inTargetIta + "�Ƃ������O�̌f���͂���܂���");
	}
	return (*i).second;
}