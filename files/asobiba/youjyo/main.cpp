#include "comm.h"
#include "YServer.h"
#include "YYoujyoKiller.h"
#include "YYoujyoGC.h"
#include "YYoujyoLogger.h"

#include <conio.h>
#include <ctype.h>



//�e�X�g
#include "RToken.h"
#include "YRequestPath.h"
#include "RCollect.h"
#include "YDAT.h"
#include "RConv.h"
#include "YDATWritter.h"
#include "YDATReader.h"
#include "RHTTPHeaderAttribute.h"
#include "RHTTPHeader.h"
#include "RHTTPPOSTParser.h"
#include "YBBSManager.h"
#include "YSubject.h"
#include "RAlphabetHash.h"
#include "RDateConv.h"
#include "RHtmlTemplate.h"
#include "YIndexHTML.h"
#include "YReadCGITemplate.h"
#include "YSubBackHTML.h"
#include "RConfigFile.h"
#include "RMimeType.h"
#include "YDeleteWait.h"
#include "RIPFilter.h"
#include "YFilter.h"
#include "BlowFish.h"
#include "RFileDB.h"

/*
readme.html:����� 
OS �@ 
	�@ Windows2000�ȍ~
	WindowsXP�ł̓���͊m�F���Ă��܂���(�������XP�����ĂȂ���[��)
 
�n�[�h �@ 
	�@ ������LAN�J�[�h�ƍ����ȃ�����(256MB�ȏ�K�{)
	���܂ǂ���X�y�b�N�̃}�V���ŃT�[�o���ĂĂ����Ȃ��(��
*/
/*
readme.html:���C�Z���X 

�o�C�i�����C�Z���X �@ 
	�@ �傣�ǁJ��錾�ɒ�G���Ȃ���΁A�����R�ɁB
	�������A�G���Ȃǂɓ]�ځA�Љ��ꍇ�́A
	FAQ�ɂ���Ƃ���A�傣�ǁJ��錾���Љ��X�y�[�X�Ɠ����L���̎��ʂɈ󎚂���K�v������܂��B
 
�\�[�X�R�[�h���C�Z���X �@ 
	�@ �傣�ǁJ��̃\�[�X���g���Ă���Ɩ��L���Ă���������΁A�\�[�X�R�[�h�͎��R�Ɏg���Ă�����Ă��܂��܂���B
	�������A�ꕔ�� apache2 �̃\�[�X�R�[�h���g���Ă��܂��̂ŁA���̕����� apache �̃\�[�X�R�[�h�̃��C�Z���X�ɂ��������Ďg�p���Ă��������B

	apache2�̃\�[�X�R�[�h���g���Ă��镔���́A RDateConv.cpp �̕����ł��B
	http�ł̓��t�𕶎��񂩂� unixtime �ɖ߂����߂̃��[�`���� apache2 ���������ȃ��[�`�����������M���Ȃ��������߂�
	�R�K�s�y�����Ă��������܂����B

	���̑��̃\�[�X�R�[�h�ɂ��ẮA�傣�ǁJ��錾�ɒ�G���Ȃ���΁A�����R�Ɏg�p���Ă��������B
	�������A�傣�ǁJ��T�[�o�̃\�[�X�R�[�h���܂ނƃ}�j���A�����ɋL�q����K�v������܂��B 

�f�ړ��̃��C�Z���X �@ 
	�@ �����ł����R�ɁB
	�������A���̎��_�ōŐV�̂傣�ǁJ��錾���Љ��X�y�[�X�Ɠ����L���̎��ʂɈ󎚂���K�v������܂��B

	�܂�傣�ǁJ���1�ց[�W�����G���́A���̃X�y�[�X�Ɠ����X�y�[�X�܂�A1�y�[�W�܂邲�Ƃ傣�ǁJ��錾��
	������Ȃ��Ă͂Ȃ�܂���B
 
���쌠 �@ 
	�@ ���쌠�� rti �ɂȂ�܂��B
	����ȃ\�t�g�E�F�A�ł����A���{���̖@���ő����ی삳��Ă��܂��B 
	�������߂ۂ̂�������ɂ������Ȃ��悤�ɁA���񂪂��Ďg�p���Ă��������B 

�ŐV�̃��C�Z���X �@ 
	�@ �ŐV�̃��C�Z���X�͏�Ƀ\�[�X�R�[�h�̒��Ɋ܂܂�Ă��܂��B
	�����A�����ɂ����Ă��邱�ƂƁA�\�[�X�R�[�h�̒��ɏ����Ă��郉�C�Z���X�����������ꍇ�A�\�[�X�R�[�h�̕����D�悳��܂��B
	�ꏊ�� main.cpp �� ���C�Z���X�̍��ڂ��������������B 
	�܂�R�R�ł�!!
���̑� �@ 
	�@ ��������܂�����A�v�]�ɏ����Ă������� 

*/
/*
readme.html:FAQ

�傣�ǁJ��T�[�o�ɂ��� �@ 
	�@ �傣�ǁJ��T�[�o�� read.cgi ����сA bbs.cgi �@�\��g�ݍ��񂾁A����web server �ł��B
	�f�����ێ�����̂ɕK�v�ȃt�@�C���͂قƂ�� on memory �ŕێ����Ă��邽�߂ɍ����ɓ��삵�܂��B 
	�����T�[�o�� ���C�L�^��������(�K�́K)��������!!!!! �Ƃ���������ł��u����������܂���v �Ƃ��ŁA
	����ځ[��@�Ȏ�������܂���?
	����Ȏ�����D���Ȑl�B�ɂ����肷��A������ 2ch�R���p�`�ȃT�[�o�ł��B

	KeepAlive , E-TAG , �Ȃǂ̓T�|�[�g���Ă��܂���B
	LastModifyed , Range �ɂ��Ă̓T�|�[�g���Ă��܂��B
	GET , POST ���\�b�h�����T�|�[�g���Ă��܂���B POST �� Bounder �̓T�|�[�g���Ă��܂���B
 
���O�͂Ȃ�ƌĂׂ΂悢�̂ł��傤��? �@ 
	�@ �傣�ǁJ��͓��{��ł͔����ł��Ȃ��������܂�ł��܂��B
	����́A �B��_�ł��� �x�g�v�g ���A���܂�ɐ_���Ȗ��O�̂��ߔ����ł��Ȃ��悤�ɕꉹ�𔲂����悤��
	���܂�Ɉ̑�Ő_���Ȃ��߂ɁA�傣�ǁJ��ƕ\�L���܂��B 
	�����A���̂܂܂ł́A�������Ŏx������������߂ɁA
	��ԋ߂����{��ł��� �悤���� �����蓖�Ă邱�Ƃɂ��A Youjyo �ƕ\�L���܂��B
	����́A�����܂Ŕ�����̏����ł���A�ق��ɈӖ��͂���܂���B �Ȃ��񂾂��Ă΁B
 
���̎g���Ă��� 2ch�u���E�U����M�l�̃T�[�o�݂͂��񂼺�ק �@ 
	�@ �ꉞ�A������[���� �ƁA Live2ch �ł͓��삵�Ă��邱�Ƃ��m�F���Ă��܂��B
	����ȊO�œ��삵�Ȃ����̂�����܂�����A�K���ɃX�����ĂĎ��₵�Ă��������B 
	�Ȃ��A�����u���E�U�� ���R Live2ch �ƂȂ�܂��B
	�����[�h���Ԃ��ŏ��ɂ��Ă��g�p���������B
 
Windows9x�ł͓��삵�܂��� �@ 
	���삵�܂���B
	NT���炵����������Ă��Ȃ� API ���Ăяo���Ă��邩��ł��B
	�T�[�o�̐����� Windows 2000�ȍ~�������߂��܂��B 

���������A�z�̂悤�ɐH����ł��� �@ 
	�@ �Ђ����� on memory �œ��삷��̂ŁA ���Ȃ�̃�����������܂��B
	�܂��A�f���̐��ɂ���Ăǂ�ǂ��������͑����Ă����܂��B 
	���A���݃������Ȃ��10�N�O���猩��΁A�^�_�݂����ȉ��i�Ŕ�����̂ł���񂶂��݂܂��傤�B
	�J���ڕW�Ƃ��Ă� 200MB ���Ȃ��œ��삷��΂����Ǝv���Ă��܂���(w
	�������� 512MB �ȏ��!!!
 
�I���Ɏ��Ԃ�������̂ł��� �@ 
	�@ �X���b�h�v�[�������邽�߁A�������I�����Ă��邩��ł��B
	(�@߄t�)�߶�݂ƕ\�����ꂽ��A���΂炭���҂����������B 

�������ł����� �@ 
	�@ �܂����łȂ�ŋ����Ă�������(w
	�o�O���|���Ă��炦��ƁA���ꂵ���ł��B 

�T�[�o�̕Ȃ� windows �A�v���ł���(�� �@ 
	�@ ���邹�[�A linux? ����ȊJ���������ŁA�X���b�h�̋���������ǂ�OS�Ńv���O�������ł��邩���Ă�Ł[�B 
	VC�ō��A�}���Z�[�B Emacs , vi ���g���Ă����͐l���̔s�k�ҁB
	Emacs�M�҂� Emacs�� VC ��*���S�ɓ���*�@�\�𐮂��Ă��甽�_���Ă��������B �b���͂��ꂩ�炾�B
	(-> �� ���\�b�h�ꗗ�A Shift + SPACE �ŕ⊮�A F1 �Ńw���v�A VC�R���p�`�ȃ}�N��)

	�ǂ����A Linux�p�ɍ��Ƃ����� linux kernel �ɑg�ݍ��݂����ł���(w
 
����̖�] �@ 
	�@ �X�Ȃ鍂�����A�o�O�ׂ��A�ڎw��WebServer�V�F�ANo1�A���E���e�A���E���� 



	�ڎw�����z�̐��E 
	�}�C�N �₟�A�{�u�A ����ŃT�[�o���J�n�������񂾂���? 
	�{�u �������AI�~S���g���ĊȒP��WebServer��ݒ肵���񂾁B 
	�}�C�N ���܂ǂ��AI�~S�Ȃ�āB
	���ꂩ��́A �傣�ǁJ�傳�B  
	�{�u �傣�ǁJ��? �Ȃ񂾂������? 
	�}�C�N ���ꂪ�A�W���p�j�[�Y���˂̂������C�J���^^h^h^h^h WebServer �炵����B 
	�{�u ������A���ꂷ�����ˁB����Ɍf�����f�B�t�H���g�œ����Ă���Ȃ�ĕ֗����ˁB 
	�}�C�N ��������A���ꂩ��͂傣�ǁJ�傾��B 
	�{�u ����A���ꂩ��͂傣�ǁJ��̎��ゾ�� 

	���O�͂��ׂċ�z��̂��̂ł��B���݂̂����Ȃ���̂ł�����܂���B

	�܂��߂ɏ����ƁA
	2.0 �� �C���`�L akamai �����ɂ�镉�ו��U
	3.0 �� P2P �ɂ�镉�ו��U���˂���Ă܂����B
 
�f�ڂɂ��� �@ 
	�@ �����ł����R�ɁB
	�������A���̎��_�ōŐV�̂傣�ǁJ��錾���Љ��X�y�[�X�Ɠ����L���̎��ʂɈ󎚂���K�v������܂��B

	�܂�傣�ǁJ���1�ց[�W�����G���́A���̃X�y�[�X�Ɠ����X�y�[�X�܂�A1�y�[�W�܂邲�Ƃ傣�ǁJ��錾��
	������Ȃ��Ă͂Ȃ�܂���B 
	�\�[�X�R�[�h���C�Z���X �@ 
	�@ �傣�ǁJ��̃\�[�X���g���Ă���Ɩ��L���Ă���������΁A�\�[�X�R�[�h�͎��R�Ɏg���Ă�����Ă��܂��܂���B
	�������A�ꕔ�� apache2 �̃\�[�X�R�[�h���g���Ă��܂��̂ŁA���̕����� apache �̃\�[�X�R�[�h�̃��C�Z���X�ɂ��������Ďg�p���Ă��������B

	apache2�̃\�[�X�R�[�h���g���Ă��镔���́A RDateConv.cpp �̕����ł��B
	http�ł̓��t�𕶎��񂩂� unixtime �ɖ߂����߂̃��[�`���� apache2 ���������ȃ��[�`�����������M���Ȃ��������߂�
	�R�K�s�y�����Ă��������܂����B

	���̑��̃\�[�X�R�[�h�ɂ��ẮA�傣�ǁJ��錾�ɒ�G���Ȃ���΁A�����R�Ɏg�p���Ă��������B
	�������A�傣�ǁJ��T�[�o�̃\�[�X�R�[�h���܂ނƃ}�j���A�����ɋL�q����K�v������܂��B  
*/
void main()
{
	puts("startup.....");

	try
	{
		//�T�[�o�̐ݒ�̓ǂݍ���.
		YHttpdConf::getInstance()->Load("httpd.conf");
		//Mime�^�C�v�̓ǂݍ���.
		RMimeType::getInstance()->Load("mime.types");
		//�t�B���^�[����IP
		YFilter::getInstance()->Load();
		//BBS��o�^.
		YBBSManager::getInstance()->ReloadBBS();
	}
	catch(RException e)
	{
		printf("�ݒ�t�@�C����ǂݍ��ݒ��ɃG���[���������܂���:%s\r\n",e.getMessage() );
		abort();
	}
	puts("OK... �ݒ�t�@�C����ǂݍ��ݏI���܂����B");

#ifndef _NO_TEST_MODE
	//�e�X�g���s���܂��B
	YDeleteWait::test();
	RDateConv::test();
	RToken::test();
	YRequestPath::test();
	YDAT::test();
	RConv::test();
	YDATWritter::test();
	YDATReader::test();
	RHTTPHeaderAttribute<string>::test();
	RHTTPHeader::test();
	RHTTPPOSTParser::test();
//	YSubject::test();
	RAlphabetHash::test();
	RHtmlTemplate::test();
	YIndexHTML::test();
	YReadCGITemplate::test();
	YSubBackHTML::test();
	RConfigFile::test();
	RMimeType::test();
	RMmap::test();
	RIPFilterData::test();
	RIPFilter::test();
	RBlowFish::test();
	RFileDB::test();

	RCollect<string ,int > rc;
	rc["10"] = new int;
#endif

	puts("winsock �����������܂�...");

	WSADATA wsaData;
	WSAStartup(2 , &wsaData);
	printf("winsoc�o�[�W���� = %d.%d\n�L�q = %s\n��� = %s\n" ,
		(BYTE)wsaData.wHighVersion , wsaData.wHighVersion >> 8 ,
		wsaData.szDescription ,	wsaData.szSystemStatus
	);
	puts("winsock ���������ɐ������܂���.");

	YBBSManager::getInstance();
	{
		YServer			yServer;
		YYoujyoKiller	youjyoKiller;
		YYoujyoGC		youjyoGC;
		YYoujyoLogger	youjyoLogger;

		yServer.Start();
		youjyoLogger.Start();
		youjyoKiller.Start();
		youjyoGC.Start();

		puts("�傣�ǁJ��L�^��������(�K�́K)��������!!!!!");
		puts("�I���� q / SPACE , �������_���v�� d , �S�f�[�^�̃t���b�V���� f ����A���ɂ�����܁�");

		while( 1 )
		{
			char p = _getch();
			if ( p == 'q') break;				//�����܂�
			if ( p == ' ') break;				//����ł������܂�
			if ( p == 'd') //dump
			{
				YBBSManager::getInstance()->Dump();
			}
			if ( p == 'f') //flush
			{
				YBBSManager::getInstance()->Flush();
			}
		}
		puts("(�@߄t�)�߶��");

		puts("�T�[�o���~���Ă��܂�");
		yServer.Shutdown();
		puts("YoujyoLogger���~���Ă��܂�");
		youjyoLogger.Shutdown();
		puts("YoujyoKiller���~���Ă��܂�");
		youjyoKiller.Shutdown();
		puts("YoujyoGC���~���Ă��܂�");
		youjyoGC.Shutdown();
		Sleep(2);						//���܂��Ȃ�
		yServer.Join();
		puts("�T�[�o�̒�~���������܂���");
		youjyoLogger.Join();
		puts("YoujyoLogger�̒�~���������܂���"); 
		youjyoKiller.Join();
		puts("YoujyoKiller�̒�~���������܂���");
		youjyoGC.Join();
		puts("YoujyoGC�̒�~���������܂���");
		Sleep(2);						//���܂��Ȃ�
	}

	WSACleanup();

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

}
