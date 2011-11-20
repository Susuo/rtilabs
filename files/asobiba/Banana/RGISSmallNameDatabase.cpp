// RGISSmallNameDatabase.cpp: RGISSmallNameDatabase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallNameDatabase.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RGISSmallNameDatabase::RGISSmallNameDatabase()
{
	this->Data = NULL;
	this->DataSize  = 0;
	this->DataCount  = 0;
}

RGISSmallNameDatabase::~RGISSmallNameDatabase()
{
	Delete();
}

void RGISSmallNameDatabase::Delete()
{
	delete [] this->Data;
	this->Data = NULL;
	this->DataSize  = 0;
	this->DataCount  = 0;
}


void RGISSmallNameDatabase::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	ASSERT(this->Data == NULL);

	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = new const char*[this->DataCount + 1];

	//inBuffer �ɂ́A������ \0 ������ŋL�^����Ă���̂ŁA
	//���Ԗڂ̕������A�ǂ�����n�܂����̂��������L�^���Ă����܂��B
	const char* p = inBuffer;
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		this->Data[i] = p;

		p = p + strlen(p) + 1;
	}
}


