// RIPFilter.h: RIPFilter �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_)
#define AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"

class RIPFilterData
{
public:
	RIPFilterData(in_addr inFromIP,in_addr inToIP)
	{
		FromIP.s_addr = htonl(inFromIP.s_addr);	//�r�b�O�G���f�B�A���\�L�ɕϊ�
		ToIP.s_addr = htonl(inToIP.s_addr);		//�r�b�O�G���f�B�A���\�L�ɕϊ�
	}
	RIPFilterData(in_addr inIP,int inMask)
	{
		unsigned long mask = (2 ^ inMask);
		FromIP.s_addr = (htonl(inIP.s_addr) & (~mask));	//�r�b�O�G���f�B�A���\�L�ɕϊ�
		ToIP.s_addr = (htonl(inIP.s_addr) | mask);		//�r�b�O�G���f�B�A���\�L�ɕϊ�
	}
	
	/**
	 * IsBan:		�͂����Ă�����?
	 *
	 * @param inIP	�]�����Ȃ��Ă͂Ȃ�Ȃ�IP
	 * @return bool	�_���ȏꍇ true
	 */
	bool IsBan(in_addr inIP) const
	{
		return (FromIP.s_addr <= inIP.s_addr && inIP.s_addr <= ToIP.s_addr);
	}

	static void RIPFilterData::test();
private:
	in_addr	FromIP;	//�r�b�O�G���f�B�A���\�L
	in_addr	ToIP;	//�r�b�O�G���f�B�A���\�L
};

class RIPFilter  
{
public:
	RIPFilter();
	virtual ~RIPFilter();

	void Clear();
	void Load(const string inFilename);

	inline bool IsBan(in_addr inIP) const
	{
		in_addr p;
		p.s_addr = htonl(inIP.s_addr);	//�r�b�O�G���f�B�A���ɕϊ�.
		for(IPLIST::const_iterator i = IPList.begin() ; i != IPList.end() ; ++i)
		{
			if ( (*i)->IsBan(p) ) return true;	//�A�N�Z�X�֎~
		}
		return false;	//�A�N�Z�X����.
	}

	static void RIPFilter::test();

private:
	void AppendIP(const string) throw(RParseException);

	//�t�B���^�[����IP�ꗗ.
	typedef vector<RIPFilterData*> IPLIST;
	IPLIST	IPList;
};

#endif // !defined(AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_)
