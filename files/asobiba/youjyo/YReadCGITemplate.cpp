// YReadCGITemplate.cpp: YReadCGITemplate �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YReadCGITemplate.h"
#include "YIndexHTML.h"
#include <math.h>
#include "YDATReader.h"
#include "YDATWritter.h"
#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YReadCGITemplate::YReadCGITemplate()
{

}

YReadCGITemplate::~YReadCGITemplate()
{

}

void YReadCGITemplate::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Template.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\readcgi.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)��readcgi.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	//�y�[�W�����[�v
	PageJump	=	this->Template.Select("YPageJump");
	//�O100��
	IfBefor100	=	this->Template.Select("YIfBefor100");
	//��100��
	IfNext100	=	this->Template.Select("YIfNext100");
	//�������ݕ\������. ���C���ł��ȁB
	Kakiko	=	this->Template.Select("YKakiko");
	//���X����<!--?V:YResAlert-->�𒴂��Ă��܂��B<!--?V:YResOver-->�𒴂���ƕ\���ł��Ȃ��Ȃ��B
	IfResWarning	=	this->Template.Select("YIfResWarning");
	//�T�C�Y��<!--?V:YSizeAlert-->KB�𒴂��Ă��܂��B<!--?V:YSizeOver-->KB�𒴂���ƕ\���ł��Ȃ��Ȃ��B
	IfSizeWarning	=	this->Template.Select("YIfSizeWarning");
	//���X����<!--?V:YResOver-->�𒴂��Ă��܂��B�c�O�Ȃ���S���͕\�����܂���B
	IfResOver	=	this->Template.Select("YIfResOver");

	this->BoardName = inBoardName;
	this->SettingTXT =	inSettingTXT;
}

/**
 * make:					�w�肳�ꂽ�͈͓��ŃX���b�h�̒��g�� html �ŕ\������.
 *
 * @param inDAT				�g�p���� DAT.
 * @param inStart			�J�n�ʒu
 * @param inEnd 
 * @param is1san 
 * @return const string 
 */
const string YReadCGITemplate::make(YDAT* inDAT ,unsigned int inStart ,unsigned int inEnd , bool is1san) const
{
	RStringMap	masterSmap;

	const int res = inDAT->getLastNo();
	const int size = inDAT->getDatSize();
	//�x���\���̍쐬.
	if ( res >= MAX_NUMBER )
	{
		RStringMap	smap;
		smap["YResOver"] = num2str(MAX_NUMBER);
		masterSmap["YIfResOver"] = IfResOver->Replace(&smap);
	}
	else if ( size >= WARNING_DAT_SIZE )
	{
		RStringMap	smap;
		smap["YSizeOver"] = num2str(WARNING_DAT_SIZE);
		masterSmap["YIfResOver"] = IfResOver->Replace(&smap);
	}
	else if ( res >= WARNING_NUMBER )
	{
		RStringMap	smap;
		smap["YResAlert"] = num2str(WARNING_NUMBER);
		smap["YResOver"] = num2str(MAX_NUMBER);
		masterSmap["YIfResWarning"] = IfResWarning->Replace(&smap);
	}
	
	//�y�[�W���̍쐬.
	{
		const int page = ceil(res / 100) + 1;
		string pagestr;
		pagestr.reserve(page * 100);	//100�͓K��.
		for(int i = 1 ; i < page ; i++)
		{
			RStringMap	smap;
			smap["YPageFrom"] = num2str(i * 100 + 1);
			smap["YPageTo"] = num2str( i * 100 + 100);
			pagestr += PageJump->Replace(&smap);
		}
		masterSmap["YPageJump"] = pagestr;
	}

	//�� 100.
	{
		RStringMap	smap;
		smap["YPageFrom"] = num2str(inEnd + 1);
		smap["YPageTo"] = num2str(inEnd + 101);
		masterSmap["YIfNext100"] = IfNext100->Replace(&smap);
	}
	//�O 100.
	if (inStart > 1)
	{
		RStringMap	smap;
		if (inStart <= 101)
		{
			smap["YPageFrom"] = "";
		}
		else
		{
			smap["YPageFrom"] = num2str( inStart - 101 );
		}
		smap["YPageTo"] = num2str(inStart - 1);
		masterSmap["YIfBefor100"] = IfBefor100->Replace(&smap);
	}

	//�������ݕ��͂̕\��.
	{
		RPack pack;

		// 0 ����͂��߂邩��B
		if (inStart != 0) inStart--;	//unsigned �Ƃ������Ƃ�Y����.

		string thread;
		if (inEnd < inStart)
			thread.reserve(ONE_RES_MAX * 1 );
		else
			thread.reserve(ONE_RES_MAX * (inEnd-inStart + 2) );
		//1������?
		if (is1san)
		{
			if ( inDAT->ReadAt(&pack , 0 , 1 ) )
			{
				RStringMap	smap;
				YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
				thread += Kakiko->Replace(&smap);

				//���ł�1���������̂ŁA�X�L�b�v.
				if (inStart == 0)  inStart= 1;
			}
		}

		//����ȍ~���쐬.
		if ( inDAT->ReadAt(&pack , inStart , inEnd ) )
		{
			const char * p = (const char*) pack.getMemory();
			const char * m = p + pack.getSize();
			for(int i = inStart; ; i++)
			{
				if (!p || p >= m) break;

				RStringMap	smap;
				p = YDATReader::makeHTML( &smap , i+1  , p);
				thread += Kakiko->Replace(&smap);
			}
		}
		masterSmap["YKakiko"] = thread;
	}

	/*ChangeLog.txt:2003/01/23	�V�����X�̕\���� 2- �ɌŒ肳��Ă����̂��C��.
	*/
	masterSmap["YThreadResNumber"] = num2str(res);			//�������݌���.
	masterSmap["YThreadSubject"] = inDAT->getSubject();
	masterSmap["YDATSize"] = num2str( (int)(size / 1024) );	//KB
	masterSmap["YItaName"] = BoardName;
	masterSmap["YThreadNumber"] = num2str( inDAT->getDatNumber() );
	masterSmap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();

	return Template.Replace(&masterSmap);
}


void YReadCGITemplate::test()
{
	{
		YReadCGITemplate read;
		YDATCache Cache;
		YSubject Subject;
		string board;
		YSettingTXT setting;

		board = "template3";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		setting.Load(board);
		read.Load(board,&setting);
		Cache.setBoardName(board);

		//700���炢��������ł݂�.
		//�b�͂��ꂩ�炾�B
		{
			{
				const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "test!" , "1test thread." ,"");
				RPackSuper pack;
				pack.Write(msg.c_str() , msg.size() );
				YDAT* dat = Cache.getNewDAT( 1000 );
				dat->Append(&pack);
				dat->setSubject( "1test thread." );
				Subject.Age(dat);
				Cache.Release(dat);
			}
			for(int i = 1 ; i < 700 ; i++)
			{
				const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "test!" + num2str(i+1) ,"");
				RPackSuper pack;
				pack.Write(msg.c_str() , msg.size() );
				YDAT* dat = Cache.getDAT( 1000 );
				dat->Append(&pack);
				Subject.Age(dat);
				Cache.Release(dat);
			}
			YDAT* dat = Cache.getDAT( 1000 );
			// �t�]����.
			{ 
				string ret = read.make( dat , 100 , 0 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test8.html");
			}
			// 0 - 0 ��\�����Ă݂�. 1�����\��.
			{ 
				string ret = read.make( dat , 0 , 0 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test7.html");
			}
			// 0 - 200 ��\�����Ă݂�.
			{
				string ret = read.make( dat , 0 , 200 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test6.html");
			}
			// 101 - 200 ��\�����Ă݂�.
			{
				string ret = read.make( dat , 101 , 200 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test5.html");
			}
			Cache.Release(dat);
		}
		//���肦�Ȃ��͈͂̎w��. �����ǁA1��\������t���O��OK. 1�����\�������.
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "test!" , "1test thread.","" );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1100 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 100 , 1001 , true);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test4.html");
			Cache.Release(dat);
		}
		//���肦�Ȃ��͈͂̎w��. �����\������Ȃ�.
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1200 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 100 , 1001 , false);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test3.html");
			Cache.Release(dat);
		}
		//1�����Ȃ̂ɁA 1 ��\������t���O�����낷. ����ς�1�������\������邪����.
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1300 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 1 , 1 , false);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test2.html");
			Cache.Release(dat);
		}
		//1�����Ȃ̂ɁA 1 ��\������t���O�����ĂāA 1 ��\������悤�ɂ���. 1�������\������邪����.
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1400 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 1 , 1 , true);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test1.html");
			Cache.Release(dat);
		}
	}

	puts("YReadCGITemplate::test() OK");
}

