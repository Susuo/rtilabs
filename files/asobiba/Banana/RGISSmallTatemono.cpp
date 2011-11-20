// RGISSmallTatemono.cpp: RGISSmallTatemono �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallTatemono.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RGISSmallTatemono::RGISSmallTatemono()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallTatemono::~RGISSmallTatemono()
{
}


void RGISSmallTatemono::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//��/�_�̕`��
void RGISSmallTatemono::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallTatemonoDataOptimizMainData mainD;
	SmallTatemonoDataOptimizUniqData d;

	mainD.LoopCount = 0;

	ParticularLevel level = inDraw->getLevel();

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , 1 , 8 );
		case LEVEL_NOT_MIDDLELEVEL:
			break;
		}
	}
}

//������̕`��
void RGISSmallTatemono::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallTatemonoDataOptimizMainData mainD;
	SmallTatemonoDataOptimizUniqData d;

	mainD.LoopCount = 0;

	ParticularLevel level = inDraw->getLevel();
	RGISSmallDupKill dupKill;

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			break;
		}

	}
}


//�f�[�^�̓ǂݍ���.
void RGISSmallTatemono::readDataMain(RStdioMemory * ioFile, SmallTatemonoDataOptimizMainData * outData) const
{
	//���O
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}

//�f�[�^�̓ǂݍ���.
void RGISSmallTatemono::readDataUniq(RStdioMemory * ioFile, SmallTatemonoDataOptimizUniqData * outData) const
{

	//CV�f�[�^(�_)�̎擾
	outData->CVCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//�_�f�[�^�̓ǂݔ�΂�
	outData->CV = ioFile->ptr();
	ioFile->fseek(  sizeof(unsigned short) * 2, SEEK_CUR );
	ioFile->fseek(  (outData->CVCount - 1) *  sizeof(unsigned char) * 2, SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);
}
