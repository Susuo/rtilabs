// MVConfig.cpp: MVConfig �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVConfig.h"
#include "RConfigFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MVConfig::MVConfig()
{
	this->CurrentDirectory = CommonCurrentDirectory();
}

MVConfig::~MVConfig()
{

}


MVConfig* MVConfig::getInstance()
{
	static MVConfig conf;
	return &conf;
}

//�J�����g�f�B���N�g���̎擾.
string MVConfig::CommonCurrentDirectory() const
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

void MVConfig::Load()
{
	RConfigFile confFile;
	confFile.Load(CurrentDirectory + "slit.conf");

	//�Ō�ɊJ�����t�@�C����
	this->LastFilename = confFile.LoadOrDefault("LastFilename" , string("") );
	//�Ō�ɊJ�����y�[�W
	this->LastPage = confFile.LoadOrDefault("LastPage" , 0 );

	//����O�̏�Ԃ̋L�^
	this->LastMemory	= confFile.LoadOrDefault("LastMemory" ,  true );
	//�W���X�g�t�B�b�g
	this->JustFit	= confFile.LoadOrDefault("JustFit" ,  false );
	//�X�N���[���J�E���g
	this->ScrollCount	= confFile.LoadOrDefault("ScrollCount" ,  2 );
	//�X�N���[���^�C�v
	this->ScrollType	= (SCROLL_TYPE) confFile.LoadOrDefault("ScrollType" ,  0 );
	
}

void MVConfig::SavePage(const string & inFilename , int inPage)
{
	if (this->LastMemory)
	{
		this->LastFilename		=	inFilename;
		this->LastPage			=	inPage;
	}
	else
	{
		this->LastFilename		=	"";
		this->LastPage			=	0;
	}
}

void MVConfig::setFlg(bool isLastMemory ,bool isJustFit,int inScrollCount , SCROLL_TYPE inScrollType)
{
	this->LastMemory	= isLastMemory;
	this->JustFit		= isJustFit;
	this->ScrollCount	= inScrollCount;
	this->ScrollType	= inScrollType;
}


void MVConfig::Save()
{
	RConfigFile confFile;
	confFile.Load(CurrentDirectory + "slit.conf");

	confFile.Upsert("LastFilename" , this->LastFilename);
	confFile.Upsert("LastPage" , this->LastPage);
	confFile.Upsert("LastMemory" , this->LastMemory);
	confFile.Upsert("JustFit" , this->JustFit);
	confFile.Upsert("ScrollCount" , this->ScrollCount);
	confFile.Upsert("ScrollType" , (int)this->ScrollType);
	


	confFile.Save(CurrentDirectory + "slit.conf");

}
