// RConfigFile.cpp: RConfigFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RConfigFile.h"
#include "RConv.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RConfigFile::RConfigFile()
{

}

RConfigFile::~RConfigFile()
{

}

/**
 * Load:				�ݒ�t�@�C����ǂݍ���.
 *
 * @param inFilename	�t�@�C����
 * @return void 
 */
void RConfigFile::Load(const string inFilename)
{
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}
	char buf[MAX_PATH];

	while( !feof(fp) )
	{
		if ( fgets(buf , MAX_PATH , fp) == NULL ) break;

		if ( buf[0] == '\0' || buf[0] == '#' ) continue;

		const char * eq = strchr( buf , '=');
		if (eq == NULL) continue;

		string key = RConv::rtrim( RConv::ltrim( string(buf , 0 , eq - buf) ) );
		string value = RConv::rtrim( RConv::ltrim( string(eq + 1) ) );

		Setting[key] = value;
	}
	fclose(fp);
}


void RConfigFile::test()
{
	RConfigFile	rcf;
	rcf.Load("conftest/test1.txt");

	string ret;
	ASSERT( (ret = rcf.Setting["abc"]) == "10");
	ASSERT( (ret = rcf.Setting["xyz"]) == "20");
	ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
	ASSERT( (ret = rcf.Setting["meid"]) == "mahoro");

	puts("RConfigFile::test() OK");
}
