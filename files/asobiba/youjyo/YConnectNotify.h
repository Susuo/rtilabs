// YConnectNotify.h: YConnectNotify �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_)
#define AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotify.h"
#include "RSocket.h"
#include "RAutoOverLapped.h"

struct YConnectPack 
{
	SOCKET				AcceptSocket;
	RAutoOverLapped		OverLapped;
	in_addr				RemoteIP;
};

class YConnectNotify  
{
public:
	YConnectNotify()
	{
	}
	virtual ~YConnectNotify()
	{
	}

	//�Ȃ��������Ƃ�ʒm����
	void PostAcceptNotify(YConnectPack * inConnectPack)
	{
		Notify.Post(inConnectPack);
	}

	//���̐ڑ���j������
	void PostShutdownNotify()
	{
		Notify.Shutdown();
	}

	//�Ȃ��������Ƃ��ʒm�����܂ő҂�
	bool WaitAcceptNotify(YConnectPack ** outConnectPack)
	{
		return Notify.Wait((void**)outConnectPack);
	}

	static YConnectNotify* getInstance();

private:
	RNotify		Notify;
};

#endif // !defined(AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_)
