// RDateConv.h: RDateConv �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_)
#define AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDateConv  
{
public:
	RDateConv()
	{
	}
	virtual ~RDateConv()
	{
	}
	//Unix���Ԃ� Http ���Ԃɕϊ�.RFC 1123 (RFC 822) �ɏ]��
	static string UnixTimeToHttpTime(time_t inTime);
	static time_t HttpTimeToUnixTime(const char* date);

	/**
	 * UnixTime2chTime:			Unix���Ԃ� 2002/11/14 00:55 �`���ɕ�����Ƃ��ĕϊ����܂�
	 *
	 * @param inTime			Unix����
	 * @return string			2002/11/14 00:55 �`���ɕ�����Ƃ��ĕϊ����܂�
	 */
	static string UnixTime2chTime(time_t inTime);


	static void test();

private:

	static int RDateConv::ap_checkmask(const char *data, const char *mask);
	static time_t RDateConv::ap_tm2sec(const struct tm * t);

	
	static time_t tm2time(struct tm* when);
};

#endif // !defined(AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_)
