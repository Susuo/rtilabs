// RSocketForWinSock2.h: RSocketForWinSock2 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_)
#define AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"
#include "RWin32Exception.h"

const int PADDED_ADDR_SIZE = sizeof(SOCKADDR_IN)+16;

class AsyncParam
{
public:
	AsyncParam()
	{
		Overlapped.Offset = 0;
		Overlapped.OffsetHigh = 0;
		Overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~AsyncParam()
	{
		CloseHandle(Overlapped.hEvent);
	}


	WSABUF			Buffer;
	WSAOVERLAPPED	Overlapped;
	DWORD			Size;
	DWORD			Flag;
};


class RSocketForWinSock2  
{
public:
	/**
	 * RSocket:		�R���X�g���N�^
	 *				�Ƃ肠�������������܂��B
	 */
	RSocketForWinSock2();
	/**
	 * RSocket:		�R���X�g���N�^
	 *				�w�肳�ꂽ�\�P�b�g�ŏ��������܂��B
	 *
	 * @param inNewSocket	���ɂȂ����Ă���\�P�b�g.
	 * @return  
	 */
	RSocketForWinSock2(SOCKET inNewSocket);
	/**
	 * ~RSocket:	�f�X�g���N�^
	 *				
	 */
	virtual ~RSocketForWinSock2();

	/**
	 * Open:			�\�P�b�g���쐬����.
	 *
	 * @param type		�\�P�b�g�̎��
	 *						SOCK_STREAM : TCP�Ŏg�p����
	 *						SOCK_DGRAM : UDP�Ŏg�p����
	 * @param protocol	�g�p����v���g�R��
	 * @throws			RIOCannotOpenException		�J���Ȃ������Ƃ�
	 */
	void Open( int type, int protocol ) throw(RIOCannotOpenException);
	/**
	 * Connect:				�ڑ�
	 *
	 * @param inHost		�z�X�g��
	 * @param inPort		�|�[�g
	 * @throws				RIOException			�ڑ��ł��Ȃ������Ƃ�
	 */
	void Connect(string inHost , int inPort)	throw(RIOException);
	/**
	 * Connect:			�ڑ�
	 *
	 * @param inSai		�ڑ����
	 * @throws			RIOException			�ڑ��ł��Ȃ������Ƃ�
	 */
	void Connect(const SOCKADDR_IN * inSai) throw(RIOException);
	/**
	 * Bind:			bind(�ȈՔ�)
	 *
	 * @param inPort	bind����|�[�g
	 * @throws			RIOException		bind���s
	 */
	void Bind(int inPort) throw(RIOException);
	/**
	 * Bind:			bind
	 *
	 * @param inSai		bind���
	 * @throws			RIOException		bind���s
	 */
	void Bind(const SOCKADDR_IN * inSai) throw(RIOException);
	/**
	 * Listen:					listen
	 *
	 * @param inBacklog			�ڑ��v�����O��ۑ�����i�L���[�C���O����j�ő吔
	 * @throws	RIOException	listen���s	
	 */
	void Listen(int inBacklog = SOMAXCONN) throw(RIOException);
	/**	
	 * Accept:					accept
	 *
	 * @throws	RIOException	accept���s
	 */
	void Accept(SOCKET inAcceptSocket ,
						PVOID lpOutputBuffer, DWORD * outReadByte,OVERLAPPED * outOverlapped) throw(RIOException);
	/**
	 * ShutDown:	�ؒf.
	 *
	 */
	void ShutDown();
	/**
	 * Close:	�\�P�b�g�N���[�Y.
	 *
	 * @throws	RIOCannotCloseException		�ؒf�Ɏ��s
	 */
	void Close() throw(RIOCannotCloseException);
	/**
	 * Send:				���M
	 *
	 * @param ioParam		�Ȃ�
	 * @return int 
	 * @throw	RIOCannotWriteException
	 */
	int RSocketForWinSock2::Send( AsyncParam * ioParam ) throw(RIOCannotWriteException);
	/**
	 * Reserve:				��M.
	 *
	 * @param ioParam		�Ȃ�
	 * @return int 
	 * @throw	RIOCannotReadException
	 */
	int RSocketForWinSock2::Reserve( AsyncParam * ioParam ) throw(RIOCannotReadException);
	/**
	 * WaitForSignal:		��M�A���M����܂ő҂�.
	 *
	 * @param ioParam		�Ȃ�
	 * @param inTimeOut		�^�C���A�E�g.
	 * @return void 
	 * @throw	RIOException
	 */
	void RSocketForWinSock2::WaitForSignal(AsyncParam * ioParam,int inTimeOut ) throw(RIOException);
	/**
	 * WSAGetOverlappedResult:		��M�A���M�������ʂ��󂯎��.
	 *
	 * @param ioParam				�Ȃ�
	 * @return void 
	 * @throw	RIOException
	 */
	void RSocketForWinSock2::WSAGetOverlappedResult(AsyncParam * ioParam) throw(RIOException);

	bool RSocketForWinSock2::TransmitFile( HANDLE	inFileHandle , int inNumberOfBytesToWrite , int inNumberOfBytesPerSend,
									TRANSMIT_FILE_BUFFERS * inTransmitFileBufers , AsyncParam * ioParam )
									throw (RIOCannotReadException);
	/**
	 * getSocket:	�\�P�b�g�n���h���̎擾
	 *				�J�v�Z�������󂷂̂Ŏg��Ȃ��łق���.
	 *
	 * @return SOCKET	�\�P�b�g�n���h��
	 */
	SOCKET	getSocket()
	{
		return Socket;
	}

private:
	/**
	 * HoldTillReadyForWrite:		�������݂��ł���悤�ɂȂ�܂őҋ@.
	 *
	 * @param	inTimeOutS			�^�C���A�E�g����b��
	 * @throws	RIOException	�^�C���A�E�g.
	 */
	void HoldTillReadyForWrite(long inTimeOutS) throw(RIOException);

	/**
	 * IntTime:	�^�C���A�E�g�Ɏg�����Ԃ̒�.
	 *
	 * @return DWORD	UNIX����
	 */
	DWORD IntTime();

protected:
	/**
	 * ErrorToMesage:	�G���[���b�Z�[�W���擾����.
	 *
	 * @return string	�G���[���b�Z�[�W
	 */
	string ErrorToMesage();


private:
	//�\�P�b�g�n���h��.
	SOCKET	Socket;
	//�ڑ����Ă���ꍇ true.
	bool	IsConnect;

	enum { receives = 0, sends = 1, both = 2 };

};

#endif // !defined(AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_)
