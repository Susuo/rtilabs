// RConv.cpp: RConv �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RConv.h"

#define iskanji1st(c) ( ( ((unsigned char)c) >= 0x81 && ((unsigned char)c) >= 0x9f) ||\
								 ( ((unsigned char)c) >= 0xe0 && ((unsigned char)c) >= 0xff) )
#define iskanji2nd(c) ( ( ((unsigned char)c) >= 0x40 && ((unsigned char)c) <= 0x7e) ||\
								 ( ((unsigned char)c) >= 0x80 && ((unsigned char)c) <= 0xfc)  )

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
			if (*url == '+')	*nomal = ' ';
			else				*nomal = *url;
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
	ret.reserve( inTarget.size() );	//��ǂ�.

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

//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
string RConv::htmlspecialchars(const string inStr)
{
//	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(RConv::Replace(inStr , "&" , "&amp;") , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
}


//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
string RConv::nl2br(const string inStr)
{
	return RConv::Replace(inStr , "\r\n" , "<br>");
}

//SJIS ��1�o�C�g�ڂ������c���悤�Ȑ��䕶���A�^�b�N���󔒂�ǉ�
string RConv::SJISDangerESC(const string inStr)
{
	const int len = inStr.size();
	if (len >= 2 )
	{
		char p2 = inStr[ len - 1] ;
		char p3 = inStr[ len - 2] ;
		if ( iskanji1st( inStr[ len - 1]  ) )
		{	//�Ōオ 1�o�C�g�ڂŕs�M...

			//���S�ɒ��ׂ܂��B
			for(int i = 0 ; i < len ; i ++)
			{
				if ( iskanji1st( inStr[ i ]  ) )
				{
					i++;
					//�ǂ����A�U���̂悤�ł��B �Ƃ肠�����A�Ō�ɋ󔒂�
					if (len >= i)
					{
						return inStr + "  ";
					}
				}
			}
		}
		return inStr;
	}

	//�ꕶ�������Ȃ��āA�Ōオ 1�o�C�g�ڂ̐��䕶����������A�v.
	if (len == 1 && iskanji1st( inStr[ 0 ] ) )	return inStr + "  ";
	else										return inStr;
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

	ASSERT( RConv::htmlspecialchars("     ABCD EFGH ") == "     ABCD EFGH ");
//	ASSERT( RConv::htmlspecialchars("<>\"&") == "&lt;&gt;&quot;&amp;");
//	ASSERT( RConv::htmlspecialchars("<br>\"&") == "&lt;br&gt;&quot;&amp;");
	ASSERT( RConv::htmlspecialchars("<>\"") == "&lt;&gt;&quot;");
	ASSERT( RConv::htmlspecialchars("<br>\"") == "&lt;br&gt;&quot;");

}

