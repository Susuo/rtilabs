// YTransmitFile.h: YTransmitFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTRANSMITFILE_H__6C555459_7859_4CFD_98F7_30F60DDD11F2__INCLUDED_)
#define AFX_YTRANSMITFILE_H__6C555459_7859_4CFD_98F7_30F60DDD11F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHTTPHeader.h"
#include "RSocketForWinSock2.h"

class YTransmitFile  
{
public:
	YTransmitFile();
	virtual ~YTransmitFile();

	void TransmitFile(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,RSocketForWinSock2* ioSocket);
private:
	//�t�@�C�������g���q�𕪗�
	string YTransmitFile::Filename2Ext(const string inFilename) const;
};

#endif // !defined(AFX_YTRANSMITFILE_H__6C555459_7859_4CFD_98F7_30F60DDD11F2__INCLUDED_)
