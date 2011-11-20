// RConv.cpp: RConv �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//URL�𕶎���ɕϊ� 
string RConv::UrlEncode(string inUrl)
{
	static const char xc[] = "0123456789ABCDEF";

	const char* url = inUrl.c_str();
	char* orinal = new char[ inUrl.size() + 1];
	char* nomal = orinal;


	for( ; *url ; url++ , nomal++)
	{
		if (*url != '%')
		{
			*nomal = *url;
		}
		else
		{
			//% �����ŏI����Ă��镶����̔r��
			if ( *(url+1) == 0 ){	*nomal = '%';	continue;	}

			//%Z �Ƃ��̂킯�킩�߂̔r��
			char* first = strchr(xc,*(url+1));
			if ( first == NULL ) {	*nomal = '%';	continue;	}

			//%A �����ŏI����Ă��镶����̔r��
			if ( *(url+2) == 0 ){	*nomal = '%';	continue;	}

			//%AZ �Ƃ��̂킯�킩�߂̔r��
			char* second = strchr(xc,*(url+2));
			if ( second == NULL ) {	*nomal = '%';	continue;	}

			*nomal = 16 * (first - xc) + (second - xc);
			url ++;
			url ++;
		}
	}
	*nomal = '\0';
	//������ɓ���āA�o�b�t�@���N���A.
	string nomalString = orinal;
	delete [] orinal;

	return nomalString;
}

//�u��.
string RConv::Replace(const string inTarget ,const string inOld ,const string inNew)
{
	string ret;

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;
	//�u������镶����̒���
	int replaceTargetSize = inOld.size();

	while( (pos = inTarget.find( inOld , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos , pos - oldpos);
		ret += inNew;

		oldpos = pos + replaceTargetSize ;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;

}


//���󔒍폜.
string RConv::ltrim(const string inStr)
{
	int size = inStr.size();
	for(int i = 0 ; i < size ; i++)
	{
		if ( !TRIM_SPACE(inStr[i]) )
		{
			return inStr.substr(i , size - i + 1);
		}
	}
	return inStr;
}


//�E�󔒍폜.
string RConv::rtrim(const string inStr)
{
	int size = inStr.size();
	for(int i = size-1 ; i >= 0 ; i--)
	{
		if ( !TRIM_SPACE(inStr[i]) )
		{
			return inStr.substr(0 , i + 1);
		}
	}
	return inStr;
}

//�����𕶎���ɕϊ�.
string RConv::itoa(int inInt)
{
	char buf[20];
	return ::itoa(inInt , buf , 10 );
}

void RConv::test()
{
	ASSERT( RConv::UrlEncode("%20") == " ");

	ASSERT( RConv::Replace("ABABA" , "A" , "C") == "CBCBC");
	ASSERT( RConv::Replace("���Ђ�" , "��" , "�Ƃ�") == "���Ƃ�Ƃ�");
	ASSERT( RConv::Replace("ABCDEFGH" , "AB" , "ABCDEFGH") == "ABCDEFGHCDEFGH");
	ASSERT( RConv::Replace("ABCDEFGH" , "DEF" , "XYZ") == "ABCXYZGH");
	ASSERT( RConv::Replace("ABCDEFGH" , "DEF" , "XYZ2") == "ABCXYZ2GH");

	ASSERT( RConv::rtrim("ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::rtrim("ABCDEFGH   ") == "ABCDEFGH");
	ASSERT( RConv::rtrim("     ABCDEFGH   ") == "     ABCDEFGH");
	ASSERT( RConv::rtrim("     ABCD EFGH   ") == "     ABCD EFGH");

	ASSERT( RConv::ltrim("ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::ltrim("   ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::ltrim("     ABCDEFGH ") == "ABCDEFGH ");
	ASSERT( RConv::ltrim("     ABCD EFGH ") == "ABCD EFGH ");
}

