// YHttpdConf.h: YHttpdConf �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_)
#define AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//�T�[�o�̐ݒ�t�@�C���ł��B
//singleton.
//�����̃X���b�h����ǂݏo���܂����A�@�ǂݍ��݂������Ȃ��̂Ń��b�N�Ȃ��ł�!!
class YHttpdConf  
{
private:	//singleton,
	YHttpdConf();
public:
	virtual ~YHttpdConf();

	void Load(const string inFilename);

	//ListenPort.
	int getListenPort() const
	{
		return this->ListenPort;
	}
	//�T�[�o��.
	const string getServerName() const
	{
		return this->ServerName;
	}
	//�h�L�������g���[�g�̃p�X.
	const string getDocumentRoot() const
	{
		return this->DocumentRoot;
	}

	const string getVersion() const
	{
		return this->Version;
	}


	/**
	 * getInstance:		Singleton�C���^�[�t�F�[�X�̍쐬.
	 *
	 * @return YHttpdConf* 
	 */
	static YHttpdConf* getInstance();
private:
	int		ListenPort;
	string	ServerName;
	string	DocumentRoot;
	string	Version;
};

#endif // !defined(AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_)
