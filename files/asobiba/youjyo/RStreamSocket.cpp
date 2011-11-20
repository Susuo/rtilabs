// RStreamSocket.cpp: RStreamSocket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RStreamSocket.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStreamSocket::RStreamSocket(RSocketForWinSock2 * inSocket)
{
	Socket = inSocket;
}

RStreamSocket::~RStreamSocket()
{
}

int RStreamSocket::Write(const char *inBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->Use.Overlapped.hEvent );
	this->Use.Buffer.buf = (char*)inBuffer;
	this->Use.Buffer.len = inBufferSize;
	this->Use.Size = 0;
	this->Use.Flag = 0;

	Socket->Send(&this->Use);
	//�ǁ[��A�ǂ߂�?
	Socket->WaitForSignal( &this->Use,10000);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->Use);
	//���񂾂��ǂ߂����.
	return this->Use.Size;

}
int RStreamSocket::Read(char *outBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->Use.Overlapped.hEvent );
	this->Use.Buffer.buf = (char*)outBuffer;
	this->Use.Buffer.len = inBufferSize;
	this->Use.Size = 0;
	this->Use.Flag = 0;

	Socket->Reserve(&this->Use);
	//�ǁ[��A�ǂ߂�?
	Socket->WaitForSignal( &this->Use,10000);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->Use);
	//���񂾂��ǂ߂����.
	return this->Use.Size;
}


