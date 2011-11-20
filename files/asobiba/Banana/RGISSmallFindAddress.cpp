// RGISSmallFindAddress.cpp: RGISSmallFindAddress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallFindAddress.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallFindAddress::RGISSmallFindAddress()
{

}

RGISSmallFindAddress::~RGISSmallFindAddress()
{
	Delete();
}


void RGISSmallFindAddress::Create(unsigned long inCode , const string & inName , unsigned long inBanchi , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	this->Code = inCode;
	this->Name = inName;
	this->Banchi = inBanchi;
	this->Cache = ioCache;
}

void RGISSmallFindAddress::Delete()
{
	FindPool::iterator i = FoundData.begin();
	FindPool::iterator e = FoundData.end();

	for( ; i != e ; i++)
	{
		delete (*i);
	}
	FoundData.clear();
}

//�V���{���̉���
void RGISSmallFindAddress::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	unsigned long code;
	unsigned long block;

	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		memory.fread( &code , sizeof(code) );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &block , sizeof(block) );
//		ASSERT(memory.ftell() <= inDataSize);

		//�T���Ă��钬�ł���?
		if (code != this->Code)
		{
			continue;
		}

		//�T���Ă��钬������悤�Ȃ̂ŁA���̃u���b�N���ڂ����m�肽���B
		const RGISSmallParent* parent = this->Cache->Load(block);
		if (parent == NULL)
		{
			continue;
		}


		//������x�i�荞�߂��̂ŁA���ۂɌ������߂��o���Ă݂悤�B
		parent->FindAddress(&FoundData , Name , Banchi);

	}

//	ASSERT(!(memory.ftell() <= inDataSize));
}

