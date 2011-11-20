// SusiePluginManager.cpp: SusiePluginManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePluginManager.h"
#include "Rfiles.h"
#include "DeleteObject.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SusiePluginManager::SusiePluginManager()
{
	PluginLoad("C:\\Program Files\\susie\\");
}

SusiePluginManager::~SusiePluginManager()
{
//	for_each( this->Plugines.begin() , this->Plugines.end() , DeleteObject )
	//�m�ۂ����v���O�C���̏���.
	for(PLUGINES::iterator p = this->Plugines.begin(); p != this->Plugines.end() ; ++p)
	{
		delete *p;
	}
}

//�C���X�^���X�擾
SusiePluginManager*	SusiePluginManager::getInstance()
{
	static SusiePluginManager spm;
	return  &spm;
}

//�g���q�ꗗ�̎擾
SUSIE_EXT SusiePluginManager::getSupportExt()
{
	SUSIE_EXT ret;
	for(EXTMAPPER::iterator p = this->ExtMapper.begin(); p != this->ExtMapper.end() ; ++p)
	{
		ret.push_back((*p).first);
	}
	//�d������菜��.
	ret.erase( unique(ret.begin() , ret.end()) , ret.end() );

	return ret;
}

//�v���O�C�����[�h
string SusiePluginManager::PluginLoad(const string inPluginPath)
{
	string wildpath = inPluginPath + "*.spi";
	WIN32_FIND_DATA data;
	HANDLE p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			//�f�B���N�g���͖���
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			SusiePluginWapper* sp = NULL;
			try
			{
				sp = new SusiePluginWapper;
				sp->Create( inPluginPath + data.cFileName);
				AppendSupportBy(sp);
				this->Plugines.push_back(sp);
			}
			catch(RException e)
			{
				delete sp;
			}
		}
		while( ::FindNextFile( p , &data) );
		::FindClose(p);
	}

	return "";
}

//�T�|�[�g���Ă���v���O�C�����ڂ��
//�݂���Ȃ��ꍇ�� RNotSupportException 
const SusiePluginWapper* SusiePluginManager::getSupportPlugin(const string inFilename,const string inExt) throw()
{
	pair<MULTIMAP_RANGE_ITERATOR,MULTIMAP_RANGE_ITERATOR> p = this->ExtMapper.equal_range(inExt);

	//2K�o�C�g�ǂݍ��݂܂��B
	const int THE2KBYTESIZE = 1024 * 2;
	char the2KByte[THE2KBYTESIZE] = {0};
	FILE *fp = fopen(inFilename.c_str() , "rb");
	if (fp == NULL) throw RNotSupportException("%s �t�@�C�����I�[�v���ł��܂���" , inFilename.c_str() );
	fread(the2KByte , sizeof(char) , THE2KBYTESIZE , fp);
	fclose(fp);

	for ( MULTIMAP_RANGE_ITERATOR i = p.first ; i != p.second ; ++i)
	{
		SusiePluginWapper* plugin = (*i).second;
		
		//�T�|�[�g���Ă܂���?
		if ( plugin->IsSupported( inFilename , the2KByte) )
		{
			return plugin;
		}
	}
	throw RNotSupportException("%s �`���͖��T�|�[�g�ł�" , inFilename.c_str() );
	return NULL;
}

//�Ή�����v���O�C�����}�b�s���O.
//�G���[ RSusieConferException
void SusiePluginManager::AppendSupportBy(SusiePluginWapper* inSusiePlugin) throw()
{
	int no = 0;
	string extes;
	string ext;
	do
	{
		//�T�|�[�g���Ă���`���̎擾
		extes = inSusiePlugin->GetPluginSupportExt(no++);
		//��? ������.
		if ( extes.empty() ) continue;
		//�Ō�ɔԕ�������.
		extes += ';';
		// ; ��؂�Ȃ̂�..
		int findstart = 0;
		int hitpos = 0;
		while(1)
		{
			hitpos = extes.find(';' , findstart);
			if (hitpos == -1) break;
			ext = extes.substr(findstart , hitpos - findstart);
			ext = Rfiles::Filename2Ext(ext);
			this->ExtMapper.insert(pair< string, SusiePluginWapper* >( ext, inSusiePlugin ));

			TRACE("Susie Mapping %s \n",ext.c_str());

			findstart = hitpos + 1;
		}
	}
	while(extes != "");
}
