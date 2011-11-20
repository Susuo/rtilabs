// RConv.h: RConv �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_)
#define AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



#define TRIM_SPACE(x)	((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')

class RConv  
{
public:
	RConv()
	{
	}
	virtual ~RConv()
	{
	}

	//URL�𕶎���ɕϊ� 
	static string UrlEncode(string inUrl);
	//�u��.
	static string Replace(const string inTarget ,const string inOld ,const string inNew);
	//���󔒍폜.
	static string ltrim(const string inStr);
	//�E�󔒍폜.
	static string rtrim(const string inStr);
	//�����𕶎���ɕϊ�.
	static string itoa(int inInt);


	static void test();
};

#endif // !defined(AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_)
