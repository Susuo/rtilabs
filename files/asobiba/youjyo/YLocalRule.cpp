// YLocalRule.cpp: YLocalRule �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YLocalRule.h"
#include "YHttpdConf.h"
#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YLocalRule::YLocalRule()
{

}

YLocalRule::~YLocalRule()
{

}

//���[�J�����[���̓ǂݍ���.
void YLocalRule::Load(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/head.txt";

	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		//���[�J�����[���Ȃ�
		return ;
	}

	FILE * fp =  fopen(filename.c_str() , "rb" );
	if (!fp)
	{
		//���[�J�����[���Ȃ�.
		return ;
	}

	int size = statbuf.st_size;
	char * buf =  new char[size + 1];

	size = fread( buf ,sizeof(char), size ,fp);
	fclose(fp);

	//���܂��Ȃ�(w
	buf[size] = '\0';

	LocalRule = buf;

	delete buf;
}
