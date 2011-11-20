// RDiskUtil.cpp: RDiskUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDiskUtil.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RDiskUtil::RDiskUtil()
{

}

RDiskUtil::~RDiskUtil()
{

}

//�t�@�C�������݂��邩?
bool RDiskUtil::Exist(const string & inFileName)
{
	return (0xFFFFFFFF != GetFileAttributes(inFileName.c_str() ));
}


//�J�����g�f�B���N�g���̎擾.
string RDiskUtil::getCurrentDirectory() 
{
#ifndef _WIN32_WCE
	char buffer[MAX_PATH];
	if ( ::GetCurrentDirectory(MAX_PATH, buffer) == 0) return "";
	return string(buffer) + "\\";
#else
	//WINCE �ɃJ�����g�f�B���N�g���Ƃ����T�O���Ȃ����߁A
	//�v���Z�X�̃p�X���p�[�X����.
	MY_USES_CONVERSION;

	unsigned short buffer[MAX_PATH];
	if ( ::GetModuleFileName(NULL , buffer , MAX_PATH) == 0) return "";
	string dir = _W2A(buffer);
	int pos = dir.rfind('\\');
	if (pos == -1) return dir;

	return dir.substr(0, pos )+ "\\";
#endif
}

