// RSocket.h: RSocket �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_)
#define AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"


/**
 * class RSocket:	�\�P�b�g
 *
 */
class RSocket  
{
public:
	/**
	 * RSocket:		�R���X�g���N�^
	 *				�Ƃ肠�������������܂��B
	 */
	RSocket();
	/**
	 * RSocket:		�R���X�g���N�^
	 *				�w�肳�ꂽ�\�P�b�g�ŏ��������܂��B
	 *
	 * @param inNewSocket	���ɂȂ����Ă���\�P�b�g.
	 * @return  
	 */
	RSocket(SOCKET inNewSocket);
	/**
	 * ~RSocket:	�f�X�g���N�^
	 *				�Ђ炫���ς̏ꍇ�͂����Ăɕ��܂��B
	 */
	virtual ~RSocket();

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
	 * @return RSocket*			�ڑ����ꂽ�N���C�A���g
	 * @throws	RIOException	accept���s
	 */
	RSocket* Accept() throw(RIOException);
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
	* Send:				���M.
	*
	* @param inBuffer		���M����o�b�t�@
	* @param inBufferLen	���M����o�b�t�@�̃T�C�Y
	* @param inFlags		�t���O
	* @param inTimeoutS	�^�C���A�E�g�b�� * 5
	* @return int			���M�����T�C�Y.
	* @throws	RIOException	�^�C���A�E�g.
	*/
	int Send( const char* inBuffer ,int inBufferLen , int inFlags = 0,int inTimeoutS = 1) throw(RIOException,RIOCannotWriteException);
	/**
	 * Reserve:				��M.
	 *
	 * @param outBuffer		��M����o�b�t�@
	 * @param inBufferLen	��M����o�b�t�@�̃T�C�Y
	 * @param inFlags		�t���O
	 * @param inTimeoutS	�^�C���A�E�g�b�� * 5
	 * @return int			��M�����T�C�Y.
	 * @throws	RIOException	�^�C���A�E�g.
	 */
	int Reserve( char* outBuffer ,int inBufferLen , int inFlags = 0,int inTimeoutS = 1) throw(RIOException,RIOCannotReadException);
private:
	/**
	 * HoldTillReadyForRead:	�ǂݍ��݂��ł���悤�ɂȂ�܂őҋ@.
	 *
	 * @param	inTimeOutS		�^�C���A�E�g����b�� * 5
	 * @throws	RIOException	�^�C���A�E�g.
	 */
	void HoldTillReadyForRead(long inTimeOutS) throw(RIOException);
	/**
	 * HoldTillReadyForWrite:		�������݂��ł���悤�ɂȂ�܂őҋ@.
	 *
	 * @param	inTimeOutS			�^�C���A�E�g����b�� * 5
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
	//�\�P�b�g�n���h��.
	SOCKET	Socket;
	//�ڑ����Ă���ꍇ true.
	bool	IsConnect;

	enum { receives = 0, sends = 1, both = 2 };
};

#endif // !defined(AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_)
