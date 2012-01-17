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
void RConfigFile::Load(const string & inFilename)
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

/**
 * Save:				��������
 *
 * @param inFilename	�t�@�C����
 * @return void 
 */
void RConfigFile::Save(const string & inFilename)
{
	RStringMap	localSetting = Setting;

	string tmpfilename = inFilename + ".tmp";
	FILE * wfp = fopen( tmpfilename.c_str() ,"wb");
	if (wfp == NULL)
	{
		return;
	}
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp != NULL)
	{
			char buf[MAX_PATH];

			while( !feof(fp) )
			{
				if ( fgets(buf , MAX_PATH , fp) == NULL ) break;


				if ( buf[0] == '\0' || buf[0] == '#' )
				{
					fprintf(wfp , "%s" , buf);
					continue;
				}

				const char * eq = strchr( buf , '=');
				if (eq == NULL)
				{
					fprintf(wfp , "%s" , buf);
					continue;
				}

				string key = RConv::rtrim( RConv::ltrim( string(buf , 0 , eq - buf) ) );
				string value = RConv::rtrim( RConv::ltrim( string(eq + 1) ) );

				//���̃f�[�^�͓ǂݍ���ł��܂���?
				RStringMap::iterator i = localSetting.find(key);
				if (i == localSetting.end() )
				{
					//�Ȃ��ꍇ�͍폜
					continue;
				}
				if (value == (*i).second)
				{
					fprintf(wfp , "%s" , buf);
					localSetting.erase(i);
					continue;
				}

				//��������.
				fprintf(wfp , "%s=%s\r\n" , (*i).first.c_str()  , (*i).second.c_str() );

				localSetting.erase(i);
			}
	}
	//�܂���������Œl�͒ǉ�.
	{
		for(RStringMap::iterator i = localSetting.begin() ; i != localSetting.end() ; i++)
		{
			fprintf(wfp , "%s=%s\r\n" , (*i).first.c_str()  , (*i).second.c_str() );
		}
	}
	if (fp) fclose(fp);
	if (wfp) fclose(wfp);

	{
#ifndef _WIN32_WCE
		DeleteFile( inFilename.c_str() );
		MoveFile( tmpfilename.c_str() , inFilename.c_str() );
#else
		MY_USES_CONVERSION;

		DeleteFile( _A2W( inFilename.c_str() ) );
		MoveFile( _A2W( tmpfilename.c_str() ) , _A2W( inFilename.c_str() ) );
#endif
	}
}


void RConfigFile::test()
{
	//���������e�X�g
	{
		RConfigFile	rcf;
		rcf.Load("test/conftest/test2.txt");

		string ret;
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "mahoro");

		rcf.Setting["meid"] = "minawa";
		rcf.Save("test/conftest/test2.txt");

		rcf.Load("test/conftest/test2.txt");
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "minawa");

		rcf.Setting["meid"] = "mahoro";
		rcf.Save("test/conftest/test2.txt");
	}
	//���[�h�e�X�g
	{
		RConfigFile	rcf;
		rcf.Load("test/conftest/test1.txt");

		string ret;
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "mahoro");
	}

	puts("RConfigFile::test() OK");
}