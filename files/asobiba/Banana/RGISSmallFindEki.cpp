// RGISSmallFindEki.cpp: RGISSmallFindEki �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFindEki.h"
#include "RStdioMemory.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallFindEki::RGISSmallFindEki()
{

}

RGISSmallFindEki::~RGISSmallFindEki()
{
	Delete();
}


void RGISSmallFindEki::Create(const string & inName , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	if (inName.size() >= NAME_SHORT_CUT_SIZE) 
	{
		memcpy(NameShortCut , inName.c_str() , NAME_SHORT_CUT_SIZE);
		this->NameSize = NAME_SHORT_CUT_SIZE;
	}
	else
	{
		memset(NameShortCut , 0 , NAME_SHORT_CUT_SIZE );
		memcpy(NameShortCut , inName.c_str() , inName.size() );
		this->NameSize = inName.size() ;
	}

	this->Name = inName;
	this->Cache = ioCache;
}

void RGISSmallFindEki::Delete()
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
void RGISSmallFindEki::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	char name[NAME_SHORT_CUT_SIZE];
	unsigned long block;
	unsigned short nameIndex;


	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		memory.fread( name , sizeof(unsigned char) * NAME_SHORT_CUT_SIZE );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &block , sizeof(block) );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &nameIndex , sizeof(nameIndex) );
//		ASSERT(memory.ftell() <= inDataSize);

		//���O�͐擪��v�Ō���.
		if ( memcmp( name , this->NameShortCut , this->NameSize) != 0 )
		{
			continue;
		}
		//���O������悤�Ȃ̂ŁA���̃u���b�N���ڂ����m�肽���B
		const RGISSmallParent* parent = this->Cache->Load(block);
		if (parent == NULL)
		{
			continue;
		}
		//������x�i�荞�߂��̂ŁA���ۂɌ������߂��o���Ă݂悤�B
		parent->FindEki(&FoundData , Name , nameIndex);
	}

//	ASSERT(!(memory.ftell() <= inDataSize));
}


