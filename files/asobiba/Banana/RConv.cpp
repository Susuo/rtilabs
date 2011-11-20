// RConv.cpp: RConv �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "stdafx.h"
#include "RConv.h"
#include <math.h>

#define iskanji1st(c) ( ( ((unsigned char)c) >= 0x81 && ((unsigned char)c) <= 0x9f) ||\
								 ( ((unsigned char)c) >= 0xe0 && ((unsigned char)c) <= 0xff) )
#define iskanji2nd(c) ( ( ((unsigned char)c) >= 0x40 && ((unsigned char)c) <= 0x7e) ||\
								 ( ((unsigned char)c) >= 0x80 && ((unsigned char)c) <= 0xfc)  )

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//URL�𕶎���ɕϊ� 
string RConv::UrlEncode(const string &inUrl)
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
			const char* first = strchr(xc,*(url+1));
			if ( first == NULL ) {	*nomal = '%';	continue;	}

			//%A �����ŏI����Ă��镶����̔r��
			if ( *(url+2) == 0 ){	*nomal = '%';	continue;	}

			//%AZ �Ƃ��̂킯�킩�߂̔r��
			const char* second = strchr(xc,*(url+2));
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
string RConv::Replace(const string &inTarget ,const string &inOld ,const string &inNew)
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
string RConv::ltrim(const string & inStr)
{
	const char * p = inStr.c_str();
	int size = inStr.size();
	for(int i = 0 ; i < size ; i++)
	{
		if ( !TRIM_SPACE(p[i]) )
		{
			return inStr.substr(i , size - i + 1);
		}
	}
	return inStr;
}


//�E�󔒍폜.
string RConv::rtrim(const string & inStr)
{
	const char * p = inStr.c_str();
	int size = inStr.size();
	for(int i = size-1 ; i >= 0 ; i--)
	{
		if ( !TRIM_SPACE(p[i]) )
		{
			return inStr.substr(0 , i + 1);
		}
	}
	if ( TRIM_SPACE(p[0]) )
	{
		return "";
	}

	return inStr;
}


//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
string RConv::htmlspecialchars(const string &inStr)
{
//	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(RConv::Replace(inStr , "&" , "&amp;") , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
}


//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
string RConv::nl2br(const string &inStr)
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

RStringList RConv::Explode(const string & inStr , char inSep )
{
	RStringList ret;

	const char * str = &inStr[0];
	int size = inStr.size();
	int i , old = 0 ;
	for(i = 0; i < size ; i++)
	{
		if ( str[i] == inSep )
		{
			ret.push_back( string(str + old , 0 , i - old ) );
			old = i + 1;
		}
	}
	if (i != old)
	{
		ret.push_back( str + old );
	}

	return ret;
}

bool RConv::isKanji(unsigned char in1st,unsigned char in2nd)
{
	return (iskanji1st(in1st) && iskanji2nd(in2nd));
}


string RConv::atok(const string & inStr)
{
	string ret = inStr;

	static char * convertBaseTable[] = {"�O","�P","�Q","�R","�S","�T","�U","�V","�W","�X"};
	static char * convertSmallBaseTable[] = {"0","1","2","3","4","5","6","7","8","9"};

	for(int i = TABLE_COUNT_OF(convertBaseTable) -1 ; i >= 0  ; i-- )
	{
		ret = Replace( ret , convertBaseTable[i] , convertSmallBaseTable[i] );
	}

	const char* p = ret.c_str();
	const char* start = p;
	const char* found = NULL;

	string ret2;
	while(*p)
	{
			if (found == NULL)
			{
				if (!iskanji1st(*p) && *p >= '0' && *p <= '9')
				{
					//�����̊J�n.

					//�����܂ł͕����������킯�ł�����L�^.
					found = p;
					ret2 += string(start , 0 , found - start);
				}
			}
			else
			{
				if ( !(!iskanji1st(*p) && *p >= '0' && *p <= '9' ) )
				{
					//�����̏I���
					string num = string(found , 0 , p - found);
					found = NULL;
					start = p;

					ret2 += NumberToKansuji( atoi( num.c_str() ) );
				}
			}
			if (iskanji1st(*p))	p +=2;
			else				p++;
	}
	if (found == NULL)
	{
		//�����܂ł͕����������킯�ł�����L�^.
		found = p;
		ret2 += string(start , 0 , found - start);
	}
	else
	{
		//�����̏I���
		string num = string(found , 0 , p - found);
		found = NULL;

		ret2 += NumberToKansuji( atoi( num.c_str() ) );
	}

	return ret2;
}
string RConv::atos(const string & inStr)
{
	string ret = inStr;

	static char * convertBaseTable[] = {"�O","�P","�Q","�R","�S","�T","�U","�V","�W","�X"};
	static char * convertSmallBaseTable[] = {"0","1","2","3","4","5","6","7","8","9"};

	for(int i = TABLE_COUNT_OF(convertBaseTable) -1 ; i >= 0  ; i-- )
	{
		ret = Replace( ret , convertBaseTable[i] , convertSmallBaseTable[i] );
	}

	static char * convertKansujiTable[] = {"��","��","��","�O","�l","��","�Z","��","��","��","�\","�S","��","��"};
	class __Eval
	{
	public:
		static bool isHit(const char* p , const char** table , int tableSize)
		{
			for(int i = 0 ; i < tableSize ; i ++)
			{
				if ( strncmp( p , table[i] , 2) == 0)
				{
					return true;
				}
			}
			return false;
		}
	};

	const char* p = ret.c_str();
	const char* start = p;
	const char* found = NULL;

	string ret2;
	while(*p)
	{
			if (found == NULL)
			{
				if (iskanji1st(*p) && __Eval::isHit( p , (const char**)convertKansujiTable , TABLE_COUNT_OF(convertKansujiTable) ) )
				{
					//�����̊J�n.

					//�����܂ł͕����������킯�ł�����L�^.
					found = p;
					ret2 += string(start , 0 , found - start);
				}
			}
			else
			{
				if (! (iskanji1st(*p) && __Eval::isHit( p , (const char**)convertKansujiTable , TABLE_COUNT_OF(convertKansujiTable) ) ) )
				{
					//�����̏I���
					string num = string(found , 0 , p - found);
					found = NULL;
					start = p;

					ret2 += num2str( KansujiToNumber( num ) );
				}
			}
			if (iskanji1st(*p))	p +=2;
			else				p++;
	}
	if (found == NULL)
	{
		//�����܂ł͕����������킯�ł�����L�^.
		found = p;
		ret2 += string(start , 0 , found - start);
	}
	else
	{
		//�����̏I���
		string num = string(found , 0 , p - found);
		found = NULL;

		ret2 += num2str( KansujiToNumber( num ) );
	}

	return ret2;
}

string RConv::NumberToKansuji(int inNum)
{
	static char * convertBaseTable[] = {"��","��","��",	"�O","�l","��","�Z","��","��","��"};
	static char * convert3BaseTable[] ={"" , "��"};

	string ret;
	int num = inNum;
	for(int i = TABLE_COUNT_OF(convert3BaseTable) -1 ; i >= 0  ; i-- )
	{
		int shikii = (int)pow((double)10 , (double)(i * 4));

		int man = num / shikii;
		num = num - (man * shikii);

		int p = man;
		int sen = man / 1000;
		ASSERT(sen >= 0 && sen <= 9);
		p = p - (sen * 1000);

		int hyaku = p / 100;
		ASSERT(hyaku >= 0 && hyaku <= 9);
		p = p - (hyaku * 100);

		int jyu = p / 10;
		ASSERT(jyu >= 0 && jyu <= 9);
		p = p - (jyu * 10);

		ASSERT(p >= 0 && p <= 9);

		string str;
		if (sen == 1)		str += "��";
		else if (sen > 1)	str += string("") + convertBaseTable[sen] + "��";
		if (hyaku == 1)		str += "�S";
		else if (hyaku > 1)	str += string("") + convertBaseTable[hyaku] + "�S";
		if (jyu == 1)		str += "�\";
		else if (jyu > 1)	str += string("") + convertBaseTable[jyu] + "�\";
		if (p > 0)			str += convertBaseTable[p];

		if (str != "")		str += convert3BaseTable[i];

		ret += str;
	}

	return ret;
}

int RConv::KansujiToNumber(const string & inStr)
{
	static char * convertBaseTable[] = {"��","��","��",	"�O","�l","��","�Z","��","��","��"};
	static char * convert2BaseTable[] ={"�\","�S","��"};
	static char * convert3BaseTable[] ={"��"};

	const char * p = inStr.c_str();
	int ret = 0;
	while( *p )
	{
		int pool = 1;
		for(int i = 0 ; i < TABLE_COUNT_OF(convertBaseTable) ; i++ )
		{
			if ( strncmp( p , convertBaseTable[i] , 2) == 0)
			{
				pool = i;
				p += 2;
				break;
			}
		}
		for(i = 0 ; i < TABLE_COUNT_OF(convert2BaseTable) ; i++ )
		{
			if ( strncmp( p, convert2BaseTable[i] , 2) == 0)
			{
				switch(i)
				{
				case 0:
					pool *= 10;
					break;
				case 1:
					pool *= 100;
					break;
				case 2:
					pool *= 1000;
					break;
				default:
					ASSERT(0);
					break;
				}
				p += 2;
				break;
			}
		}
		for(i = 0 ; i < TABLE_COUNT_OF(convert3BaseTable) ; i++ )
		{
			if ( strncmp( p, convert3BaseTable[i] , 2) == 0)
			{
				switch(i)
				{
				case 0:
					pool *= 10000;
					break;
				default:
					ASSERT(0);
					break;
				}
				p += 2;
				break;
			}
		}
		ret += pool;
	}
	return ret;
}

void RConv::test()
{
	ASSERT( RConv::atos("�i���o�[��S�܏\�Z�͐_�̎��S���\���ł��B") == "�i���o�[256�͐_��777�ł��B");
	ASSERT( RConv::atok("�i���o�[256�͐_�̂V�V�V�ł��B") == "�i���o�[��S�܏\�Z�͐_�̎��S���\���ł��B");

	ASSERT( RConv::KansujiToNumber("��S�܏\�Z") == 256);
	ASSERT( RConv::KansujiToNumber("�Z���ܐ�ܕS�O�\��") == 65535);

	ASSERT( RConv::NumberToKansuji(256) == "��S�܏\�Z");
	ASSERT( RConv::NumberToKansuji(65535) == "�Z���ܐ�ܕS�O�\��");

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

	{
		string r;
		RStringList sv = RConv::Explode("ab,c,d");
		ASSERT( sv.size() == 3 );
		ASSERT( (r = sv[0]) == "ab" );
		ASSERT( (r = sv[1]) == "c" );
		ASSERT( (r = sv[2]) == "d" );
	}

	ASSERT(! RConv::isKanji('1' , '1') );
	ASSERT(! RConv::isKanji('1' , '2') );
	ASSERT( RConv::isKanji(0x88 , 0xae) );
}


