// YWriteHTML.cpp: YWriteHTML �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YWriteHTML.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YWriteHTML::YWriteHTML()
{

}

YWriteHTML::~YWriteHTML()
{

}

void YWriteHTML::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Error.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\error.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)error.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	try
	{
		KakikoSimasuta.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\kakikomi.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)kakikomi.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	try
	{
		KakikoKakunin.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\kakunin.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)kakunin.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	this->BoardName = inBoardName;
	this->SettingTXT = inSettingTXT;
}



//�G���[�쐬.
const string YWriteHTML::makeError(const string & inErrorMessage , const string & inIP , 
								   const string & inFROM , const string & inMail , const string & inMESSAGE) const
{
	RStringMap smap;
	smap["YErrorString"] = inErrorMessage;
	smap["YIPAddress"] = inIP;
	smap["YKakikoName"] = inFROM;
	smap["YKakikoMail"] = inMail;
	smap["YkakikoText"] = inMESSAGE;
	smap["YItaName"] = BoardName;

	return Error.Replace(&smap);
}

//�������݂܂����쐬.
const string YWriteHTML::makeKakikoSimasuta() const
{
	RStringMap smap;
	smap["YItaName"] = BoardName;

	return KakikoSimasuta.Replace(&smap);
}

//�������݊m�F�쐬.
const string YWriteHTML::makeKakikoKakunin(const string & inIP , const string & inFROM , const string & inMail , const string & inMESSAGE) const
{
	RStringMap smap;
	smap["YIPAddress"] = inIP;
	smap["YKakikoName"] = inFROM;
	smap["YKakikoMail"] = inMail;
	smap["YkakikoText"] = inMESSAGE;
	smap["YItaName"] = BoardName;

	return KakikoKakunin.Replace(&smap);
}

