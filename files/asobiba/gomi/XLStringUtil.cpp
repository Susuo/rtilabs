// XLStringUtil.cpp: XLStringUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLStringUtil.h"

USE_TEST(XLStringUtil)


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLStringUtil::XLStringUtil()
{

}

XLStringUtil::~XLStringUtil()
{

}

#define TRIM_SPACE(x)	((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')

string XLStringUtil::UrlDecode(const string & inUrl)
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


//���E�̋󔒂̏���
string XLStringUtil::chop(const string & inString)
{
	const char * p = inString.c_str();
	int size = inString.size();
	int l;
	for(l = 0 ; l < size ; l++)
	{
		if ( !TRIM_SPACE(p[l]) )
		{
			break;
		}
	}
	if (l == size)
	{
		return "";	//���ׂċ�
	}

	int r;
	for(r = size-1 ; r >= 0 ; r--)
	{
		if ( !TRIM_SPACE(p[r]) )
		{
			break;
		}
	}
	ASSERT (l  <= r);

	return inString.substr(l , r - l + 1);
}

//�u��
string XLStringUtil::Replace(const string &inTarget ,const string &inOld ,const string &inNew)
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

//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
string XLStringUtil::htmlspecialchars(const string &inStr)
{
	return Replace(Replace(Replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;");
}


//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
string XLStringUtil::nl2br(const string &inStr)
{
	return Replace(inStr , "\r\n" , "<br>");
}

//inTarget �� inStart �` inEnd �܂ł��擾
string XLStringUtil::Cut(const string &inTarget , const string & inStart , const string &inEnd , string * outNext )
{
	const char * p = NULL;
	if ( !inStart.empty() )
	{
		p = strstr(inTarget.c_str() , inStart.c_str() );
		if (p == NULL ) return "";
		p += inStart.size();
	}
	else
	{
		p = inTarget.c_str();
	}

	const char * p2 = NULL;
	if ( ! inEnd.empty() )
	{
		p2 = strstr(p , inEnd.c_str() );
		if (p2 == NULL ) return "";
	}
	else
	{
		p2 = p + strlen(p);
	}

	string ret = string( p , 0 , p2 - p );
	if (outNext)
	{
		*outNext = string(p2 + inEnd.size());
	}

	return ret;
}


void XLStringUtil::test()
{
	{
		string r;
		ASSERT( (r = chop("abc") ) == "abc");
		ASSERT( (r = chop(" abc") ) == "abc");
		ASSERT( (r = chop(" abc  ") ) == "abc");
		ASSERT( (r = chop(" abc \r\n") ) == "abc");
		ASSERT( (r = chop("			abc \r\n") ) == "abc");
	}
}
