// RTcpSocket.cpp: RTcpSocket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RTcpSocket.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTcpSocket::RTcpSocket()
{

}

RTcpSocket::~RTcpSocket()
{

}

/**
 * Open:			TCP/IP�̃\�P�b�g���쐬����.
 *
 * @throws			RIOCannotOpenException		�J���Ȃ������Ƃ�
 */
void RTcpSocket::Open( ) throw(RIOCannotOpenException)
{
	RSocket::Open(SOCK_STREAM,0);
}
