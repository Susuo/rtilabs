// RSocketForWinSock2.cpp: RSocketForWinSock2 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RSocketForWinSock2.h"
#include <stdio.h>
#include <sys\timeb.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

//��M�o�b�t�@�̍ő�l.
const int RESERVE_BUFFER_SIZE	= 50000;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

/**
 * RSocketForWinSock2:		�R���X�g���N�^
 *				�Ƃ肠�������������܂��B
 */
RSocketForWinSock2::RSocketForWinSock2()
{
	Socket = INVALID_SOCKET;
	IsConnect = false;
}
/**
 * RSocketForWinSock2:		�R���X�g���N�^
 *				�w�肳�ꂽ�\�P�b�g�ŏ��������܂��B
 *
 * @param inNewSocket	���ɂȂ����Ă���\�P�b�g.
 * @return  
 */
RSocketForWinSock2::RSocketForWinSock2(SOCKET inNewSocket)
{
	Socket = inNewSocket;
	IsConnect = true;
}


/**
 * ~RSocketForWinSock2:	�f�X�g���N�^
 *				
 */
RSocketForWinSock2::~RSocketForWinSock2()
{
}

/**
 * Open:			�\�P�b�g���쐬����.
 *
 * @param type		�\�P�b�g�̎��
 *						SOCK_STREAM : TCP�Ŏg�p����
 *						SOCK_DGRAM : UDP�Ŏg�p����
 * @param protocol	�g�p����v���g�R��
 * @throws			RIOCannotOpenException		�J���Ȃ������Ƃ�
 */
void RSocketForWinSock2::Open( int type, int protocol ) throw(RIOCannotOpenException)
{
	Socket = socket(AF_INET,type,protocol);
	if (Socket == INVALID_SOCKET || Socket == NULL) throw RIOCannotOpenException(EXCEPTIONTRACE + "�\�P�b�g���J���܂���ł���" + ErrorToMesage() );

	// Winsock�Ŕ�u���b�L���O�̃\�P�b�g�ʐM���s���悤�ɐݒ肷��.
	unsigned long argp = 1;   // ��u���b�L���O�ɐݒ� 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "��u���b�L���O���[�h�ֈړ��ł��܂���" + ErrorToMesage() );
	}

}

/**
 * Connect:				�ڑ�(�ȈՔ�)
 *
 * @param inHost		�z�X�g��
 * @param inPort		�|�[�g
 * @throws				RIOException			�ڑ��ł��Ȃ������Ƃ�
 */
void RSocketForWinSock2::Connect(string inHost , int inPort)	throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//���łɐڑ����Ă��܂��B
	if (IsConnect)	return ;

	LPHOSTENT	HostEnt;
	SOCKADDR_IN	sai = {0};
	in_addr		iadr;

	iadr.s_addr=inet_addr( inHost.c_str() );
	if(iadr.s_addr==INADDR_NONE)
	{
		HostEnt=gethostbyname(inHost.c_str() );
		if(!HostEnt)
		{
			throw RIOException(EXCEPTIONTRACE + "�z�X�g���������ł��܂���ł���" + ErrorToMesage() );
		}
		sai.sin_addr=*((LPIN_ADDR)*HostEnt->h_addr_list);
	}
	else
	{
		sai.sin_addr = iadr;
	}

	sai.sin_port=htons((unsigned short)inPort); // �ϊ���Y�ꂸ��!
	sai.sin_family=AF_INET;

	this->Connect(&sai) ;
}

/**
 * Connect:			�ڑ�
 *
 * @param inSai		�ڑ����
 * @throws			RIOException			�ڑ��ł��Ȃ������Ƃ�
 */
void RSocketForWinSock2::Connect(const SOCKADDR_IN * inSai) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//���łɐڑ����Ă��܂��B
	if (IsConnect)	return ;

	// Winsock�Ŕ�u���b�L���O�̃\�P�b�g�ʐM���s���悤�ɐݒ肷��.
	unsigned long argp = 1;   // ��u���b�L���O�ɐݒ� 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "��u���b�L���O���[�h�ֈړ��ł��܂���" + ErrorToMesage() );
	}

	//�ڑ������݂�.
	int ConnectRet = connect(Socket,(LPSOCKADDR)inSai,sizeof(SOCKADDR_IN));
	if( ! (ConnectRet == 0 || ConnectRet == -1) ) //����(0) ���A�܂��I����Ă��Ȃ�(-1)�łȂ��Ȃ��...
	{	//�ڑ��Ɏ��s���܂���.
		string error = ErrorToMesage();	//�u���b�N���[�h�ɖ߂��Ƃ��ɃG���[���㏑������Ȃ��悤�ɐ�Ɏ擾.

		argp = 0;   // �u���b�L���O�ɐݒ� 
		ioctlsocket( Socket , FIONBIO , &argp );

		throw RIOException(EXCEPTIONTRACE + "�ڑ��Ɏ��s���܂���" + error );
	}

	try
	{
		//�������݂��ł���悤�ɂȂ�܂őҋ@.
		//�^�C���A�E�g����������� RIOException ���N���܂��B
		HoldTillReadyForWrite( 60 ) ;
	}
	catch(...)
	{
		argp = 0;   // �u���b�L���O�ɐݒ� 
		ioctlsocket( Socket , FIONBIO , &argp );
		throw ;
	}

	argp = 0;   // �u���b�L���O�ɐݒ� 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "�u���b�L���O���[�h�ֈړ��ł��܂���" + ErrorToMesage() );
	}

	IsConnect = true;
}

/**
 * Bind:			bind(�ȈՔ�)
 *
 * @param inPort	bind����|�[�g
 * @throws			RIOException		bind���s
 */
void RSocketForWinSock2::Bind(int inPort) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//���ł�bind���Ă��܂��B
	if (IsConnect)	return ;

	int	Opt = 1;
	setsockopt(Socket , SOL_SOCKET, SO_REUSEADDR, (const char*)&Opt , sizeof(int) );

	SOCKADDR_IN	sai = {0};
	sai.sin_family			= AF_INET;
	sai.sin_addr.s_addr	= htonl(INADDR_ANY);
	sai.sin_port			= htons((unsigned short)inPort);

	this->Bind( &sai );
}

/**
 * Bind:			bind
 *
 * @param inSai		bind���
 * @throws			RIOException		bind���s
 */
void RSocketForWinSock2::Bind(const SOCKADDR_IN * inSai) throw(RIOException)
{
	//���ł�bind���Ă��܂��B
	if (IsConnect)	return ;

	int ret = bind(Socket , (struct sockaddr*) inSai , sizeof(sockaddr) );
	if (ret < 0)
	{
		throw RIOException(EXCEPTIONTRACE + "Bind �Ɏ��s���܂���" + ErrorToMesage() );
	}

	IsConnect = true;
}

/**
 * Listen:					listen
 *
 * @param inBacklog			�ڑ��v�����O��ۑ�����i�L���[�C���O����j�ő吔
 * @throws	RIOException	listen���s	
 */
void RSocketForWinSock2::Listen(int inBacklog ) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	int ret = listen(Socket , inBacklog );
	if (ret < 0)
	{
		throw RIOException(EXCEPTIONTRACE + "Listen �Ɏ��s���܂���" + ErrorToMesage() );
	}
}

/**	
 * Accept:					accept
 *
 * @throws	RIOException	accept���s
 */
void RSocketForWinSock2::Accept(SOCKET inAcceptSocket ,
								PVOID lpOutputBuffer, DWORD * outReadByte,OVERLAPPED * outOverlapped) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

    if (!AcceptEx(Socket, inAcceptSocket,
                  lpOutputBuffer,
                  0,
                  PADDED_ADDR_SIZE, 
                  PADDED_ADDR_SIZE,
                  outReadByte,
                  outOverlapped))
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return ;

		WSASetLastError(error);
		throw RIOException(EXCEPTIONTRACE + "accept �Ɏ��s���܂���" + ErrorToMesage() );
	}

}

/**
 * HoldTillReadyForWrite:		�������݂��ł���悤�ɂȂ�܂őҋ@.
 *
 * @param	inTimeOutS			�^�C���A�E�g����b��
 * @throws	RIOException	�^�C���A�E�g.
 */
void RSocketForWinSock2::HoldTillReadyForWrite(long inTimeOutS) throw(RIOException)
{
	timeval UseTimeVal;
	FD_SET	WriteFlg;

	//�Z���N�g�̖߂�l
	int	SelectRet;	
	int i;
	//�Z�����ԂŃ`�F�b�N���܂�.
	for(i = 0 ; i < inTimeOutS ; i++)
	{
		//�b.
		UseTimeVal.tv_sec  = 1;
		//�bms
		UseTimeVal.tv_usec = 0;
		FD_ZERO(&WriteFlg);
		FD_SET(Socket , &WriteFlg);

		SelectRet = select(2 , NULL , &WriteFlg , NULL , &UseTimeVal);
		//�^�C���A�E�g�ł͂Ȃ������烋�[�v���E.
		if (SelectRet != 0) break;	
	}
	//�^�C���A�E�g�ł�.
	if (i == inTimeOutS)
	{
		throw RIOException(EXCEPTIONTRACE + "�^�C���A�E�g���������܂���.");
	}

	if ( ! FD_ISSET(Socket , &WriteFlg) )
	{//�~�X.
		throw RIOException(EXCEPTIONTRACE + "�����s���̃G���[���������܂���" + ErrorToMesage() );
	}
}

/**
 * ShutDown:	�ؒf.
 *
 */
void RSocketForWinSock2::ShutDown()
{
	if (Socket == INVALID_SOCKET) return ;
	if (!IsConnect)	return ;

	shutdown(Socket , RSocketForWinSock2::sends);

	//���̎��ԂɂȂ��Ă��Ȃ��ł���ꍇ�́A���؂�.
	DWORD TimeOutTime = IntTime() + 10; //10�b�҂�.

	//�ۗ����̂��ׂẴf�[�^���󂯎��.
	int		Ret;
	char	Buffer[RESERVE_BUFFER_SIZE];
	while(1)
	{
		Ret = recv(Socket , Buffer , RESERVE_BUFFER_SIZE*sizeof(char) , 0 );
		//TRACE("Shutdown �ۗ����̃f�[�^ %d ����M���܂���\r\n",Ret);
		if ( Ret <= 0 || Ret == SOCKET_ERROR)	break;

		//�^�C���A�E�g�`�F�b�N
		if ( TimeOutTime < IntTime() ) 
		{
			//TRACE("Shutdown �^�C���A�E�g���������܂���\r\n");
			break;
		}
	}
	//TRACE("Shutdown ���Ă΂�܂���3");
	//�f�[�^������ȏ��M���Ȃ����Ƃ𑊎�ɓ`����.
	shutdown(Socket , RSocketForWinSock2::both);

	IsConnect = false;
}

/**
 * Close:	�\�P�b�g�N���[�Y.
 *
 * @throws	RIOCannotCloseException		�ؒf�Ɏ��s
 */
void RSocketForWinSock2::Close() throw(RIOCannotCloseException)
{
	if(Socket == INVALID_SOCKET) return ;

	//�ؒf����.
//	ShutDown();

	if( closesocket(Socket) )
	{
		throw RIOException(EXCEPTIONTRACE + "�\�P�b�g�̃N���[�Y���ɃG���[���������܂���" + ErrorToMesage() );
	}
	Socket = INVALID_SOCKET;

}

/**
 * Send:				���M
 *
 * @param ioParam		�Ȃ�
 * @return int 
 * @throw	RIOCannotWriteException
 */
int RSocketForWinSock2::Send( AsyncParam * ioParam ) throw(RIOCannotWriteException)
{
	ASSERT(ioParam != NULL);
	if (this->Socket == INVALID_SOCKET) return 0;  

	int ret = ::WSASend( this->Socket, & ioParam->Buffer ,  1,
				&ioParam->Size,  ioParam->Flag,  &ioParam->Overlapped,  NULL	);
	if (ret < 0)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return 0;

		WSASetLastError(error);
		throw RIOCannotWriteException(EXCEPTIONTRACE + "���M���ɃG���[���������܂���" + ErrorToMesage() );
	}

	return ret;
}

bool RSocketForWinSock2::TransmitFile( HANDLE	inFileHandle , int inNumberOfBytesToWrite , int inNumberOfBytesPerSend,
										TRANSMIT_FILE_BUFFERS * inTransmitFileBufers , AsyncParam * ioParam )
										throw (RIOCannotReadException)
{
	bool ret = Btob(::TransmitFile(this->Socket , 
			inFileHandle ,
			inNumberOfBytesToWrite ,
			inNumberOfBytesPerSend ,
			&ioParam->Overlapped ,
			inTransmitFileBufers ,
			0 ) );
	if (ret == false)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING || error == WSA_IO_PENDING) return false;

		WSASetLastError(error);
		throw RIOCannotReadException(EXCEPTIONTRACE + "��M���ɃG���[���������܂���" + ErrorToMesage() );
	}

	return true;
}

/**
 * Reserve:				��M.
 *
 * @param ioParam		�Ȃ�
 * @return int 
 * @throw	RIOCannotReadException
 */
int RSocketForWinSock2::Reserve( AsyncParam * ioParam ) throw(RIOCannotReadException)
{
	ASSERT(ioParam != NULL);
	if (this->Socket == INVALID_SOCKET) return 0;  

	int ret = ::WSARecv( this->Socket, & ioParam->Buffer ,  1,
				&ioParam->Size,  &ioParam->Flag,  &ioParam->Overlapped,  NULL	);
	if (ret < 0)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return 0;

		WSASetLastError(error);
		throw RIOCannotReadException(EXCEPTIONTRACE + "��M���ɃG���[���������܂���" + ErrorToMesage() );
	}

	return ret;
}

/**
 * WaitForSignal:		��M�A���M����܂ő҂�.
 *
 * @param ioParam		�Ȃ�
 * @param inTimeOut		�^�C���A�E�g.
 * @return void 
 * @throw	RIOException
 */
void RSocketForWinSock2::WaitForSignal(AsyncParam * ioParam,int inTimeOut ) throw(RIOException)
{
	ASSERT(ioParam != NULL);

	const DWORD ret = WSAWaitForMultipleEvents(1 ,& (ioParam->Overlapped.hEvent), TRUE, inTimeOut,TRUE);
	if (ret == WSA_WAIT_FAILED )
	{
		throw RIOException(EXCEPTIONTRACE + "WSAWaitForMultipleEvents���G���[��Ԃ��܂���" + ErrorToMesage() );
	}
	if (ret == WSA_WAIT_TIMEOUT)
	{
		throw RIOException(EXCEPTIONTRACE + "�^�C���A�E�g���������܂���.");
	}
}

/**
 * WSAGetOverlappedResult:		��M�A���M�������ʂ��󂯎��.
 *
 * @param ioParam				�Ȃ�
 * @return void 
 * @throw	RIOException
 */
void RSocketForWinSock2::WSAGetOverlappedResult(AsyncParam * ioParam) throw(RIOException)
{
	ASSERT(ioParam != NULL);

	if ( ::WSAGetOverlappedResult(this->Socket , &ioParam->Overlapped , &ioParam->Size ,TRUE , &ioParam->Flag ) == FALSE)
	{
		throw RIOException(EXCEPTIONTRACE + "WSAGetOverlappedResult���G���[��Ԃ��܂���" + ErrorToMesage() );
	}
}




/**
 * IntTime:	�^�C���A�E�g�Ɏg�����Ԃ̒�.
 *
 * @return DWORD	UNIX����
 */
DWORD RSocketForWinSock2::IntTime()
{
	struct _timeb Time;
	_ftime(&Time);

	return Time.time;
}
/**
 * ErrorToMesage:	�G���[���b�Z�[�W���擾����.
 *
 * @return string	�G���[���b�Z�[�W
 */
string RSocketForWinSock2::ErrorToMesage()
{

	//http://kaf.tri6.net/code/wsock_r.html
	DWORD error = WSAGetLastError();
	if (error == 0) return "";
	switch(error)
	{
	case WSAEINTR: return string(" �֐��Ăяo�������f���ꂽ�B\nWinSock 1.1 �̃u���b�L���O�֐��Ăяo�����AWSACancelBlockingCall() �ɂ���Ē��f���ꂽ�B");
	case WSAEBADF: return string("�����ȃt�@�C���n���h���B\n�\�P�b�g�֐��ɓn���ꂽ�t�@�C���n���h���i�\�P�b�g�n���h���j���s���ł���B�iWSAENOTSOCK�Ɠ��`�ŗp������j");
	case WSAEACCES: return string("�A�N�Z�X�����ۂ��ꂽ�B\nconnect()�F�u���[�h�L���X�g�A�h���X�Őڑ����悤�Ƃ����\�P�b�g�� SO_BROADCAST�I�v�V���� ���L���ɂȂ��Ă��Ȃ��B\nsendto()�F�u���[�h�L���X�g�A�h���X�ɑ��M���悤�Ƃ����\�P�b�g�� SO_BROADCAST�I�v�V�������L���ɂȂ��Ă��Ȃ��B\naccept()�F��t���悤�Ƃ����ڑ��v�������łɃ^�C���A�E�g�ɂȂ��Ă���A�������͎�������Ă���B");
	case WSAEFAULT: return string("�����ȃo�b�t�@�A�h���X\n�֐��ɓn���ꂽ�o�b�t�@���w���|�C���^�������ȃA�h���X��Ԃ��w���Ă���B�܂��̓o�b�t�@�̃T�C�Y������������B");
	case WSAEINVAL: return string("�����Ȉ������n���ꂽ�B\nlisten()���Ăяo������accept()���Ăяo�����B\n�o�C���h�ς݂̃\�P�b�g�ɑ΂���bind()���悤�Ƃ����B\nlisten()�őҋ@���̃\�P�b�g��connect()���悤�Ƃ����B\n�֐��ɓn���ꂽ�p�����[�^�ɖ����Ȓl���܂܂�Ă���B");
	case WSAEMFILE: return string("�g�p���̃\�P�b�g�̐�����������B\n�����ɗ��p�ł���\�P�b�g���̍ő�ɒB���Ă���A�V���Ƀ\�P�b�g���쐬�ł��Ȃ��B");
	case WSAEWOULDBLOCK: return string("����̓u���b�L���O�����B\n�m���u���b�L���O���[�h�Ń\�P�b�g�֐����Ăяo���ꂽ�߁A�֐��͑����Ƀ��^�[���������v�����ꂽ����͊������Ă��Ȃ��B");
	case WSAEINPROGRESS: return string("���łɃu���b�L���O�葱�������s����Ă���B\n�u���b�L���O�֐������s���ł��邩�A�T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������ł���BWinSock�ł͂P�̃v���Z�X�œ����ɕ����̃u���b�L���O��������邱�Ƃ͂ł��Ȃ����߁A�O��Ăяo�������삪���S�ɏI������܂Ŏ��̑�����J�n���邱�Ƃ͂ł��Ȃ��B");
	case WSAEALREADY: return string("�v�����ꂽ����͊��Ɏ��s���A�܂��͎��s�ς݁B\n�L�����Z�����悤�Ƃ����񓯊����삪���ɃL�����Z������Ă���B\nconnect()���Ăяo�������A���ɑO��̌Ăяo���ɂ���Đڑ��������ł���B");
	case WSAENOTSOCK: return string("�w�肳�ꂽ�\�P�b�g�������ł���B\n�\�P�b�g�n���h���Ƃ��Ė����Ȓl���\�P�b�g�֐��ɓn���ꂽ�B");
	case WSAEMSGSIZE: return string("���b�Z�[�W�T�C�Y���傫������B\n���M�A�܂��͎�M���悤�Ƃ������b�Z�[�W���A��ՂƂȂ�v���g�R���̃T�|�[�g���Ă���ő僁�b�Z�[�W�T�C�Y�𒴂��Ă���Bioctlsocket()�Ńo�b�t�@�T�C�Y�𑝂₷���Ƃŉ���\�Ȃ��Ƃ�����B");
	case WSAEPROTOTYPE: return string("�\�P�b�g�͗v�����ꂽ�v���g�R���ɓK�����Ă��Ȃ��B\nsocket()���Ăяo���Ƃ��Ɏw�肵���v���g�R���^�C�v���\�P�b�g�^�C�v�ƓK�����Ă��Ȃ��B���b�Z�[�W�^�v���g�R���ŃX�g���[���\�P�b�g���쐬���悤�Ƃ����ꍇ�ȂǁB");
	case WSAENOPROTOOPT: return string("�s���ȃv���g�R���I�v�V�����B\n�w�肵���\�P�b�g�I�v�V�����܂���ioctlsocket()�ł̑��삪�A���̃v���g�R���ł̓T�|�[�g����ĂȂ��B\n�X�g���[���\�P�b�g�� SO_BROADCAST ���w�肵�悤�Ƃ����B");
	case WSAEPROTONOSUPPORT: return string("�v���g�R�����T�|�[�g����Ă��Ȃ��B\n�T�|�[�g����Ă��Ȃ��v���g�R�����w�肵���A�܂��̓v���g�R���̃T�|�[�g���Ă��Ȃ���������s���悤�Ƃ����B");
	case WSAESOCKTNOSUPPORT: return string("�w�肳�ꂽ�\�P�b�g�^�C�v�̓T�|�[�g����Ă��Ȃ��B");
	case WSAEOPNOTSUPP: return string("�v�����ꂽ����̓T�|�[�g����Ă��Ȃ��B\n�o���h�O�f�[�^�𑗐M/��M���悤�Ƃ����\�P�b�g�ł��ꂪ�T�|�[�g����Ă��Ȃ��B\n��ڑ��^(UDP�Ȃ�)�\�P�b�g��accept()���Ăяo�����B");
	case WSAEPFNOSUPPORT: return string("�v���g�R���t�@�~�����T�|�[�g����Ă��Ȃ��B");
	case WSAEAFNOSUPPORT: return string("�A�h���X�t�@�~�����T�|�[�g����Ă��Ȃ��B");
	case WSAEADDRINUSE: return string("�A�h���X�͊��Ɏg�p���ł���B\nbind()���悤�Ƃ����A�h���X�́A���ɂق��̃\�P�b�g�Ŏg���Ă���B�������[�J���A�h���X�𕡐��̃\�P�b�g�Ŏg�����߂ɂ́ASO_REUSEADDR�\�P�b�g�I�v�V������TRUE�ɂ���K�v������B");
	case WSAEADDRNOTAVAIL: return string("�����ȃl�b�g���[�N�A�h���X�B\n�\�P�b�g�֐��ɓn���ꂽ�l�b�g���[�N�A�h���X�ɖ����ȕ���������B");
	case WSAENETDOWN: return string("�l�b�g���[�N���_�E�����Ă���B\n�l�b�g���[�N�V�X�e�������炩�̏�Q���N�����A�@�\��~���Ă��邽�ߗv�����ꂽ���삪���s�ł��Ȃ��B");
	case WSAENETUNREACH: return string("�w�肳�ꂽ�l�b�g���[�N�z�X�g�ɓ��B�ł��Ȃ��B\nWSAEHOSTUNREACH�Ɠ��`�Ŏg���Ă���B");
	case WSAENETRESET: return string("�l�b�g���[�N�ڑ����j�����ꂽ�B\n�L�[�v�A���C�u���s���Ă���ڑ��ŁA���炩�̏�Q�̂��߃^�C���A�E�g�����o����ڑ�������ꂽ�B");
	case WSAECONNABORTED: return string("�l�b�g���[�N�ڑ����j�����ꂽ�B");
	case WSAECONNRESET: return string("�l�b�g���[�N�ڑ�������ɂ���Ĕj�����ꂽ�B");
	case WSAENOBUFS: return string("�o�b�t�@���s�����Ă���B\n�������s���̂���WinSock������ɕK�v�ȃo�b�t�@���m�ۂ��邱�Ƃ��ł��Ȃ��B�܂��́A�o�b�t�@��K�v�Ƃ���֐��Ăяo���ŁA����������o�b�t�@���n���ꂽ�B");
	case WSAEISCONN: return string("�\�P�b�g�͊��ɐڑ�����Ă���B\n�ڑ����̃\�P�b�g�ɑ΂���connect()�Alisten()�Abind()���̑�����s�����Ƃ����B\n�ڑ����̃\�P�b�g��sendto()�����s���悤�Ƃ����B");
	case WSAENOTCONN: return string("�\�P�b�g�͐ڑ�����Ă��Ȃ��B\n�ڑ�����Ă��Ȃ��\�P�b�g��send()�Agetpeername()���̐ڑ����ꂽ�\�P�b�g��K�v�Ƃ��鑀������s���悤�Ƃ����B");
	case WSAESHUTDOWN: return string("�\�P�b�g�̓V���b�g�_�E������Ă���B\n���ɃV���b�g�_�E������đ��M/��M����~����Ă���\�P�b�g�ŁA���M�܂��͎�M�̑�������s���悤�Ƃ����B");
	case WSAETOOMANYREFS: return string("�Q�Ƃ̐�����������B");
	case WSAETIMEDOUT: return string("�ڑ��v�����^�C���A�E�g�����B\nconnect()���Ăяo���Đڑ������݂����A�i����̉������Ȃ����Łj�������^�C���A�E�g�ɂȂ�ڑ��v�����L�����Z�����ꂽ�B");
	case WSAECONNREFUSED: return string("�ڑ������ۂ��ꂽ�B");
	case WSAELOOP: return string("���[�v�B");
	case WSAENAMETOOLONG: return string("���O����������B");
	case WSAEHOSTDOWN: return string("�z�X�g���_�E�����Ă���B\nWSAETIMEDOUT�Ɠ��`�B");
	case WSAEHOSTUNREACH: return string("�z�X�g�ւ̌o�H���Ȃ��B\n�l�b�g���[�N�̍\���㓞�B�ł��Ȃ��z�X�g�ɑ΂��đ�������s���悤�Ƃ����B�܂��̓A�h���X���s���ł���B");
	case WSAENOTEMPTY: return string("�f�B���N�g������ł͂Ȃ��B");
	case WSAEPROCLIM: return string("�v���Z�X�̐�����������B\nWSAStartup()�F����WinSock���Ǘ��ł���ő�v���Z�X���ɒB���Ă��菈�������s�ł��Ȃ��B");
	case WSAEUSERS: return string("���[�U�[�̐�����������B");
	case WSAEDQUOT: return string("�f�B�X�N�N�H�[�^�B");
	case WSAESTALE: return string("���s���悤�Ƃ�������͔p�~����Ă���B");
	case WSAEREMOTE: return string("�����[�g�B");
	case WSASYSNOTREADY: return string("�l�b�g���[�N�T�u�V�X�e�������p�ł��Ȃ��B\nWSAStartup()�F�l�b�g���[�N�T�u�V�X�e�������p�ł��Ȃ��A�܂��͐������Z�b�g�A�b�v����Ă��Ȃ����ߋ@�\���Ă��Ȃ��B");
	case WSAVERNOTSUPPORTED: return string("Winsock.dll�̃o�[�W�������͈͊O�ł���B\nWSAStartup()�F�v������WinSock�o�[�W�����́A���݂̎����ł̓T�|�[�g����Ă��Ȃ��B");
	case WSANOTINITIALISED: return string("WinSock�V�X�e��������������Ă��Ȃ��B\nWinSock�֐������s���悤�Ƃ������AWSAStartup()������Ɏ��s����Ă��Ȃ����ߋ@�\���Ȃ��B");
	case WSAEDISCON: return string("�V���b�g�_�E���������B\n�ڑ�������̏����ɂ���Đؒf����悤�Ƃ��Ă���B");
	case WSAENOMORE: return string("�f�[�^�͂���ȏ㑶�݂��Ȃ��B\nWSA_E_NO_MORE�Ɠ��`�B");
	case WSAECANCELLED: return string("����͎������ꂽ�B\nWSA_E_CANCELLED�Ɠ��`�B");
	case WSAEINVALIDPROCTABLE: return string("�T�[�r�X�v���o�C�_�̊֐��e�[�u���������B");
	case WSAEINVALIDPROVIDER: return string("�T�[�r�X�v���o�C�_�������B");
	case WSAEPROVIDERFAILEDINIT: return string("�T�[�r�X�v���o�C�_�̏������Ɏ��s�����B");
	case WSASYSCALLFAILURE: return string("�V�X�e���R�[���Ɏ��s�����B\n�����Ď��s���Ȃ��͂��̃V�X�e���R�[�������炩�̗��R�ɂ�莸�s�����B");
	case WSASERVICE_NOT_FOUND: return string("�T�[�r�X��������Ȃ��B\n�v�����ꂽ�T�[�r�X���W���I�ȃT�[�r�X�łȂ��A�w�肵�����O��Ԃ���T�[�r�X�������邱�Ƃ��ł��Ȃ��B");
	case WSATYPE_NOT_FOUND: return string("�^�C�v��������Ȃ��B");
	case WSA_E_NO_MORE: return string("�f�[�^�͂���ȏ㑶�݂��Ȃ��B\nWSALookupServiceNext()�F���͂���ȏ㑶�݂��Ȃ��B");
	case WSA_E_CANCELLED: return string("�������L�����Z�����ꂽ�B\nWSALookupServiceNext()�F�T�[�r�X�̌����r���ɁAWSALookupServiceEnd()�ɂ���ď������L�����Z�����ꂽ�B");
	case WSAEREFUSED: return string("����͋��ۂ��ꂽ�B");
	case WSAHOST_NOT_FOUND: return string("�z�X�g��������Ȃ��B\nDNS�Ȃǂ̖��O�����T�[�r�X���w�肳�ꂽ�z�X�g���������Ȃ������B�v���g�R����T�[�r�X�̃N�G���[����ɂ����Ă��Ԃ����B");
	case WSATRY_AGAIN: return string("�w�肳�ꂽ�z�X�g��������Ȃ��A�܂��̓T�[�r�X�ُ̈�B");
	case WSANO_RECOVERY: return string("�񕜕s�\�ȃG���[�����������B\n�f�[�^�x�[�X�����̏ꍇ�́A���O�����T�[�r�X���g�p�ł��Ȃ����Ƃ��Ӗ�����B");
	case WSANO_DATA: return string("�v�����ꂽ�^�C�v�̃f�[�^���R�[�h��������Ȃ��B");
	default:	return string("����`�̃G���[" + num2str(error) + "���������܂���");

	}
}
 