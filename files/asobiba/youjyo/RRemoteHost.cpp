// RRemoteHost.cpp: RRemoteHost �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RRemoteHost.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RRemoteHost::RRemoteHost()
{

}

RRemoteHost::~RRemoteHost()
{

}

void RRemoteHost::setIP(in_addr inAddr)
{
	RemoteIP =
		num2str((unsigned int)inAddr.S_un.S_un_b.s_b1) + "." +
		num2str((unsigned int)inAddr.S_un.S_un_b.s_b2) + "." +
		num2str((unsigned int)inAddr.S_un.S_un_b.s_b3) + "." +
		num2str((unsigned int)inAddr.S_un.S_un_b.s_b4) ;
	RawIP = inAddr;
}

