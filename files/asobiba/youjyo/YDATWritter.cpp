// YDATWritter.cpp: YDATWritter �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "YDATWritter.h"
#include "RDateConv.h"


//>> �̃��X�|���X�Ŏg���镶��
#define isrespons(x) (isdigit(x) || x=='-')

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YDATWritter::YDATWritter()
{

}

YDATWritter::~YDATWritter()
{

}

string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText,const string & inAppend)
{
	//ŰŰ��ư��԰��<>sage<>2002/11/14 00:55<>���������������������́L�[`)������������������!<>
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + " " + inAppend + "<>" + inText + "<>\n";
}
string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText , const string & inSubject,const string & inAppend)
{
	//ŰŰ��ư��԰��<>sage<>2002/11/14 00:55<>���������������������́L�[`)������������������!<>
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + " " + inAppend + "<>" + inText + "<>" + inSubject + "\n";
}

string YDATWritter::makeAborn(const string & inAbornName)
{
	return inAbornName + "<>" + inAbornName + "<>" + inAbornName + "<>" + inAbornName + "<>\n";
}

//�V�X�e�������ŏ�������
string YDATWritter::makeSystem(const string & inName ,const string & inMail ,const string & inText)
{
	//�V�X�e�������ŏ������ޏꍇ.
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + "<>" + inText + "<>\n";
}

// >> �ւ̃��X�������N�ɂ���.
const string YDATWritter::AppendTagHtml(const string & inBaee , const string & inTarget)
{
	if (inBaee.empty() ) return inTarget;

	string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;

	//>> ��T���܂��B
	while( (pos = inTarget.find( "&gt;&gt;" , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos,pos - oldpos);

		pos += sizeof("&gt;&gt;") - 1;
		oldpos = pos;
		for( ; isrespons(inTarget[pos]) ; ++pos)	;

		string res = inTarget.substr(oldpos,pos-oldpos );
		if ( res.empty() )
		{	//>> �����Ȃ������ꍇ.
			ret += "&gt;&gt;";
		}
		else
		{
			ret += "<a href=\"";
			ret += inBaee + res;
			ret += "\" target=\"_blank\">&gt;&gt;";
			ret += res;
			ret += "</a>";
		}
		oldpos = pos;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;
}


void YDATWritter::test()
{
	string r;
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","&gt;&gt;10-100") ) == "<a href=\"a/10-100\" target=\"_blank\">&gt;&gt;10-100</a>");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","&gt;&gt;") ) == "&gt;&gt;");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","�g��Ƃ�&gt;&gt;10-100�ނ�") ) == "�g��Ƃ�<a href=\"a/10-100\" target=\"_blank\">&gt;&gt;10-100</a>�ނ�");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","�g��Ƃ�&gt;&gt;10-��100�ނ�") ) == "�g��Ƃ�<a href=\"a/10-\" target=\"_blank\">&gt;&gt;10-</a>��100�ނ�");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","�g��Ƃ�&gt;&gt;10->>100�ނ�") ) == "�g��Ƃ�<a href=\"a/10-\" target=\"_blank\">&gt;&gt;10-</a>>>100�ނ�");
}
