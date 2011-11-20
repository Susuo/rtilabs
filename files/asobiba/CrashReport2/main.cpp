
#include "stdafx.h"
#include "NazoNeetWorker.h"
#include "RDiskUtil.h"
#include "RMiniDump.h"
#include <conio.h>
#include <ctype.h>

//�j�[�g�̐�
const int NEET_THREAD_COUNT = 10;


//���O�烁�C���֐��ł���!
int main()
{
	puts("All your thread are belong to us!.");
	puts("");
	puts("");
	puts("�������Ȃ�...");
	
	//�J�����g�f�B���N�g����ۑ����Ă���.
	string currentDirectory = RDiskUtil::getCurrentDirectory();
	
	
	/*
		�X���b�h�N�A�ǂ��s���Ă��������E�E�E
		
		���̃X���b�h�N�͂܂������d�������܂���B
		�ꉞ���삷��X���b�h�Ȃ̂ł����A��x���s�����炢�܂ł����Ă����C���X���b�h�ɋA���Ă��܂���B
		���\�[�X�����̖��ʂƂ��q���񂩂��ڋʂ�H�炤���A�S������������Ȃ�ł��B 
		
		����ȃX���b�h�N�Ȃ̂ł����A�T���v���v���O�����Ƃ��Ă�s�J�C�`���Ă��ƂɍŋߋC�t���܂����B
		���C�悭�g���Ă���΁A���������������ł��B
	*/
	//�j�[�g���Y��
	NazoNeetThread neetThread[NEET_THREAD_COUNT];

	puts("MinuDump ���X���b�h�̒��ԂŒǐՂ��Ă���邩���ׂ�T���v���v���O��������");
	puts("q �܂��́A�X�y�[�X�Ńv���O�������I��.");
	puts("d �� minidump(�q�[�v������)���擾���܂�.");
	puts("b �� minidump(������̂��ׂ�)���擾���܂�.");
	puts("");
	puts("");
	printf("�R�}���h>");

	int i = 0;
	//�X���b�h�X�^�[�g
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//�̃��́� �����I�𖯂ǂ�
		neetThread[i].Start();
	}

	//���͑҂�����
	while( 1 )
	{
		char p = _getch();
		if ( p == 'q') break;				//�����܂�
		if ( p == ' ') break;				//����ł������܂�
		if ( p == 'd') 						//minidump(�q�[�v������)
		{
			string dumpfilename = currentDirectory + "__MiniDumpSmall.dmp";
			printf("�_���v���擾�������B�B�B\r\n");
			fflush(stdout);
			RMiniDump::MiniDump(dumpfilename,MINIDUMP_TYPE(MiniDumpNormal | MiniDumpWithHandleData));
			printf("MiniDump(�s�[�v������)���擾���܂����B %s\r\n" , dumpfilename.c_str() );
		}
		if ( p == 'b') 						//minidump(������̂��ׂ�)
		{
			string dumpfilename = currentDirectory + "__MiniDumpFull.dmp";
			printf("�_���v�擾���Ȃ�\r\n");
			fflush(stdout);
			RMiniDump::MiniDump(dumpfilename,MINIDUMP_TYPE(MiniDumpWithFullMemory | MiniDumpWithHandleData));
			printf("MiniDump(������̂��ׂ�)���擾���܂����B %s\r\n" , dumpfilename.c_str() );
		}
	}
	puts("");
	puts("(�@߄t�)�߶��");
	puts("�I�����ł�...");
	fflush(stdout);

	//�X���b�h�ɏI�����߂��o��
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//����ł����������q!
		neetThread[i].HoldUp();
	}
	//�X���b�h�����S�ɏI������܂őҋ@
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//�j�[�g�����ʂ܂ŉ���̂���߂Ȃ��I
		neetThread[i].Join();
	}

	//���̌��l�͍K���ɕ�炵�܂����Ƃ��B�߂ł������߂ł����B
	//�����܂�. �������C�P�����Ɍ���B
	return 0;
}
