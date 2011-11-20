// RGISSmallKawa.cpp: RGISSmallKawa �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallKawa.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RGISSmallKawa::RGISSmallKawa()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallKawa::~RGISSmallKawa()
{
}

void RGISSmallKawa::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//��/�_�̕`��
void RGISSmallKawa::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallKawaDataOptimizMainData mainD = { 0 };
	SmallKawaDataOptimizUniqData d;

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
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_KOUKYOU:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			//����͐� �� ��������
			if (0x55 == mainD.SB || 0x56 == mainD.SB)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
		}
	}
}

//������̕`��
void RGISSmallKawa::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallKawaDataOptimizMainData mainD = { 0 };
	SmallKawaDataOptimizUniqData d;

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
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
//			inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			//����͐� �� ��������
//			if (0x55 == mainD.SB || 0x56 == mainD.SB)
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		}
	}
}


COLORREF RGISSmallKawa::selectColor(unsigned char inShubetsu) const
{
	return BBananaConfig::getInstance()->getColor(COLOR_MIZU);
/*
	if (0x55 == inShubetsu)				return COLOR_9;		//����͐�
	if (0x56 == inShubetsu)				return COLOR_9;		//����͐�
	if (0x57 == inShubetsu)				return COLOR_9;		//��������
	if (0x58 == inShubetsu)				return COLOR_9;		//��������
	if (0x59 == inShubetsu)				return COLOR_9;		//�Ώ�������S��

	ASSERT(0);
	return 0;
*/
}

COLORREF RGISSmallKawa::selectSize(unsigned char inShubetsu) const
{
	if (0x55 == inShubetsu)				return 1;		//����͐�
	if (0x56 == inShubetsu)				return 2;		//����͐�
	if (0x57 == inShubetsu)				return 1;		//��������
	if (0x58 == inShubetsu)				return 2;		//��������
	if (0x59 == inShubetsu)				return 2;		//�Ώ�������S��

	ASSERT(0);
	return 0;
}

//�f�[�^�̓ǂݍ���.
void RGISSmallKawa::readDataMain(RStdioMemory * ioFile, SmallKawaDataOptimizMainData * outData) const
{
	outData->SB = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//���O
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}


//�f�[�^�̓ǂݍ���.
void RGISSmallKawa::readDataUniq(RStdioMemory * ioFile, SmallKawaDataOptimizUniqData * outData) const
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
