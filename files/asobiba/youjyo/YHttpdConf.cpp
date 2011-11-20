// YHttpdConf.cpp: YHttpdConf �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YHttpdConf.h"
#include "RConfigFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YHttpdConf::YHttpdConf()
{

}

YHttpdConf::~YHttpdConf()
{

}

//�ݒ�t�@�C��.
void YHttpdConf::Load(const string inFilename)
{
	RConfigFile	Setting;

	//�ݒ�����[�h����̂ł���[.
	Setting.Load(inFilename);

	/*ChangeLog.txt:2003/01/24	httpd.conf �� ListenPort ���w�肳�ꂽ�Ƃ��Ă��������Ă����̂ŏC��.
	*/
	//listen ����|�[�g
	this->ListenPort	= Setting.LoadOrDefault("ListenPort" , 80 );
	//�T�[�o�̖��O
	this->ServerName	= Setting.LoadOrDefault("ServerName" , string("127.0.0.1") );

	//�h�L�������g���[�g
	char currentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH , currentDirectory);
	this->DocumentRoot	= Setting.LoadOrDefault("DocumentRoot" , string(currentDirectory)  + "/htdocs" );

	this->Version = "�傣�ǁJ��1.0��3+ (02/02/22)";
}

/**
 * getInstance:		Singleton�C���^�[�t�F�[�X�̍쐬.
 *
 * @return YHttpdConf* 
 */
YHttpdConf* YHttpdConf::getInstance()
{
	static YHttpdConf p;
	return &p;
}

