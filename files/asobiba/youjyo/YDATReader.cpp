// YDATReader.cpp: YDATReader �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "YDATReader.h"
#include "RConv.h"

//URL�Ɏg���镶��.
#define isurl(x) (isalnum(x) || x=='.' || x==':' ||  x=='@' || x == '/' || x == '-' || x == '_' || x == '#')

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YDATReader::YDATReader()
{

}

YDATReader::~YDATReader()
{

}

const char* YDATReader::makeHTML(RStringMap* outSmap ,int inNo ,const char* inData )
{
	const char * name  = inData;
	const char * mail  = next(name);
	const char * date  = next(mail);
	const char * text  = next(date);
	const char * title = next(text);

	if (name==NULL || mail==NULL || date==NULL || text==NULL || title==NULL)
	{//�����Ă܂��B
		(*outSmap)["YKakikoNo"] = num2str(inNo) ;
		(*outSmap)["YKakikoName"] = "��YoujyoMaster" ;
		(*outSmap)["YKakikoTime"] = " ID: YoujyoMaster" ;
		(*outSmap)["YkakikoText"] = "�������Ă܂��B�B�B" ;

		return NULL;
	}
	else
	{//�ʏ�
		if (date - mail- 2 == 0)
		{//age
			(*outSmap)["YKakikoNo"] = num2str(inNo) ;
			(*outSmap)["YKakikoName"] = string(name , mail - name - 2);
			(*outSmap)["YKakikoTime"] = string(date , text - date - 2) ;
			(*outSmap)["YkakikoText"] = makeLink(string(text , title - text - 2)) ;
		}
		else
		{//sage
			(*outSmap)["YKakikoNo"] = num2str(inNo) ;
			(*outSmap)["YKakikoName"] = "<A href=\"mailto:" + string(mail , date - mail - 2) + "\">" + string(name , mail - name - 2) + "</A>";
			(*outSmap)["YKakikoTime"] = string(date , text - date - 2) ;
			(*outSmap)["YkakikoText"] = makeLink(string(text , title - text - 2)) ;
		}
	}
	return nextnr(title);
}



/**
 * MaskKakikomi:			�������݂̃}�X�N
 *
 * @param inData			�ꏑ�����݂� dat
 * @param inMaskSize		�}�X�N�̐�
 * @param inMask			�}�X�N�f�[�^
 * @return string			�}�X�N�������ꂽ dat �ɏ����߂���f�[�^
 */
string YDATReader::MaskKakikomi(const char* inData , int inMaskSize   ,const string *inMask)
{
	const char * name  = inData;
	const char * mail  = YDATReader::next(name);
	const char * date  = YDATReader::next(mail);
	const char * text  = YDATReader::next(date);
	const char * title = YDATReader::next(text);

	if (name==NULL || mail==NULL || date==NULL || text==NULL || title==NULL)
	{//�����Ă܂��B
		return "<><><><>\n";
	}
	else
	{//�ʏ�
		return 
			YDATReader::MaskOneRecord( string(name , mail - name - 2) , inMaskSize , inMask) + "<>" +
			YDATReader::MaskOneRecord( string(mail , date - mail - 2) , inMaskSize , inMask) + "<>" +
			string(date , text - date - 2) + "<>" +
			YDATReader::MaskOneRecord( string(text , title - text - 2) , inMaskSize , inMask) + "<>\n";
	}
}

/**
 * MaskOneRecord:			�ꍀ�ڂ��Ƃ̃}�X�N
 *
 * @param inData			�ꏑ�����ݍ���
 * @param inMaskSize		�}�X�N�̐�
 * @param inMask			�}�X�N�f�[�^
 * @return string			�}�X�N�������ꂽ�f�[�^
 */
string YDATReader::MaskOneRecord(const string& inData , int inMaskSize   ,const string *inMask)
{
	string rea = inData ;
	for(int i = 0; i < inMaskSize ; i++)
	{
		rea = RConv::Replace( rea , inMask[i] , "!(�폜�˗��ɂ��폜)!" );
	}
	return rea;
}


//�U�u�W�F�N�Ƃ̃��[�h
string YDATReader::Subject(const char* inDat,int len)
{
	const char * title = next(next(next(next(inDat))));
	if (title == NULL) return "";

	//�I�[�T�[�`.
	const char* end = inDat + len;
	const char* str = nextnr(title , end);

	return  string(title , str - title );
}



//���� <> ��
const char* YDATReader::next(const char* str)
{
	if (str == NULL) return NULL;

	const char*  p = strstr(str , "<>");
	if ( p == NULL) return NULL;

	return p + 2;
}
//���� \n ��
const char* YDATReader::nextnr(const char* str)
{
	if (str == NULL) return NULL;

	const char*  p = strchr(str , '\n');
	if ( p == NULL) return NULL;

	return p + 1;
}
//���� \n ��
const char* YDATReader::nextnr(const char* str,const char* end)
{
	if (str == NULL) return NULL;

	for ( ; *str != '\n' && str < end ; str++)	;

	return str;
}

void YDATReader::test()
{
	{
		string http;
		ASSERT( (http = makeLink("unkonageman"))  == "unkonageman");
		ASSERT( (http = makeLink("http://www.yahoo.co.jp/"))  == "<a href=\"http://www.yahoo.co.jp/\">http://www.yahoo.co.jp/</a>");
		ASSERT( (http = makeLink("�g���http://www.yahoo.co.jp/�g���"))  == "�g���<a href=\"http://www.yahoo.co.jp/\">http://www.yahoo.co.jp/</a>�g���");
		ASSERT( (http = makeLink("http://www.catnet.ne.jp/kouno/c_faq/c1.html#32"))  == "<a href=\"http://www.catnet.ne.jp/kouno/c_faq/c1.html#32\">http://www.catnet.ne.jp/kouno/c_faq/c1.html#32</a>");
		
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "<><>2003/1/3 14:19<>&lt;<br>&gt;<br>&lt;&gt;<br>&lt;&gt;<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "");
		ASSERT((ret = smap["YKakikoTime"]) == "2003/1/3 14:19");
		ASSERT((ret = smap["YkakikoText"]) == "&lt;<br>&gt;<br>&lt;&gt;<br>&lt;&gt;");
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "nazo<><>2002/12/1 1:39<>www.nazo.to<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "nazo");
		ASSERT((ret = smap["YKakikoTime"]) == "2002/12/1 1:39");
		ASSERT((ret = smap["YkakikoText"]) == "www.nazo.to");
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "nazo<>sage<>2002/12/1 1:39<>www.nazo.to<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "<A href=\"mailto:sage\">nazo</A>");
		ASSERT((ret = smap["YKakikoTime"]) == "2002/12/1 1:39");
		ASSERT((ret = smap["YkakikoText"]) == "www.nazo.to");
	}
	//�������݂̃}�X�N
	{
		string ret;
		string mask[3];
		mask[0] = "www";
		mask[1] = "nazo";
		mask[2] = "to";
		ret = MaskKakikomi("nazo<>sage<>2002/12/1 1:39<>www.nazo.to<>\n" , 3 , mask);
		ASSERT( ret == "!(�폜�˗��ɂ��폜)!<>sage<>2002/12/1 1:39<>!(�폜�˗��ɂ��폜)!.!(�폜�˗��ɂ��폜)!.!(�폜�˗��ɂ��폜)!<>\n" );
	}
}


// html�炵�����̂��������ꍇ�����N�Ƃ��܂�
string YDATReader::makeLink(const string &inTarget)
{
	string ret;
	ret.reserve( inTarget.size() );	//��ǂ�.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;

	//http://��T���܂��B
	while( (pos = inTarget.find( "http://" , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos,pos - oldpos);

		oldpos = pos;
		pos += sizeof("http://");
		for( ; isurl(inTarget[pos]) ; ++pos)	;

		string http = inTarget.substr(oldpos,pos-oldpos);
		ret += "<a href=\"";
		ret += http;
		ret += "\">";
		ret += http;
		ret += "</a>";
		oldpos = pos;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;
}


