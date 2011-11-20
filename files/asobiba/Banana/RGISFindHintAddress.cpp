// RGISFindHintAddress.cpp: RGISFindHintAddress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISFindHintAddress.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISFindHintAddress::RGISFindHintAddress()
{

}

RGISFindHintAddress::~RGISFindHintAddress()
{
	Delete();
}

void RGISFindHintAddress::Create(unsigned long inBlock,unsigned long inCode)
{
	UniqAddress::iterator i = Address.begin();
	UniqAddress::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		if ( (*i)->Code == inCode && (*i)->Block == inBlock) return ;
	}
	//������Ȃ������̂Œǉ�.
	FindHintAddressData * p = new FindHintAddressData;
	p->Code = inCode;
	p->Block = inBlock;
	Address.push_back( p );
}

void RGISFindHintAddress::Write(const string & inDir) throw (RException)
{
	RStdioFile file;

	file.Open(inDir + "addressHint" , "a+b");
	UniqAddress::iterator i = Address.begin();
	UniqAddress::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		file.fwrite( &((*i)->Code) , sizeof(unsigned long) );
		file.fwrite( &((*i)->Block) , sizeof(unsigned long) );
	}
}

void RGISFindHintAddress::Delete()
{
	UniqAddress::iterator i = Address.begin();
	UniqAddress::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		delete (*i);
	}
	Address.clear();
}

JoinHeadFormatScript RGISFindHintAddress::getPackInfo() 
{
	JoinHeadFormatScript ret;
	ret.TempFilename = "addressHint";
	ret.BlockStartSimbol = 'A';
	ret.DataSize = sizeof(unsigned long) + sizeof(unsigned long);	//4 + 4 = 8
	ret.HeadType = HeadTypeFixed;

	return ret;
}

