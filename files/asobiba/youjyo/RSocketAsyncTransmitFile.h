// RSocketAsyncTransmitFile.h: RSocketAsyncTransmitFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_)
#define AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSocketForWinSock2.h"
#include "RNotFoundException.h"

//�񓯊��t�@�C���]��.
class RSocketAsyncTransmitFile  
{
public:
	RSocketAsyncTransmitFile();
	virtual ~RSocketAsyncTransmitFile();

	void Mount(RSocketForWinSock2*		inSocket)
	{
		this->Socket = inSocket;
	}
//�t�@�C���]��.
void RSocketAsyncTransmitFile::Transmit(const string inFilename  , int inStart,int inEnd , TRANSMIT_FILE_BUFFERS* inAppendBuffer);
//�t�@�C���𔺂�Ȃ��]��.
void RSocketAsyncTransmitFile::TransmitWithout(TRANSMIT_FILE_BUFFERS* inAppendBuffer);

	int RSocketAsyncTransmitFile::WaitForComplate() throw(RIOException);
private:
	HANDLE					FileHandle;
	RSocketForWinSock2*		Socket;
	AsyncParam				TransmitUse;
	bool					IsPending;
};

#endif // !defined(AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_)
