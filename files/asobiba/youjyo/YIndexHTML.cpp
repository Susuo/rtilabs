// YIndexHTML.cpp: YIndexHTML �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YIndexHTML.h"
#include "YDATWritter.h"
#include "YDATReader.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YIndexHTML::YIndexHTML()
{
}

YIndexHTML::~YIndexHTML()
{
}

void YIndexHTML::Setting(const string inBoardName  ,const YSettingTXT*	inSettingTXT)
{
	try
	{
		AllTemplate.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\index.tpl");
	}
	catch(RException e)
	{
		printf("��(%s)��index.tpl�e���v���[�g�ǂݍ��ݒ��ɗ�O�����m���܂����B �����ɏI�������Ă��������B\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	//�X���b�h�ꗗ
	ThreadIndex = AllTemplate.Select("YThreadIndex");
	//�X���b�h�P��
	ThreadsTemplate = AllTemplate.Select("YThreads");
	//��������}�邽�߂ɃX���b�h�ŌŒ�̂Ƃ���̕�����؂�o�������c
	SubThreadTemplate = (ThreadsTemplate == NULL) ? NULL : ThreadsTemplate->Select("YSubThread");
	//�ꏑ������
	KakikoTemplate = (SubThreadTemplate == NULL) ? NULL : SubThreadTemplate->Select("YKakiko");

	ASSERT(ThreadIndex);
	ASSERT(ThreadsTemplate);
	ASSERT(SubThreadTemplate);
	ASSERT(KakikoTemplate);

	this->BoardName = inBoardName;
	this->SettingTXT =	inSettingTXT;
}


/**
 * Load:			Subject ����擪�� TOP_DAT ���擾�� index.html ���쐬���܂�.
 *
 * @param ioCache	�L���b�V��
 * @param inSubject	�T�u�W�F�N�g
 * @return void 
 */
void YIndexHTML::Load(YDATCache* ioCache , YSubject* inSubject, YLocalRule* inLocalRule)throw(RIOCannotOpenException)
{
	RAutoWriteSpinLock al(&Lock);

	const string byReadcgi = "../test/read.cgi/" + BoardName + "/" ;

	string threads;
	threads.reserve(ONE_RES_MAX * ALL_PAGE_RES);
	//���C���R���e���c�̍쐬.
	{
		DATNUMBER numbers[TOP_DAT];
		//�擾.
		int datCount = inSubject->getNumbers(numbers , TOP_DAT);
		for(int i = 0; i < datCount ; i++)
		{
			DATNUMBER datnumer = numbers[i] ; 
			YDAT * dat = ioCache->getDAT( datnumer );
			const int end = dat->getLastNo();
			const int start = end > ONE_DAT_TAIL ? (end-ONE_DAT_TAIL) : (1) ;

			string kakiko;
			kakiko.reserve(ONE_RES_MAX * (ONE_DAT_TAIL + 1));
			//1�����.
			{
				RPack pack;
				if ( dat->ReadAt(&pack , 0 , 1) )
				{
					RStringMap	smap;
					YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
					kakiko += KakikoTemplate->Replace(&smap);
				}
			}

			//����ȍ~���쐬.
			RPack pack;
			if ( dat->ReadAt(&pack , start , end ) )
			{
				const char * p = (const char*) pack.getMemory();
				const char * m = p + pack.getSize();
				for(int i = start; ; i++)
				{
					if (!p || p >= m) break;

					RStringMap	smap;
					p = YDATReader::makeHTML( &smap , i+1  , p);
					kakiko += KakikoTemplate->Replace(&smap);
				}
			}
			const string readcgi = byReadcgi + num2str( datnumer ) + "/";

			// Subthread �̍\�z.
			RStringMap	smap;
			smap["YThreadResNumber"] = num2str( dat->getLastNo() );
			smap["YThreadSubject"] = dat->getSubject();
			smap["YItaName"] = BoardName;
			smap["YThreadNumber"] = num2str( datnumer );
			smap["YThreadALL"] = readcgi;
			smap["YThreadL50"] = readcgi + "l50";
			smap["YThreadL1-100"] =  readcgi + "-100";
			smap["YKakiko"] = kakiko;
			string subthread = SubThreadTemplate->Replace(&smap) ;

			// SubThread �����������L���b�V���ɁB
			Cache.setCache(i , subthread);

			//Threads ���쐬�B
			int index = i + 1;
			smap["YThreadIndexNo"] = num2str(index);
			smap["YThreadLocalJumpBack"] = "#" + num2str( (index-1) <= 0 ? datCount : index-1 );
			smap["YThreadLocalJumpNext"] = "#" + num2str( (index+1) > datCount ? 1 : index+1 );
			smap["YSubThread"] = subthread;
			
			threads += ThreadsTemplate->Replace(&smap) ;
		}
	}

	//��ʂ�����̂ł���[.
	RStringMap	smap;

	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YItaName"] = BoardName;
	smap["YThreads"] = threads;
	smap["YThreadIndex"] = inSubject->DrawThreadIndex(ThreadIndex , byReadcgi , SHOW_INDEX_MAX);
	smap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	this->IndexHTML = AllTemplate.Replace(&smap);
}


void YIndexHTML::Write(YDAT* inDAT ,YSubject* inSubject, YLocalRule* inLocalRule, int  oldPos , bool isAge )
{
	RAutoWriteSpinLock al(&Lock);

	if (! isAge )
	{
		// sage �� top 10 �ɓ����Ă��Ȃ���� index.html �Ɋ܂߂Ȃ��Ă��ǂ��̂� return .
		//�������A���̃V�i���I�͂قƂ�Ǌ��҂ł��Ȃ��B
		//�����Ď������ƈ�̂��ꂾ�� age ��A�ق��̃X���� age ��Ȃ��̂� ���ʂƂ���
		//��������͕\��������ςȂ��ɂȂ�̂��B
		if ( oldPos >= TOP_DAT ) return ;
	}

	DATNUMBER datnumer = inDAT->getDatNumber();

	const int end = inDAT->getLastNo();
	const int start = end > ONE_DAT_TAIL ? (end-ONE_DAT_TAIL) : (1) ;

	//������ DAT �̍쐬.
	string kakiko;
	kakiko.reserve(ONE_RES_MAX * (ONE_DAT_TAIL + 1));
	//1�����.
	{
		RPack pack;
		if ( inDAT->ReadAt(&pack , 0 , 1) )
		{
			RStringMap	smap;
			YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
			kakiko += KakikoTemplate->Replace(&smap);
		}
	}
	//����ȍ~���쐬.
	RPack pack;
	if ( inDAT->ReadAt(&pack , start , end ) )
	{
		const char * p = (const char*) pack.getMemory();
		const char * m = p + pack.getSize();
		for(int i = start; ; i++)
		{
			if (!p || p >= m) break;

			RStringMap	smap;
			p = YDATReader::makeHTML( &smap , i+1  , p);
			kakiko += KakikoTemplate->Replace(&smap);
		}
	}

	const string readcgi = "../test/read.cgi/" + BoardName + "/" + num2str( datnumer ) + "/";

	// Subthread �̍\�z.
	RStringMap	smap;
	smap["YThreadResNumber"] = num2str( inDAT->getLastNo() );
	smap["YThreadSubject"] = inDAT->getSubject();
	smap["YItaName"] = BoardName;
	smap["YThreadNumber"] = num2str( datnumer );
	smap["YThreadALL"] = readcgi;
	smap["YThreadL50"] = readcgi + "l50";
	smap["YThreadL1-100"] =  readcgi + "-100";
	smap["YKakiko"] = kakiko;

	kakiko = SubThreadTemplate->Replace(&smap);

//	Cache.Dump();

	if ( isAge )
	{
		//�V�K age
		if (oldPos >= TOP_DAT)	Cache.NewAge(kakiko);
		//������ DAT �� age . (�ӂ[�ň�ԍl������V�i���I)
		else					Cache.Age(kakiko,oldPos);
	}
	else
	{
		//sage �����ł͊��҂ł������B
		Cache.Sage(kakiko,oldPos);
	}

//	Cache.Dump();

	this->LoadWithCache(inSubject, inLocalRule);

//	Cache.Dump();

	Update();
}

void YIndexHTMLAlloc::Dump()
{
	{
		printf("===================== YIndexHTMLAlloc::Dump() ===========================\r\n");
		for(int i = 0; i < TOP_DAT ; i++)
		{
			printf("Cache[%d] as %x --> \r\n" ,i , &Cache[i]  );
		}
	}
	{
		for(int i = 0; i < TOP_DAT ; i++)
		{
			printf("CacheIndex[%d] as %x --> \r\n" ,i , CacheIndex[i]  );
		}
		printf("===================== --------------------- ===========================\r\n");
	}
	//���ȃ`�F�b�N
	//�����Ƃ���������Ă���|�C���^�͂Ȃ�.
	{
		for(int i = 0; i < TOP_DAT ; i++)
		{
			for(int n = i+1; n < TOP_DAT ; n++)
			{
				if (CacheIndex[i] == CacheIndex[n])
				{
					printf( "�����Ƃ�����w���Ă��܂�. ");
					ASSERT(0);
				}
			}
		}
	}
}



/**
 * LoadWithCache:					Subject ����擪�� TOP_DAT ���擾�� index.html ���쐬���܂�.
 *
 * @param ioCache	�L���b�V��
 * @param inSubject	�T�u�W�F�N�g
 * @return void 
 */
void YIndexHTML::LoadWithCache(YSubject* inSubject, YLocalRule* inLocalRule)
{
	const string byReadcgi = "../test/read.cgi/" + BoardName + "/" ;
	string threads;
	threads.reserve(ONE_RES_MAX * ALL_PAGE_RES);
	//���C���R���e���c�̍쐬.
	{
		RStringMap	smap;
		DATNUMBER numbers[TOP_DAT];
		//�擾.
		int datCount = inSubject->getNumbers(numbers , TOP_DAT);
		for(int i = 0; i < datCount ; i++)
		{
			DATNUMBER datnumer = numbers[i]; 
			int index = i + 1;

			smap["YThreadIndexNo"] = num2str(index);
			smap["YThreadLocalJumpBack"] = "#" + num2str( (index-1) <= 0 ? datCount : index-1 );
			smap["YThreadLocalJumpNext"] = "#" + num2str( (index+1) > datCount ? 1 : index+1 );
			smap["YSubThread"] = Cache.getCache(i);
			string r = Cache.getCache(i);
			threads += ThreadsTemplate->Replace(&smap) ;
		}
	}

	RStringMap	smap;

	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YItaName"] = BoardName;
	smap["YThreads"] = threads;
	smap["YThreadIndex"] = inSubject->DrawThreadIndex(ThreadIndex , byReadcgi , SHOW_INDEX_MAX);
	smap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	this->IndexHTML = AllTemplate.Replace(&smap);
}


void YIndexHTML::Show(RPack * outRPack) const
{
	RAutoReadSpinLock al(&Lock);

	outRPack->Write( IndexHTML.c_str()  , IndexHTML.size() );
}

void YIndexHTML::test()
{
	{
		YIndexHTML IndexHTML;
		YDATCache Cache;
		YSubject Subject;
		string board;

		board = "template2";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		Cache.setBoardName(board);
//		Subject.Load(board);

		YSettingTXT setting;
		YLocalRule	localRule;

		setting.Load(board);
		IndexHTML.Setting( board ,&setting );
		IndexHTML.Load(&Cache,&Subject,&localRule);

		//�ŏ��ɉ����Ȃ��̂ŁA�X���b�h������Ă݂܂��B 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "1test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1000 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject ,&localRule, INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test1.html");
			Cache.Release(dat);
		}
		//������X���b�h�����܂��B 2 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "2test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1100 );
			dat->Append(&pack);
			dat->setSubject( "2test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule , INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test2.html");
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "3test thread." ); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1200 );
			dat->Append(&pack);
			dat->setSubject( "3test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test3.html");
			Cache.Release(dat);
		}
		//2�Ԃ̃X���b�h�� age ���܂��B 2 3 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test4.html");
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "4test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1400 );
			dat->Append(&pack);
			dat->setSubject( "4test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test5.html");
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "5test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1500 );
			dat->Append(&pack);
			dat->setSubject( "5test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "6test thread.",""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1600 );
			dat->Append(&pack);
			dat->setSubject( "6test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "7test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1700 );
			dat->Append(&pack);
			dat->setSubject( "7test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}

		//����ɃX���b�h�����܂��B8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "8test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1800 );
			dat->Append(&pack);
			dat->setSubject( "8test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "9test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1900 );
			dat->Append(&pack);
			dat->setSubject( "9test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B10 9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "10test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 2000 );
			dat->Append(&pack);
			dat->setSubject( "10test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//����ɃX���b�h�����܂��B11 10 9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "11test thread." ,""); //�X���b�h�쐬�� age �ɂȂ�.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 2100 );
			dat->Append(&pack);
			dat->setSubject( "11test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test6.html");
			Cache.Release(dat);
		}
		//2�Ԃ̃X���b�h�� age ���܂��B 2 11 10 9 8 7 6 5 4 3 1
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test7.html");
			Cache.Release(dat);
		}
		//1�Ԃ̃X���b�h�� new age ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����MX" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test8.html");
			Cache.Release(dat);
		}
		//2�Ԃ̃X���b�h�� sage ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test9.html");
			Cache.Release(dat);
		}
		//3�Ԃ̃X���b�h�� sage(��ʊO sage) ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1200 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1200 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test10.html");
			Cache.Release(dat);
		}
		//1�Ԃ̃X���b�h�� age ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test11.html");
			Cache.Release(dat);
		}
		//1�Ԃ̃X���b�h�� sage ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test12.html");
			Cache.Release(dat);
		}
		//2�Ԃ̃X���b�h�� sage ���܂��B 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("�ȂȂ�����" , "sage" , "sage�ł���" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//�ǂ��̈ʒu�ɂ��邩����.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test13.html");
			Cache.Release(dat);
		}
	}

	{
		YIndexHTML IndexHTML;
		YDATCache Cache;
		YSubject Subject;
		string board;

		board = "template";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		Cache.setBoardName(board);
		Subject.Load(board);

		YSettingTXT setting;
		YLocalRule	localRule;

		setting.Load(board);
		IndexHTML.Setting( board ,&setting );
		IndexHTML.Load(&Cache,&Subject ,  &localRule );
		RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );

		//�쐬�����t�@�C�������炩���߃e�X�g�ō�����t�@�C���Ɠ����ł��邩�ǂ����`�F�b�N.
		RDebug::FileVerify(path + "/index.html" , path + "/indexcheck.html");
	}

	puts("YIndexHTML::test() OK");
}

