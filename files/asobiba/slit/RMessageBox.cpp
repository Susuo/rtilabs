// RMessageBox.cpp: RMessageBox �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RMessageBox.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMessageBox::RMessageBox()
{

}

RMessageBox::~RMessageBox()
{

}

void RMessageBox::Message(const string &inMsg , const string &inTitle )
{
#ifndef _WIN32_WCE
	MessageBox(NULL , inMsg.c_str() , inTitle.c_str() , MB_OK | MB_ICONHAND);
#else
	MY_USES_CONVERSION;
	MessageBox(NULL , _A2W(inMsg.c_str()) , _A2W(inTitle.c_str()) , MB_OK | MB_ICONHAND);
#endif

}

