// RStreamAnSyncSocket.cpp: RStreamAnSyncSocket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RStreamAnSyncSocket.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStreamAsyncSocket::RStreamAsyncSocket(RSocketForWinSock2* inSocket)
{
	Socket = inSocket;
}

RStreamAsyncSocket::~RStreamAsyncSocket()
{
}

void RStreamAsyncSocket::Write(const char *inBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->ReadUse.Overlapped.hEvent );
	this->ReadUse.Buffer.buf = (char*)inBuffer;
	this->ReadUse.Buffer.len = inBufferSize;
	this->ReadUse.Size = 0;
	this->ReadUse.Flag = 0;
	Socket->Reserve(&this->ReadUse);
}

void RStreamAsyncSocket::Read(char *outBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->ReadUse.Overlapped.hEvent );
	this->ReadUse.Buffer.buf = outBuffer;
	this->ReadUse.Buffer.len = inBufferSize;
	this->ReadUse.Size = 0;
	this->ReadUse.Flag = 0;
	Socket->Reserve(&this->ReadUse);
}

int RStreamAsyncSocket::ReadWaitForSignal() throw(RIOException)
{
	//�ǁ[��A�ǂ߂�?
	Socket->WaitForSignal( &this->ReadUse,60);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->ReadUse);
	//���񂾂��ǂ߂����.
	return this->ReadUse.Size;
}

int RStreamAsyncSocket::ReadWaitForComplate() throw(RIOException)
{
	//�ǁ[��A�ǂ߂�?
	Socket->WaitForSignal(&this->ReadUse,WSA_INFINITE);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->ReadUse);
	//���񂾂��ǂ߂����.
	return this->ReadUse.Size;
}

int RStreamAsyncSocket::WriteWaitForSignal() throw(RIOException)
{
	//�ǁ[��A������?
	Socket->WaitForSignal( &this->WriteUse,60);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->WriteUse);
	//���񂾂����������
	return this->WriteUse.Size;
}

int RStreamAsyncSocket::WriteWaitForComplate() throw(RIOException)
{
	//�ǁ[��A������?
	Socket->WaitForSignal( &this->WriteUse,60);	//�^�C���A�E�g�͗�O.
	//�������A���ʃv���[�Y.
	Socket->WSAGetOverlappedResult(&this->WriteUse);
	//���񂾂����������
	return this->WriteUse.Size;
}

