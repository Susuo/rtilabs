// YServer.cpp: YServer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YServer.h"
#define SOCKBUF_SIZE	(256 * 1024)

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YServer::YServer()
{
}

YServer::~YServer()
{
}

/**
 * Run:	�T�[�o���s.
 *
 * @return virtual unsigned int 
 */
unsigned int YServer::Run()
{
	try
	{
		ListenSocket.Open(SOCK_STREAM,0);
		ListenSocket.Bind( YHttpdConf::getInstance()->getListenPort() );
		ListenSocket.Listen();

	}
	catch(RException e)
	{
		printf("���������ɗ�O�����m���܂��� : %s\n" ,e.getMessage() );
		ListenSocket.Close();
		return 0;
	}

	YConnectPack* ao = NULL;
	try
	{
		char		buff[2*PADDED_ADDR_SIZE];	
		DWORD		readByte;

		YConnectNotify* ycn = YConnectNotify::getInstance();
		while( this->IsHoldUp() )
		{
			if (ao == NULL)
			{
				ao = new YConnectPack ;
				ao->AcceptSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

				// Winsock�Ŕ�u���b�L���O�̃\�P�b�g�ʐM���s���悤�ɐݒ肷��.
				unsigned long argp = 1;   // ��u���b�L���O�ɐݒ� 
				ioctlsocket( ao->AcceptSocket  , FIONBIO , &argp );
				//���M�o�b�t�@���g��
				int buf_size = SOCKBUF_SIZE;
				for ( ; buf_size > 0; buf_size /= 2)
				{
					if (setsockopt(ao->AcceptSocket, SOL_SOCKET, SO_SNDBUF, (char *)&buf_size, sizeof(buf_size)) == 0)	break;
				}
			}

			ListenSocket.Accept(ao->AcceptSocket , buff , &readByte , &ao->OverLapped);

			DWORD ret = WaitForSingleObject(ao->OverLapped.hEvent, INFINITE);
			if (ret != WAIT_OBJECT_0)
			{
				if (this->IsHoldUp())
				{
					closesocket(ao->AcceptSocket);
					delete ao;
					ao = NULL;
					break;
				}
			}
			else
			{
				//���łɕ����Ă���.
				if (ao->AcceptSocket == INVALID_SOCKET)
				{
//					printf("accept �����\�P�b�g(%x)�͂��łɕ����Ă��܂�\r\n",ao->AcceptSocket);
					continue;	//Accept ���Ȃ���.
				}
				//���ʂ��󂯎��.
				if (!GetOverlappedResult((HANDLE)ao->AcceptSocket , &ao->OverLapped, &readByte, FALSE))
				{
					//???
//					printf("accept �����\�P�b�g(%x)�̌��ʂ��󂯎��܂���ł���\r\n",ao->AcceptSocket);

					//���̃\�P�b�g�ɑ΂���ė��p�͕s�B
					closesocket(ao->AcceptSocket);
					delete ao;
					ao = NULL;

					continue;	//Accept ���Ȃ���.
				}
				//�����[�g�z�X�g�����߂�.
				{
					SOCKADDR*			localSocket;
					int					localSocketLen;
					SOCKADDR*			remoteSocket;
					int					remoteSocketLen;
					//IP��������ק.
					GetAcceptExSockaddrs(buff , 0 ,PADDED_ADDR_SIZE , PADDED_ADDR_SIZE ,
							&localSocket , &localSocketLen , &remoteSocket , &remoteSocketLen);
					sockaddr_in* addrin = (sockaddr_in*)remoteSocket;
					ao->RemoteIP = addrin->sin_addr;
				}
				//��M�������S���@
				ycn->PostAcceptNotify(ao);
				ao = NULL;
			}
		}
	}
	catch(RException e)
	{
		printf("���s����O���A�I�����荞�݂����m���܂���. : %s\n" ,e.getMessage() );
	}

	if (ao)
	{
		closesocket(ao->AcceptSocket);
		delete ao;
	}

	return 0;
}


/**
 * Shutdown:	��~���ߑ��M.
 *
 */
void YServer::Shutdown()
{
	ListenSocket.Close();
	this->HoldUp();
}

