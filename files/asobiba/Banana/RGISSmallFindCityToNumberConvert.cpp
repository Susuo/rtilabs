// RGISSmallFindCityToNumberConvert.cpp: RGISSmallFindCityToNumberConvert �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFindCityToNumberConvert.h"
#include "RStdioMemory.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallFindCityToNumberConvert::RGISSmallFindCityToNumberConvert()
{
	this->Table = NULL;
	this->TableCount = 0;

}

RGISSmallFindCityToNumberConvert::~RGISSmallFindCityToNumberConvert()
{
	Delete();
}


void RGISSmallFindCityToNumberConvert::Create() throw(RException) 
{
	Delete();
}

void RGISSmallFindCityToNumberConvert::Delete()
{
	delete [] this->Table;
	this->Table = NULL;
}

//�V���{���̉���
void RGISSmallFindCityToNumberConvert::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	ASSERT(this->Table == NULL);

	this->Table = new AddressToCodeMatchingTable[inDataCount];
	this->TableCount = inDataCount;

	unsigned long block;


	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		//�ŏ��Ƀu���b�N.
		memory.fread( &block , sizeof(block) );
		ASSERT(memory.ftell() <= inDataSize);

		//���ɖ��O ����� \0 �ŏI���.
		const char * cityName = memory.ptr();
		int cityNameLen = strlen(cityName) + 1;
		//���O���ǂݔ�΂�.
		memory.fseek( cityNameLen , SEEK_CUR);

		this->Table[i].Address = cityName;
		this->Table[i].Code = block;
	}
}


