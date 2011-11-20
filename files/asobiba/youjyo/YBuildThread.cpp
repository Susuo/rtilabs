// YBuildThread.cpp: YBuildThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YBuildThread.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YBuildThread::YBuildThread()
{

}

YBuildThread::~YBuildThread()
{

}


void YBuildThread::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Template.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/buildthread.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)buildthread.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	this->BoardName = inBoardName;
	this->SettingTXT = inSettingTXT;
}

const string YBuildThread::make(POST& ioPOST, YLocalRule* inLocalRule) const
{
	RAutoReadSpinLock al(&Lock);

	RStringMap smap;
	smap["YItaLocalRule"] = "";
	smap["YItaName"] = BoardName;
	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YRequestThread"] = ioPOST["key"];
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	return Template.Replace(&smap);
}

