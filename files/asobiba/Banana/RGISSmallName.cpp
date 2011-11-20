// RGISSmallName.cpp: RGISSmallName �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallName.h"

#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallName::RGISSmallName()
{

}

RGISSmallName::~RGISSmallName()
{

}


void RGISSmallName::Creae(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	char matchListBuffer[1024];
	
	Chain.clear();

	int type;
	char spaceBuffer[1024];

	for(int nowLine = 0 ;  !file.IsEof() ; nowLine ++ )
	{
		string line = file.ReadLine();
		//��s�܂��́A�R�����g�͖���.
		if ( line.empty() || line[0] == '#' || line == "\r\n") continue;

		int r = sscanf(line.c_str() , "%d%1024[ ]%s1024", &type , spaceBuffer , matchListBuffer);
		if (r < 1) 
		{
			throw RException(EXCEPTIONTRACE + inFilename + " �t�@�C����" + num2str(nowLine) + "�s�ڂ̓ǂݍ��݂Ɏ��s���܂���");
		}

		if (type < 0 || type > 256)
		{
			throw RException(EXCEPTIONTRACE + inFilename + " �t�@�C����" + num2str(nowLine) + "�s�ځA��ނ͈̔͂� 0 �` 255 �܂łł�");
		}

		//�������Ȃ��ꍇ����肪�����Ȃ��悤�ɁB
		if (r != 3)
		{
			matchListBuffer[0] = '\0';
		}
		//�i�[.
		RGISSmallNameRuleChain* p = new RGISSmallNameRuleChain;
		p->Create( (unsigned char) type , matchListBuffer);
		Chain.push_back( p );
	}
}

bool RGISSmallName::Match(const char* inName , RGISSmallNamePair * outPair) const
{
	newVector<RGISSmallNameRuleChain>::const_iterator i = Chain.begin();
	newVector<RGISSmallNameRuleChain>::const_iterator e = Chain.end();

	int nameLen = strlen(inName);

	for( ; i != e ; i++)
	{
		const RStringList* list = (*i)->getMatchList();
		RStringList::const_iterator is = list->begin();
		RStringList::const_iterator es = list->begin();

		for( ; is != es ; is++ )
		{
			//�Ōオ��v���Ă��邩����.
			const char * p = inName + nameLen - is->size();
			if ( strcmp( is->c_str() , p ) == 0 )
			{
				//��v���Ă���.
				//��ނ�\��������؂�̂Ă�.

				//ex.
				//�ƂƂ�a�@	��	�ƂƂ�
				//
				outPair->Name = string(p , 0  , nameLen - is->size() );
				outPair->Type = (*i)->getType();

				return true;
			}
		}
	}
	return false;
}

