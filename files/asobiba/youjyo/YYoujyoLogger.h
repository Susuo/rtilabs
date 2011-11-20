// YYoujyoLogger.h: YYoujyoLogger �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_)
#define AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotify.h"
#include "RThread.h"
#include "YConnectNotify.h"


class YLogNotify  
{
public:
	//�Ȃ��������Ƃ�ʒm����
	void Post(string * logString)
	{
		Notify.Post(logString);
	}

	//���̐ڑ���j������
	void Shutdown()
	{
		Notify.Shutdown();
	}

	//�Ȃ��������Ƃ��ʒm�����܂ő҂�
	bool Wait(string ** logString)
	{
		return Notify.Wait((void**)logString);
	}

	static YLogNotify* getInstance();

private:
	RNotify		Notify;
};


//���O���������܂�.
class YYoujyoLogger  : public RThread  
{
public:
	YYoujyoLogger();
	virtual ~YYoujyoLogger();

	/**
	 * Run:	���s.
	 *
	 * @return virtual unsigned int 
	 */
	virtual unsigned int Run();

	/**
	 * Shutdown:	��~���ߑ��M.
	 *
	 */
	void Shutdown();

private:
	FILE	* LogFile;
};

#endif // !defined(AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_)
