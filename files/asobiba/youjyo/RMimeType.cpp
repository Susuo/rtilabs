// RMimeType.cpp: RMimeType �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RMimeType.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMimeType::RMimeType()
{

}

RMimeType::~RMimeType()
{

}

void RMimeType::Load(const string inMimeFilname)
{
	FILE * fp = fopen( inMimeFilname.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}
	char buf[MAX_PATH];

	while( !feof(fp) )
	{
		if ( fgets(buf , MAX_PATH , fp) == NULL ) break;

		if ( buf[0] == '\0' || buf[0] == '#' ) continue;

		const char* next = nextSpace( buf );
		if (next == NULL) continue;

		//���ꂪ mime �^�C�v
		const string type = RConv::chop( string(buf , 0 , next - buf) );

		const char* extStart = nextNonSpace( next );
		if (extStart == NULL) continue;

		while(1)
		{
			const char* extEnd = nextSpace( extStart );
			if (extEnd == NULL)
			{	//�ЂƂ����Ȃ��ꍇ
				const string ext = RConv::chop(extStart);
				MimeType[ ext ] = type;
				break;
			}

			//�ŁA���ꂪ�t�@�C���g���q�ł��B
			const string ext = RConv::chop( string(extStart , 0 , extEnd - extStart) );
//			MimeType[ RAlphabetHash( ext.c_str() ) ] = type;
			MimeType[ ext ] = type;
			
			extStart = nextNonSpace( extEnd );
			if (extStart == NULL) break;
		}
	}
	fclose(fp);
}

//���̋�؂�܂œǂݔ�΂�.
const char* RMimeType::nextSpace(const char* inStr) const
{
	while(*inStr != '\0')
	{
		if ( *inStr == ' ' || *inStr == '\t' ) return inStr;
		++inStr;
	}

	return NULL;
}


//���̋�؂�ł͂Ȃ������܂œǂݔ�΂�.
const char* RMimeType::nextNonSpace(const char* inStr) const
{
	while(*inStr != '\0')
	{
		if ( *inStr != ' ' && *inStr != '\t' ) return inStr;
		++inStr;
	}

	return NULL;
}

void RMimeType::test()
{
	{
		RMimeType	rmt;
		rmt.Load("conftest/mime_small.types");

		string ret;

		ASSERT( (ret = rmt.getMimeType("jpeg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("jpg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("png") ) == "image/png" );
	}
	{
		RMimeType	rmt;
		rmt.Load("conftest/mime.types");

		string ret;

		ASSERT( (ret = rmt.getMimeType("jpeg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("jpg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("png") ) == "image/png" );
	}
}

RMimeType* RMimeType::getInstance()
{
	static RMimeType p;
	return &p;
}

