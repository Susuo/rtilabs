// YMain.cpp: YMain �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YMain.h"
#include "RStreamUtil.h"
#include "RStreamSocket.h"
#include "RHTTPHeader.h"
#include "YRequestPath.h"
#include "RHTTPPOSTParser.h"
#include "RNoComplateException.h"
#include "YThreadWithTls.h"
#include "RSocketForWinSock2.h"
#include "RSocketAsyncTransmitFile.h"
#include "YTransmitFile.h"
#include "YYoujyoLogger.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YMain::YMain()
{
	try
	{
		Mmap.Create(THREAD_WORK_AREA);
	}
	catch(RException e)
	{
		printf("�������}�b�v�m�ۂɎ��s �����ɏI�������Ă�������: %s\n",e.getMessage() );
	}
}

YMain::~YMain()
{
}

//���ꂪ���s�����̂ł���[
unsigned int YMain::Run()
{
	while( Execute() )
		;

	return 0;
}

//��~�𖽂���
//�����͂����ɋA���Ă��܂��B
//��~�t���O�𗧂Ă܂��B
void	YMain::HoldUp()
{
	YConnectNotify::getInstance()->PostShutdownNotify();
	RThread::HoldUp();
}



//���C������
bool YMain::Execute()
{
	YConnectPack* yConnectPack;
	YConnectNotify::getInstance()->WaitAcceptNotify(&yConnectPack );

//	printf("start %x\n" ,this);

	//�I�����߂������炠�ځ[��.
	if (yConnectPack == NULL)
	{
		return false;
	}

	RSocketForWinSock2 socket(yConnectPack->AcceptSocket);
//B
	try
	{
		char* buffer = (char*) Mmap.getMemory();

		//���s�R�[�h���A�����Ă���Ƃ���𔭌�����N���X.
		StreamCheckerForSecutiveReturnCode secutiveReturnCode;
		//�X�g���[������ǂݍ��ރ��[�e�B���e�B
		RStreamUtil	rsu;
		//�\�P�b�g���X�g���[���Ƃ��Ĉ������߂̃N���X.
		RStreamSocket rss(&socket);
		//�X�g���[���� mount ���܂��B
		rsu.Mount(&rss);

		//�A��������s�R�[�h�܂œǂݍ���.
		int readByte = rsu.ReadBuffer(buffer , THREAD_WORK_AREA , &secutiveReturnCode);

//C

		//�w�b�_�Ɩ{���𕪂����؂�̎擾.
		const int separater = secutiveReturnCode.getSeparater();

		//���N�G�X�g���.
		RHTTPHeader HeaderParser;
		HeaderParser.ReadBuffer(buffer , separater);
		HeaderParser.setRemoteHost(yConnectPack->RemoteIP);



//D

		int conectLength = 0;
		//�����w�肪����΁A���̒������ǂݍ���.
		if ( HeaderParser.ContentLength.IsEnable() )
		{
			/*ChangeLog.txt:2003/01/25	�w�b�_�̒����������Ă��Ȃ������̂ŏC�� >70 �ݸ��
			*/
			conectLength = HeaderParser.ContentLength.getValue();
			const int conectStart = secutiveReturnCode.getConectStart();
			//���͊��ɓǂݍ���ł��Ȃ����`�F�b�N.
			if (readByte < conectLength+conectStart)
			{
				StreamCheckerForLength scfl( conectLength+conectStart-readByte );
				readByte += rsu.ReadBuffer(buffer+readByte , THREAD_WORK_AREA - readByte ,&scfl);
			}
		}
//E
		try
		{
			YRequestPath	RequestPath;
			if ( ! RequestPath.PathParse( HeaderParser.getPath() ) )
			{//2ch�`���ȊO�̃A�N�Z�X�A �ʏ�̃t�@�C���]���Ƃ݂Ȃ�.
				YTransmitFile	tf;
				tf.TransmitFile(buffer ,&HeaderParser,&socket);
			}
			else
			{
				//2ch�`���̃A�N�Z�X.

				if (HeaderParser.getMethod() == "GET")
				{//GET ���\�b�h
					YBBSManager::getInstance()->Read(buffer , &HeaderParser , &RequestPath, &rsu);
				}
				else if (HeaderParser.getMethod() == "POST")
				{//POST ���\�b�h
					//POST���ꂽ���e�̃`�F�b�N.
					RHTTPPOSTParser rhpp;
					//�R���e���c�J�n�ʒu
					const int conectStart = secutiveReturnCode.getConectStart();
					//�R���e���c�̒���(���ۂɓǂݍ��񂾗� - �R���e���c�J�n�ʒu)
					int tureContentSize = readByte-conectStart;
					//Conetent-Length ���w�肳��Ă���� ����ɂ���.
					if (conectLength < tureContentSize)	tureContentSize = conectLength;

					//�R���e���c�p�[�X
					rhpp.Parse(buffer+conectStart , tureContentSize );

//					rhpp.Dump();

					//submit=%8F%91%82%AB%8D%9E%82%DE&FROM=&mail=sage&MESSAGE=%8D%A1%93%FA%82%CD%8E%9E%8A%D4%82%B8%82%EA%82%E9%82%CC%82%A9&bbs=choanitoku&key=1038415253&time=1039048481
					if (RequestPath.getRequestType() != REQUESTTYPE_BBSCGI)
					{//�G���[
						throw RNoComplateException("�s���� CGI �ɑ΂��� POST�����s����܂���");
					}
					YBBSManager::getInstance()->Write(buffer ,&HeaderParser, rhpp.PostData , &rsu);
				}
				else
				{//����ȊO
					rsu.WriteBuffer("HTTP/1.1 501 Not Implemented\n\n");
				}
			}
		}
		catch(RException e)
		{
			rsu.WriteBuffer("HTTP/1.1 403 Forbidden\n\n");
			throw ;
		}
		socket.Close();
		//�񓯊��̈�����ځ[��
		delete yConnectPack;

//		string* logString = new string(RDateConv::UnixTime2chTime(time(NULL))  + " " +  HeaderParser.RemoteHost.getIP() + " " + HeaderParser.getMethod() + " " + HeaderParser.getPath() + "\r\n");
//		YLogNotify::getInstance()->Post( logString  );
	}
	catch(RException e)
	{
		socket.Close();
		//�񓯊��̈�����ځ[��
		delete yConnectPack;
//		printf("��� %s\n" , e.getMessage() );
	}
//	::Sleep(1000);
//	printf("end %x\n" ,this);
	return true;
}

