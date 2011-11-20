// YBBS.cpp: YBBS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YBBS.h"
#include "RHTTPResponse.h"
#include "RPackSuper.h"
#include "YThreadWithTls.h"
#include "RConv.h"
#include "YBBSManager.h"
#include "YFilter.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YBBS::YBBS(string inName)
{
	this->BoardName = inName;
	//�̐ݒ�̓ǂݍ���
	SettingTXT.Load(inName);
	//DAT�L���b�V��
	Cache.setBoardName(inName);
	//subject.txt
	Subject.Load(inName);
	//���[�J�����[���̓ǂݍ���.
	LocalRule.Load(inName);
	//index.html
	IndexHTML.Setting( inName , &SettingTXT );
	IndexHTML.Load( &Cache , &Subject , &LocalRule);
	//read.cgi
	ReadCGI.Load(inName , &SettingTXT);
	//subback.html
	SubBackHTML.Load(inName , &SettingTXT);
	SubBackHTML.Write(&Subject );
	//�V�K����
	BuildThread.Load(inName , &SettingTXT);
	//�������݊֌W.
	WriteHTML.Load(inName , &SettingTXT);

	DeleteWait.Load(inName);

}

YBBS::~YBBS()
{
	Subject.Save(this->BoardName);
	DeleteWait.Save(this->BoardName);
}
//REQUESTTYPE_DAT
void YBBS::RequestTypeDat(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,  const YRequestPath* inRequestPath, RStreamUtil * outBuffer)	throw(RIOCannotOpenException)
{
	//G
	YDAT* dat = Cache.getDAT(inRequestPath->getDatNumber() );
	time_t	lastModified = dat->getLastModified();

	//If ModifiedSince �̏ꍇ�A�X�V����Ă��Ȃ����`�F�b�N.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);

		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 304 Not Modified\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Connection: close\r\n"
										"Content-Length: 0\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);
		
		//�w�b�_�̓]��
		outBuffer->WriteBuffer( outShareBuffer , len ); 
		return ;
	}

	//Range���L���������獷���擾�����܂��B
	if ( inRHTTPHeader->Range.getValue().IsEnabled() )
	{
		/*ChangeLog.txt:2003/01/23	range ���s�����Ƃ��� Content-Range ���������������̂ŏC��.
			http://pc3.2ch.net/test/read.cgi/tech/1043275721/22
			Range�w���GET���鎞�ɕԂ����Content-Range����������ֶ�

			�{���Ȃ�
			Content-Length: 400001
			Content-Range: bytes 0-400000/400001�@�ƂȂ�Ƃ��낪

			Content-Length: 400001
			Content-Range: bytes 0-400001/400002�@�ɂȂ��Ă܂�			
		*/
		int rangeStart = inRHTTPHeader->Range.getValue().getStart();
		int rangeEnd = inRHTTPHeader->Range.getValue().getEnd();
		int filesize = dat->getDatSize();

		if (rangeEnd >= rangeStart && rangeEnd >= 0 && rangeStart >= 0)
		{
			//�t�@�C�����ł����]���͔F�߂Ȃ�.
			if (rangeEnd >= filesize) rangeEnd = filesize - 1;
			//���]���o�C�g
			int lenth = (rangeEnd+1) - rangeStart;

			int len = 
					sprintf( outShareBuffer ,	
								"HTTP/1.1 206 Partial Content\r\n"
								"Date: %s\r\n"
								"Server: Youjyo\r\n"
								"Last-Modified: %s\r\n"
								"Accept-Ranges: bytes\r\n"
								"Content-Length: %d\r\n"
								"Connection: close\r\n"
								"Content-Type: text/plain\r\n"
								"Content-Range: bytes %d-%d/%d\r\n"
								"\r\n"
							,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
							,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
							,	lenth
							,	rangeStart , rangeEnd , filesize
							);
			//�w�b�_�̓]��
			outBuffer->WriteBuffer( outShareBuffer , len ); 

			RPackSuper pack;
			pack.Mount(outShareBuffer , TLS_POOL_MAX);
			dat->ReadByte( &pack , rangeStart , rangeStart + lenth);
			//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
			Cache.Release(dat);

			//�{�̂̓]��.
			outBuffer->WriteBuffer((const char*) pack.getMemory() , pack.getSize() ); 
			return ;
		}
	}
	//�ʏ�]��.
	{
		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 200 OK\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Last-Modified: %s\r\n"
										"Accept-Ranges: bytes\r\n"
										"Content-Length: %d\r\n"
										"Connection: close\r\n"
										"Content-Type: text/plain\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
									,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
									,	dat->getDatSize()
									);
		//�w�b�_�̓]��
		outBuffer->WriteBuffer( outShareBuffer , len ); 

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);
		dat->ReadByteGoEND( &pack , 0);

		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);

		//�{�̂̓]��.
		outBuffer->WriteBuffer((const char*) pack.getMemory() , pack.getSize() ); 
	}
}


//�t�@�C���]��.
void YBBS::TextFileTransmit(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,YTextFile* inUseFile,  RStreamUtil * outBuffer)
{
	RPackSuper pack;
	pack.Mount(outShareBuffer , TLS_POOL_MAX);

	time_t	lastModified = inUseFile->getLastModified();

	//If ModifiedSince �̏ꍇ�A�X�V����Ă��Ȃ����`�F�b�N.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 304 Not Modified\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Connection: close\r\n"
										"Content-Length: 0\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);
		//�w�b�_�̓]��
		outBuffer->WriteBuffer( outShareBuffer , len ); 
		return ;
	}

	inUseFile->Show(&pack);

	string ret;
	ret.reserve(1024);
	ret += RHTTPResponse::OK();
	ret += RHTTPResponse::ServerName();
	ret += "Last-Modified: " + RDateConv::UnixTimeToHttpTime( lastModified ) + "\r\n";
	ret += "Accept-Ranges: bytes\r\n";
	ret += "Content-Length: "+ num2str( pack.getSize() ) + "\r\n";
	ret += "Connection: close\r\n";
	ret += "Content-Type: text/html\r\n";
	ret += RHTTPResponse::ResponseEnd();
	outBuffer->WriteBuffer( ret.c_str() , ret.size() ); 

	outBuffer->WriteBuffer( (const char*)pack.getMemory() , pack.getSize() ); 
}






//read.cgi �R���p�`�ȕ\�����s���܂��B DAT�̉���͂��̒��ōs���܂�(�������̂���).
void YBBS::RequestTypeCGI(char* outShareBuffer,YDAT * inDat , int inStart, int inEnd , bool is1san , RStreamUtil * outBuffer)	throw(RIOCannotOpenException)
{
	const string content = ReadCGI.make(inDat ,  inStart ,inEnd  , is1san );

	//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
	Cache.Release(inDat);

	//�������ݐ�����Ԃ�
	this->TranceString(outShareBuffer, content ,outBuffer);
}




void YBBS::Read(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer )		throw(RIOCannotOpenException,ROutOfRangeException)
{
	//���N�G�X�g�ɏ]������������f�X.
	//virtual �g�����ق����I�u�W�F�N�g�v�l�I�ɂ͂�����������܂��񂪁A���x�o�܂���B
	//�����̕����͂��[���̂���Ԃ̂�[�Ȃ������܂��B
	//enum �����A jump table ���R���p�C��������Ă���邱�Ƃ����҂��Ă��܂��B
	switch( inRequestPath->getRequestType() )
	{
	case REQUESTTYPE_DAT:
		RequestTypeDat(outShareBuffer , inRHTTPHeader , inRequestPath , outBuffer);
		break;
	case REQUESTTYPE_INDEX:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &IndexHTML , outBuffer);
		break;
	case REQUESTTYPE_SUBJECT:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &Subject , outBuffer);
		break;
	case REQUESTTYPE_SUBBACK:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &SubBackHTML , outBuffer);
		break;
	case REQUESTTYPE_CGIALL:
		//REQUESTTYPE_CGIALL	/test/read.cgi/php/1008344964/
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,  0 ,dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGILAST:
		//REQUESTTYPE_CGILAST	/test/read.cgi/php/1008344964/l50
		{
			YDAT* dat = Cache.getDAT (inRequestPath->getDatNumber() );

			unsigned int start = dat->getLastNo() ;
			if (start >= inRequestPath->getwParam() )	start -= inRequestPath->getwParam();
			else										start = 0;

			RequestTypeCGI( outShareBuffer , dat ,  start ,dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIONE:
		//REQUESTTYPE_CGIONE	/test/read.cgi/php/1000797355/231
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			const int one = inRequestPath->getwParam();

			RequestTypeCGI( outShareBuffer , dat , one,one , false , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIFORM:
		//REQUESTTYPE_CGIFORM	/test/read.cgi/php/1000797355/201-
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat , inRequestPath->getwParam() , dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIFORMTO:
		//REQUESTTYPE_CGIFORMTO	/test/read.cgi/php/1000797355/201-300
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,
				inRequestPath->getwParam() , inRequestPath->getlParam() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGITO:
		//REQUESTTYPE_CGITO	/test/read.cgi/php/1000797355/-300
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,	0 , inRequestPath->getwParam() , true , outBuffer);
		}
		break;
	default:
		throw ROutOfRangeException(EXCEPTIONTRACE + "���N�G�X�g�͔͈͊O�ł�");
	}
}

void YBBS::Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{
	//�폜�˗��ɑ΂���ًc����͂������ł��.
	if (SettingTXT.getIS_SAKUBBS() && !ioPost["igiari"].empty() ) 
	{
		Igiari(outShareBuffer , inRHTTPHeader,ioPost , outBuffer);
		return ;
	}

	try
	{
		//�A�N�Z�X�֎~�̃`�F�b�N.
		AccessCheck(inRHTTPHeader);
		//�����͂ݓ��e�̃`�F�b�N.
		WriteCheck(ioPost);

		DATNUMBER datnumber = atol( ioPost["key"].c_str() );

		//���ۂɏ������݂܂��B
		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			MakeAppendData(inRHTTPHeader,ioPost)
			);
		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );
		YDAT* dat = Cache.getDAT(datnumber);

		dat->Append(&pack);

		//�ǂ��̈ʒu�ɂ��邩����.
		int oldpos = Subject.WhatPosition( datnumber );

		if (ioPost["mail"] != "sage")
		{
			//subject.txt �̍X�V
			Subject.Age(dat);
			//index.html�̍X�V
			IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
			//subject.txt �̍X�V.
			SubBackHTML.Write(&Subject);
		}
		else
		{
			/*ChangeLog.txt:2003/01/26	sage�� subject.txt , subback.html ���X�V���Ă��Ȃ������̂��C��.
			*/
			//subject.txt �̍X�V
			Subject.Sage(dat);
			/*ChangeLog.txt:2003/01/29	sage�� age �̏��������Ă����̂��C��.
			*/
			//index.html�̍X�V
			IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  false);
			//subject.txt �̍X�V.
			SubBackHTML.Write(&Subject);
		}

		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);

		//5000�𒴂��Ă�����x������������
		if (dat->getLastNo() >= MAX_NUMBER)
		{
			POST	post;
			post["bbs"]  = this->BoardName;
			post["FROM"]  = "YoujyoMaster��";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"���̃X���b�h��" + num2str(MAX_NUMBER) + "�𒴂��܂����B\r\n"
						"���������Ȃ��̂ŁA�V�����X���b�h�𗧂ĂĂ��������ł��B�B�B";
			post["key"]  = num2str(datnumber);
			post["submit"] = "��������";

			this->SystemWrite(outShareBuffer , post);
		}

		//�������ݐ�����Ԃ�
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//�������݃G���[.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP() ) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" ,ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

void YBBS::New(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{

	//�폜�˗��̏ꍇ�͂������ł��.
	if (SettingTXT.getIS_SAKUBBS())
	{
		this->NewDelete(outShareBuffer , inRHTTPHeader,ioPost , outBuffer );
		return ;
	}

	try
	{
		//�A�N�Z�X�֎~�̃`�F�b�N.
		AccessCheck(inRHTTPHeader);
		//�����͂ݓ��e�̃`�F�b�N.
		WriteCheck(ioPost);

		DATNUMBER datnumber =  time(NULL);

		//���ۂ̃X������
		const string subject = RConv::htmlspecialchars(ioPost["subject"]);
		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			subject ,
			MakeAppendData(inRHTTPHeader,ioPost)
			);

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);
		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getNewDAT( datnumber );
		dat->Append(&pack);
		dat->setSubject( subject );

		//subject.txt �̍X�V.
		Subject.Age(dat);

		//index.html�̍X�V
		IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);
		//subject.txt �̍X�V.
		SubBackHTML.Write(&Subject);

		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);

		//�������ݐ�����Ԃ�
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//�G���[��Ԃ�.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//�폜�˗�
void YBBS::NewDelete(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) throw(RException)
{
	try
	{
		string mainMessage ;
		//�A�N�Z�X�֎~�̃`�F�b�N.
		AccessCheck(inRHTTPHeader);
		//�����͂ݓ��e�̃`�F�b�N.
		WriteCheck(ioPost);
		//�폜���̍쐬.
		mainMessage =  makeSakuJyoMessage(ioPost);

		//���̎��Ԃ�������폜���܂�.
		time_t deleteWait = time(NULL) + SettingTXT.getDELETE_WAIT();
		//�폜����X���b�h.
		DATNUMBER datNumber = atol( ioPost["threadno"].c_str() );
		//�폜�˗�����̃^�C�g��.
		string subject ;

		//�폜�Ώۂ̃X���b�h�� �^�C�g�� ���擾���A������폜�˗��X���� �^�C�g�� ���܂��B
		//���̂Ƃ��^�C�g�����擾�ł��Ȃ��������A����ȃX���b�h�͂Ȃ��킯�ŁA�G���[�Ƃ��܂��B
		subject = YBBSManager::getInstance()->getSubject(SettingTXT.getSAKUBBS() , datNumber) + "�ɑ΂���폜�˗�";
			
		//�V�K�ɗ��Ă�X���b�h.
		DATNUMBER newThreadNumber = time(NULL);
		//�폜�X�������Ă�.
		{
			const string msg = YDATWritter::make(
				CheckName(inRHTTPHeader, RConv::htmlspecialchars(ioPost["FROM"]) ),
				RConv::htmlspecialchars(ioPost["mail"]) ,
				YDATWritter::MessageESC(ThisBoardShortCut(newThreadNumber) ,mainMessage) ,
				RConv::htmlspecialchars(subject) ,
				MakeAppendData(inRHTTPHeader,ioPost)
				);
			RPackSuper pack;
			pack.Mount(outShareBuffer , TLS_POOL_MAX);
			pack.Write(msg.c_str() , msg.size() );

			YDAT* dat = Cache.getNewDAT( newThreadNumber );
			dat->Append(&pack);
			dat->setSubject( subject );

			//subject.txt �̍X�V.
			Subject.Age(dat);

			//index.html�̍X�V
			IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);
			//subject.txt �̍X�V.
			SubBackHTML.Write(&Subject);

			//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
			Cache.Release(dat);
		}

		//�폜�ΏۃX���b�h�ɍ폜�ʒm���o��.
		{
			POST	post;
			post["bbs"]  = SettingTXT.getSAKUBBS();
			post["FROM"]  = "YoujyoKiller��";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"���̃X���b�h�ɑ΂��č폜�˗��������ꂽ�̂ŕ񍐂��܂��B\r\n\r\n"
						"�폜�˗��X���b�h " + ThisBoard() + num2str(newThreadNumber) + "/l50\r\n\r\n"
						"���̂܂܈ًc���Ȃ���΁A " + RDateConv::UnixTime2chTime(deleteWait) + " �ɁA�Y���������݂͍폜����Ă��܂��܂��B\r\n"
						"�ًc�̂���l�͏�̃X���b�h�Ɂu�ًc����v�J�L�R���Ă��������B\r\n"
						"���̏ꍇ�A���ׂĂ̐ӔC�́u�ًc����v�J�L�R�����l�ɂȂ�̂Œ��ӂ��Ă�������\r\n\r\n"
						" ============= �폜�˗��̏ڍ� ===================\r\n"
						+ mainMessage;
			post["key"]  = num2str(datNumber);
			post["submit"] = "��������";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}

		//�폜���鏑�����݂̎��W.
		int deleteSize = 0;
		int deletes[DELETE_REQUEST_MAX];
		for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
		{
			string thread = "t" + num2str(i);

			if ( ! ioPost[thread].empty() )
			{
				deletes[deleteSize++] = atol( ioPost[thread].c_str() ) ;
			}
		}

		//�폜�˗��Ώۏ������݂̃}�X�L���O
		{
			string targetBoard = SettingTXT.getSAKUBBS() ;
			string password = ioPost["pass"] ;

			//�}�X�N�P������W
			int maskSize = 0;
			string mask[DELETE_REQUEST_MAX];
			for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
			{
				string thread = "m" + num2str(i);

				if ( ! ioPost[thread].empty() )
				{
					mask[maskSize++] = ioPost[thread];
				}
			}
			
			//�}�X�N����.
			YBBSManager::getInstance()->MaskReWrite
						(targetBoard , datNumber , password ,deleteSize , deletes ,maskSize ,  mask );
		}

		//�폜�˗����V�X�e���ɒ�o.
		{
			DeleteWait.Presentation(newThreadNumber ,datNumber ,  deleteWait , deleteSize , deletes);
		}

		//�������ݐ�����Ԃ�
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//�G���[��Ԃ�.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//�폜�������.
const string YBBS::makeSakuJyoMessage(POST& ioPost) throw(RException)
{
	DATNUMBER datNumber = atol( ioPost["threadno"].c_str() );

	//�폜�˗���
	string message;
	message += "�ΏۃX���b�h http://" + YHttpdConf::getInstance()->getServerName() + "/test/read.cgi/" + SettingTXT.getSAKUBBS() + "/" + ioPost["threadno"] + "/l50\r\n\r\n";

	for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
	{
		string thread = "t" + num2str(i);
		string reason = "r" + num2str(i);

		if ( ! ioPost[thread].empty() )
		{
			if ( ioPost[reason].empty() )
			{
				throw RException("�폜���˗����ꂽ " + ioPost[thread] + "�ɑ΂��闝�R�����͂���Ă��܂���");
			}
			message += ">>" + ioPost[thread] + " ���R: " + ioPost[reason] + "\r\n";
		}
	}
	message += "\r\n";
	message += "�ڍח��R\r\n";
	message += ioPost["MESSAGE"];

	return message;
}

//�������ݓ��e�̃`�F�b�N.
void YBBS::WriteCheck(POST& ioPost) throw(RException)
{
	//���O
	string name = ioPost["FROM"];
	if ( name.empty() )
	{
		//����������֎~�̏ꍇ�G���[.
		if ( SettingTXT.getNANASHI_CHECK() )
		{
			throw RException("���O�����͂���Ă��܂���");
		}
	}
	if ( name.size() >= SettingTXT.getBBS_NAME_COUNT() )
	{
		throw RException("���O���������܂�");
	}

	//���[��
	if ( ioPost["mail"].size() >= SettingTXT.getBBS_MAIL_COUNT() )
	{
		throw RException("���[���A�h���X���������܂�");
	}

	//�{��.
	if ( ioPost["MESSAGE"].size() >= SettingTXT.getBBS_MESSAGE_COUNT() )
	{
		throw RException("�{�����������܂�");
	}

	//�{�����Ȃ�.
	if ( ioPost["MESSAGE"].size() <= 0 )
	{
		throw RException("���e���Ȃ��悤�B�B�B");
	}

	//Subject.
	if ( ioPost["subject"].size() >= SettingTXT.getBBS_SUBJECT_COUNT() )
	{
		throw RException("�^�C�g�����������܂�");
	}
}

//�A�N�փ`�F�b�N.
void YBBS::AccessCheck(const RHTTPHeader* inRHTTPHeader) throw(RException)
{
	in_addr ip = inRHTTPHeader->RemoteHost.getRawIP();

	if ( YFilter::getInstance()->IsDenyIP( ip ) )
	{
		throw RException("���� IP ����̏������݂͋֎~����Ă��܂��B �A�N�ւ��炤�悤�Ȃ��Ƃ��܂���ł���?");
	}
	if ( SettingTXT.getBBS_NOTJP_FILTER() )
	{
		if (! YFilter::getInstance()->IsJapanIP( ip ) )
		{
			throw RException("�C�O����̏������݂͋֎~�����Ă��������Ă��܂��B");
		}
	}
}


//�V�K�X��.
void YBBS::NewThread(char* outShareBuffer, POST& ioPost, RStreamUtil * outBuffer)
{
	//�V�X��(or�폜�˗�)�𗧂Ă��ʂ��o��.
	this->TranceString(outShareBuffer, BuildThread.make(ioPost , &LocalRule) ,outBuffer);
}



//���ځ[��
void YBBS::Aborn(DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete) throw(RIOCannotOpenException,RIOCannotWriteException)
{
	const string msg = YDATWritter::makeAborn(SettingTXT.getBBS_DELETE_NAME());
	RPackSuper pack;
	pack.Write(msg.c_str() , msg.size() );

	YDAT* dat;
	try
	{
		dat = Cache.getDAT(inDeleteTarget);

		int maxNo = dat->getLastNo();

		for(int i = 0; i < inDeleteSize ; i++)
		{
			int no = inDelete[i];
			if (no <= maxNo && no > 0 )
			{
				dat->Overwrite(no-1 , &pack);
			}
		}

		//index.html�̍X�V
		IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);

		Cache.Release(dat);
	}
	catch(RException e)
	{
		Cache.Release(dat);
		throw;
	}
}


/**
 * SystemWrite:				�V�X�e������̃��b�Z�[�W
 *
 * @param outShareBuffer	TLS�o�b�t�@.
 * @param ioPost			�������ޓ��e
 * @return void 
 */
void YBBS::SystemWrite(char* outShareBuffer,POST& ioPost) 
{
	try
	{
		DATNUMBER datnumber = atol( ioPost["key"].c_str() );
		//�V�X�e�������ŏ�������.
		const string msg = YDATWritter::makeSystem(
				RConv::htmlspecialchars(ioPost["FROM"]) ,
				RConv::htmlspecialchars(ioPost["mail"]) ,
				YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]) 
				);

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getDAT(datnumber);
		dat->AppendAdmin(&pack);		//�V�X�e�������ŏ�������

		//�ǂ��̈ʒu�ɂ��邩����.
		int oldpos = Subject.WhatPosition( datnumber );

		//subject.txt �̍X�V
		Subject.Age(dat);
		//index.html�̍X�V
		IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
		//subject.txt �̍X�V.
		SubBackHTML.Write(&Subject);

		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);
	}
	catch(RException e)
	{
		printf("��O:�V�X�e�������������݂ŃG���[���������܂���:%s\r\n" , e.getMessage() );
		ASSERT(0);
	}
}

//����DAT��Subject�̎擾.
const string YBBS::getSubject(DATNUMBER inDatNumber) throw(RNotFoundException,RIOCannotOpenException)
{
	//DAT�̎擾.
	YDAT* dat = Cache.getDAT(inDatNumber);

	//Subject�̎擾.
	const string subject = dat->getSubject();

	//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
	Cache.Release(dat);

	return subject;
}

//�폜�˗��̒�o�⏕
void YBBS::NewSakujyoIrai(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer)
	throw(RNotFoundException,RIOCannotOpenException)
{
	ioPost["saku"] = "";
	ioPost["submit"] = "�V�K�X���b�h�쐬��ʂ�";
	ioPost["bbs"] = SettingTXT.getSAKUBBS();

	YBBSManager::getInstance()->Write(outShareBuffer , inRHTTPHeader , ioPost , outBuffer);
}

//�ًc����!
void YBBS::Igiari(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{
	DATNUMBER datnumber = atol( ioPost["key"].c_str() );
	DATNUMBER targetdat ;

	try
	{
		//�A�N�Z�X�֎~�̃`�F�b�N.
		AccessCheck(inRHTTPHeader);
		//�����͂ݓ��e�̃`�F�b�N.
		WriteCheck(ioPost);

		if (ioPost["mail"] == "sage")	RException("sage �łً͈c����ł��܂���");

		//�܂����̍폜�˗��͗L��?
		targetdat = this->DeleteWait.IsYourRequest(datnumber);
		if ( targetdat == 0  )	throw RException("���̍폜�˗��͂��łɎ��s����Ă���̂Ŏ������܂���");

		try
		{
			//����� DAT �����݂��Ă���̂����ׂ�.
			(void) YBBSManager::getInstance()->getSubject( SettingTXT.getSAKUBBS() , targetdat);
		}
		catch(RException e)
		{
			throw RException("�폜�˗����͑��݂��Ă��܂���");
		}

		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			"<font color=\"red\" size=+2><B>�ًc����!</B></font><br>" + YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			MakeAppendData(inRHTTPHeader,ioPost)
			);
		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getDAT(datnumber);
		dat->Append(&pack);

		//�ǂ��̈ʒu�ɂ��邩����.
		int oldpos = Subject.WhatPosition( datnumber );

		//subject.txt �̍X�V
		Subject.Age(dat);
		//index.html�̍X�V
		IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
		//subject.txt �̍X�V.
		SubBackHTML.Write(&Subject);

		//�L���b�V���ɂ����g�p���Ȃ����Ƃ�`���܂�
		Cache.Release(dat);

		//���̃X���ɒ�~���ꂽ���Ƃ���������
		{
			POST	post;
			post["bbs"]  = ioPost["bbs"];
			post["FROM"]  = "YoujyoKiller��";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"�폜�˗��̎��������󗝂��܂���\r\n\r\n"
						"���̃X���b�h�̍폜�˗��� �u�ًc����v�J�L�R�����ꂽ�̂ō폜�˗��̓L�����Z������܂���\r\n"
						"�폜���Ȃ��������ׂĂ̐ӔC�́u�ًc����v�J�L�R�����l�ɂ���܂��̂ŁA\r\n"
						"���̌���ɕs���̐l�́u�ًc����v�J�L�R�����l�ɑ΂��čٔ��ł��Ȃ�ł��N�����Ă�������(w\r\n\r\n"
						" ============= �ًc����̓��e ===================\r\n"
						+ ioPost["MESSAGE"];
			post["key"]  = num2str(datnumber);
			post["submit"] = "��������";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
		//�폜�ΏۃX���b�h�ɍ폜�������ʒm������.
		{
			POST	post;
			post["bbs"]  = SettingTXT.getSAKUBBS();
			post["FROM"]  = "YoujyoKiller��";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"���̃X���b�h�ɑ΂��č폜�˗����������ꂽ�̂ŕ񍐂��܂��B\r\n\r\n"
						"�폜�˗��X���b�h " + ThisBoard() + num2str(datnumber) + "/l50\r\n\r\n"
						"��L�X���b�h�ɂ� �u�ًc����v�J�L�R�����ꂽ�̂ō폜�˗��̓L�����Z������܂���\r\n"
						"�폜���Ȃ��������ׂĂ̐ӔC�́u�ًc����v�J�L�R�����l�ɂ���܂��̂ŁA\r\n"
						"���̌���ɕs���̐l�́u�ًc����v�J�L�R�����l�ɑ΂��čٔ��ł��Ȃ�ł��N�����Ă�������(w\r\n\r\n"
						" ============= �ًc����̓��e ===================\r\n"
						+ ioPost["MESSAGE"];
			post["key"]  = num2str(targetdat);
			post["submit"] = "��������";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}

		//�폜�˗��̎�����.
		this->DeleteWait.Cancel(datnumber);

		//�������ݐ�����Ԃ�
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//�G���[��Ԃ�.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//�폜�l�^�X�N
void YBBS::KillerTask(char* outShareBuffer)
{
	//�폜�˗��łȂ���Ζv.
	if (!SettingTXT.getIS_SAKUBBS())	return ;

	//�폜�˗��̏�Ԃ����āA
	//�폜����ׂ��ł���΍폜����.
	DeleteWait.Check(outShareBuffer , this->BoardName , SettingTXT.getSAKUBBS() );
}

//DAT�K�y�[�W�R���N�^
void YBBS::GCTask()
{
	Cache.Collecter();
}


//�����o��.
void YBBS::Dump()
{
	Cache.Dump();
}

///���ׂĂ̏����f�B�X�N�Ƀt���b�V������.
void YBBS::Flush()
{
	Cache.Flush();
	Subject.Save(this->BoardName);
	DeleteWait.Save(this->BoardName);
}

//IP��������AID�\��������A mail�A�h���X�̉E�ɂ��邢�낢��ȏ������܂��B
const string YBBS::MakeAppendData(const RHTTPHeader* inRHTTPHeader ,const POST & iopPost) const
{
	if ( SettingTXT.getBBS_DISP_IP() )
	{	//IP�\��.
		return string(" Host:") + RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP());
	}
	//ID�\���͔�Ή��Ƃ������Ƃ�(w
	//�A���S���Y�����킩���A DES �Ȃ�? ���ꂩ�����Ă���.
	return "";
}

//���O�`�F�b�N
const string YBBS::CheckName(const RHTTPHeader* inRHTTPHeader ,const string & inName) const
{
	//���O������Ȃ疼��������.
	if (inName.empty() ) return SettingTXT.getBBS_NONAME_NAME();

	//�t�����x���Ȃ邩����Ȃ��̂Ŋe�� whois �Œ��ׂĂ�������(w
	const string ip = inRHTTPHeader->RemoteHost.getIP();

	//fusianasan�g���b�v.
	string name = RConv::Replace(inName , "fusianasan" , ip);
	//���͂����Ȃ��悤�ɂ���.
	name = RConv::Replace(name , "��" , "\"��\"");
	
	//�L���b�v�Ƃ����낢��͂܂��������Ă܂���B
	return name;
}


//�폜�˗����ꂽ�������݂̂�΂��P����}�X�N����.
void YBBS::MaskReWrite(DATNUMBER inDatNumber ,const string & password ,
	 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
	 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
}

