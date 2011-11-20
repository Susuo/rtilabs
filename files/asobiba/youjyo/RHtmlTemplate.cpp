// RHtmlTemplate.cpp: RHtmlTemplate �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RHtmlTemplate.h"
#include <sys/stat.h>

#define RTEMPLATE_LOOP_START	"<!--?LS:"
#define RTEMPLATE_TAG_END		"-->"
#define RTEMPLATE_LOOP_END		"<!--?LE:"

#define RTEMPLATE_VALUE_START	"<!--?V:"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RHtmlTemplate::RHtmlTemplate()
{
	this->Perent = NULL;
}

RHtmlTemplate::~RHtmlTemplate()
{
	CleanUp();
}


void RHtmlTemplate::Load(const string inFilename) throw(RIOCannotOpenException,RParseException)
{
	CleanUp();

	struct _stat statbuf;
	if ( _stat( inFilename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat �̎擾�Ɏ��s" + inFilename);
	}
	int size = statbuf.st_size;
	char * buf =  new char[size + 1];
	
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DAT�t�@�C�����J���܂���ł���" + inFilename);
	}
	size = fread( buf ,sizeof(char), size ,fp);
	fclose(fp);

	buf[size] = '\0';

	this->Parse( buf ,size , 0);
	delete [] buf;
}

//�q���̏ꍇ.
void RHtmlTemplate::Load(const RHtmlTemplate* inPerent ,const string inContect, int inStartLine) throw(RParseException)
{
	CleanUp();

	this->Perent = inPerent;

	this->Parse( inContect.c_str() ,inContect.size() , inStartLine);
}


/**
 * Parse:						���e����͂��A�ÓI�A���[�v�A�u�� �̍��ڕʂɃ��X�g�\���ɕϊ����܂��B
 *								�p�[�X�͉����Ȃ�܂���(w
 *
 * @param inTargetBuffer		��͂����l �Ō�� \0 �������Ă��邱�Ƃ͕ۏ؂����
 * @param inSize				��͂����l�̃T�C�Y
 * @param inStartLine			.tpl �t�@�C���ł�inTargetBuffer������s�� �G���[�s���o�͂���Ƃ��Ɏg��
 * @return void					
 * @exception	RParseException	�\����̓G���[
 */
void RHtmlTemplate::Parse(const char* inTargetBuffer , int inSize, int inStartLine) throw(RParseException)
{
	const char * p = inTargetBuffer;
	const char * territory = inTargetBuffer + inSize;	
	const char* oldpos = p;
	while(1)
	{
		oldpos = p;

		//���[�v�^�O�̌���
		//�� <!--?LS:Kahotan-->
		const char* loopstart = strstr(p , RTEMPLATE_LOOP_START);
		if (loopstart == NULL) break;
		p=loopstart + sizeof(RTEMPLATE_LOOP_START) - 1;

		const char* looptagend = strstr(p , RTEMPLATE_TAG_END);
		if (looptagend == NULL)
		{
			throw RParseException("%d�s��:���[�v�J�n�^�O�����Ă��܂���" , getline(inTargetBuffer , loopstart) + inStartLine);
		}
		// p �` looptagend �܂ł��^�O�̖��O.
		string tagname = string(p , 0 , looptagend - p);

		p=looptagend + sizeof(RTEMPLATE_LOOP_START) - 1;

		//�� <!--?LE:Kahotan-->
		//���̃^�O���T�[�`����.
		const string LoopEndTagName = string(RTEMPLATE_LOOP_END) + tagname + RTEMPLATE_TAG_END;
		//���[�v�^�O�̏I��������.
		const char* loopend = strstr(p , LoopEndTagName.c_str());
		if (loopend == NULL) 
		{
			throw RParseException("%d�s��:���[�v�J�n�͂���̂ɏI��������܂���" , getline(inTargetBuffer , loopstart) + inStartLine);
		}

		//�o�^�������̂ł����A���͂��̃^�O���������K�C�V���c�ł͂Ȃ������ׂ�.
		if ( this->Select(tagname) != NULL)
		{
			throw RParseException("%d�s�ځ`%d�s��:���[�v�J�n,�I���̃^�O(%s)�͌������K�C�V���c�ł�" , 
				getline(inTargetBuffer , loopstart) + inStartLine, 
				getline(inTargetBuffer , loopend) + inStartLine, 
				tagname.c_str() );
		}

		//���[�v�̑O�ɂ���u���^�O�����ׂĎ擾���܂��B
		this->ParseValue(oldpos , loopstart - oldpos , getline(inTargetBuffer , loopstart) );
		//���̃��[�v��u���^�O�Ƃ��ēo�^���܂��B
		ReplaceIndex.push_back( new RHtmlTemplateContent(RHtmlTemplateContent::DynamicContent , tagname) );
		//���[�v���̂�o�^���܂��B
		const char* tagContentStart = looptagend + sizeof(RTEMPLATE_TAG_END)- 1;
		RHtmlTemplate * childTemplate = new RHtmlTemplate();
		Loop[tagname] =  childTemplate;
		childTemplate->Load(this , string(tagContentStart , 0 , loopend - tagContentStart) , getline(inTargetBuffer , loopstart) );

		p = loopend + LoopEndTagName.size();
	}
	//�c��̗̈�̒���.
	this->ParseValue(oldpos , territory - oldpos , getline(inTargetBuffer , oldpos) );
}

/**
 * ParseValue:					���e����͂��A�ÓI�A�u���A�̍��ڕʂɃ��X�g�\���ɕϊ����܂��B
 *								�p�[�X�͉����Ȃ�܂���(w
 *
 * @param inTargetBuffer		��͂����l �Ō�� \0 �������Ă��邱�Ƃ͕ۏ؂����
 * @param inSize				��͂����l�̃T�C�Y
 * @param inStartLine			.tpl �t�@�C���ł�inTargetBuffer������s�� �G���[�s���o�͂���Ƃ��Ɏg��
 * @return void					
 * @exception	RParseException	�\����̓G���[
 */
void RHtmlTemplate::ParseValue(const char* inTargetBuffer,int inSize , int inStartLine) throw(RParseException)
{
	const char * p = inTargetBuffer;
	const char * territory = inTargetBuffer + inSize;	
	const char* oldpos = p;
	while(1)
	{
		oldpos = p;

		//�u���^�O�̌���
		//�� <!--?V:Kahotan-->
		const char* valuestart = strstr(p , RTEMPLATE_VALUE_START);
		if (valuestart == NULL) break;			//������Ȃ�
		if (valuestart >= territory) break;		//�S���͈͊O

		p=valuestart + sizeof(RTEMPLATE_VALUE_START)- 1;

		//�u���^�O�̏I�����T�[�`
		const char* valueend = strstr(p , RTEMPLATE_TAG_END);
		if (valueend == NULL)
		{
			throw RParseException("%d�s��:�u���^�O�����Ă��܂���" , getline(inTargetBuffer , valuestart) + inStartLine );
		}
		if (valueend >= territory)
		{
			throw RParseException("%d�s��:�u���^�O�������������ƂɂȂ��Ă��܂�" , getline(inTargetBuffer , valuestart) + inStartLine );
		}
		//oldpos �` valuestart �܂ł��ÓI�R���e���c�ł��B
		if (valuestart-oldpos > 0)
		{
			ReplaceIndex.push_back( 
				new RHtmlTemplateContent(RHtmlTemplateContent::StaticContent , string(oldpos , 0 , valuestart-oldpos) ) );
		}
		//p �` valueend �܂ł��^�O�̖��O�ł��B
		string tagname = string(p , 0 , valueend - p);
		//�����āA���ꂪ���I�R���e���c�ł��B
		ReplaceIndex.push_back( new RHtmlTemplateContent(RHtmlTemplateContent::DynamicContent , tagname) );

		p=valueend + sizeof(RTEMPLATE_TAG_END)- 1;
	}
	//�c��͂��ׂĐÓI�R���e���c.
	if (territory-oldpos > 0)
	{
		ReplaceIndex.push_back( 
			new RHtmlTemplateContent(RHtmlTemplateContent::StaticContent , string(oldpos , 0 , territory-oldpos) ) );
	}
}


/**
 * getline:				�s���𐔂���
 *
 * @param inStart		��������͂��܂���
 * @param inTarget		�����܂�
 * @return int			�s��
 */
int RHtmlTemplate::getline(const char * inStart , const char* inEnd) const
{
	int count = 0;
	const char* p = inStart;
	while( p < inEnd )
	{
		if (*p == '\n') count++;
		p++;
	}
	return count;
}

/**
 * CleanUp:				new �Ŋm�ۂ������̂̉��
 *
 * @return void 
 */
void RHtmlTemplate::CleanUp()
{
	{
		for(RHtmlTemplateMap::iterator i = Loop.begin() ; i != Loop.end() ; ++i)
		{
			delete (*i).second;
		}
		Loop.clear();
	}
	{
		for(RHtmlTemplateValueMap::iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			delete (*i);
		}
		ReplaceIndex.clear();
	}
}

/**
 * Replace:						�u�����s��
 *
 * @param inReplaceStringMap	�u�����镶����̃}�b�v
 * @return string				���� 
 */
const string RHtmlTemplate::Replace(const RStringMap * inReplaceStringMap) const
{
	int length = 0;
	{
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			const RHtmlTemplateContent* htc = (*i);
			length += htc->getConectLength(inReplaceStringMap);
		}
	}

	string ret ;
	ret.reserve(length);
	{
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			const RHtmlTemplateContent* htc = (*i);
			ret += htc->getConect(inReplaceStringMap);
		}
	}
	return ret;
}

void RHtmlTemplate::Dump() const
{
	{
		printf("===================== RHtmlTemplate::Dump() ===========================\r\n");
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			printf("\r\n===================== part. ===========================\r\n");
			const RHtmlTemplateContent* htc = (*i);
			puts( htc->Dump().c_str() );
		}
		printf("===================== --------------------- ===========================\r\n");
	}
}

void RHtmlTemplate::test()
{
	try
	{
		string ret;

		RHtmlTemplate	t;
		t.Load("test4.tpl");

		RHtmlTemplate* threadindex = t.Select("YThreads");
		ASSERT (threadindex != NULL);

		const RHtmlTemplateContent* htc;

		RHtmlTemplateValueMap::const_iterator i = threadindex->ReplaceIndex.begin();
		htc = (*i);
		ASSERT((ret = htc->Dump()) == 
"\r\n"
"<TABLE cellSpacing=7 cellPadding=3 width=\"95%\" align=center bgColor=#efefef \r\n"
"border=1>\r\n"
"  <TBODY>\r\n"
"  <TR>\r\n"
"    <TD> \r\n"
"      <DL><A name=1></A> \r\n"
"        <DIV align=right><A href=\"http://pc.2ch.net/linux/#menu\">��</A><A \r\n"
"        href=\"http://pc.2ch.net/linux/#16\">��</A><A \r\n"
"        href=\"http://pc.2ch.net/linux/#2\">��</A></DIV>\r\n"
"        <B>�y1:"
		);

	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}
	try
	{
		RHtmlTemplate	t;
		t.Load("test3.tpl");
		ASSERT(t.Loop.size() == 1);
		ASSERT(t.ReplaceIndex.size() == 1);

		RHtmlTemplate* threadindex = t.Select("YThreadIndex");
		ASSERT (threadindex != NULL);

		ASSERT(threadindex->Loop.size() == 0);
		ASSERT(threadindex->ReplaceIndex.size() == 7);

		RStringMap smap;
		smap["YThreadL50"] = "http://www.2ch.net/";
		smap["YThreadIndexNo"] = "�����[��";
		smap["YThreadIndexSubjectAndNo"] = "�������[(�G�L�D�M)ʧʧ (1000)";
		string ret = threadindex->Replace(&smap);

		ASSERT(ret == "\r\n<A href=\"http://www.2ch.net/\" target=body>�����[��:�������[(�G�L�D�M)ʧʧ (1000)</A>\r\n");
		smap["YThreadIndex"] = ret;

		ret = t.Replace(&smap);
		ASSERT(ret == "\r\n<A href=\"http://www.2ch.net/\" target=body>�����[��:�������[(�G�L�D�M)ʧʧ (1000)</A>\r\n");
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}

	try
	{
		RHtmlTemplate	t;
		t.Load("test2.tpl");
		ASSERT(t.Loop.size() == 0);
		ASSERT(t.ReplaceIndex.size() == 6);

		RStringMap smap;
		smap["YThreadIndexNo"] = "�����[��";
		string ret = t.Replace(&smap);

		ASSERT(ret == 
			"this is template test.\r\n"
			"�����[��\r\n"
			"this is template test2.\r\n"
			"aaaaaa�����[��!!!\r\n"
			"�����[��");
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}

	try
	{
		RHtmlTemplate	t;
		t.Load("test1.tpl");
		ASSERT(t.Loop.size() == 0);
		ASSERT(t.ReplaceIndex.size() == 1);
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}
}
