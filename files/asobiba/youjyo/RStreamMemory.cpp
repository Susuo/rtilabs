// RStreamMemory.cpp: RStreamMemory �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RStreamMemory.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStreamMemory::RStreamMemory(char *ioMem,int inSize)
{
	Mem = ioMem;
	Size = inSize;
	NowReadSize = 0;
}

RStreamMemory::~RStreamMemory()
{

}

int RStreamMemory::Write(const char *inBuffer ,int inBufferSize) 
{
	return 0;
}

int RStreamMemory::Read(char *outBuffer ,int inBufferSize) 
{
	return 0;
}

