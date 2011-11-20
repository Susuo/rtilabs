// RGISSmallTetudo.cpp: RGISSmallTetudo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallTetudo.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallTetudo::RGISSmallTetudo()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallTetudo::~RGISSmallTetudo()
{
}


void RGISSmallTetudo::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//��/�_�̕`��
void RGISSmallTetudo::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

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
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_KOUKYOU:
			//�w�������͕\�����Ă�����.
			if (mainD.EkiNameIndex == 0xffff)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			else
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_LOWLEVEL:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//���ʓS��JR�ƕ��ʓS��
			if (0x43 == mainD.SB || 0x44 == mainD.SB)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		}
	}
}

//������̕`��
void RGISSmallTetudo::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

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

		const char * name ;
		if (mainD.EkiNameIndex != 0xffff)	name = inName->getPoint(mainD.EkiNameIndex);
		else							name = inName->getPoint(mainD.NameIndex);

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , name , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
			//�w�������͕\�����Ă�����.
			if (mainD.EkiNameIndex != 0xffff)
				inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,  name , &dupKill);
//			else
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		case LEVEL_NOT_LOWLEVEL:
//			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//���ʓS��JR�ƕ��ʓS��
//			if (0x43 == mainD.SB || 0x44 == mainD.SB)
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		}

		
	}
}

//�w����.
void RGISSmallTetudo::FindEki(const LPOINT *inBlockStart , FindPool * outFindData , 
							  const string &inFindName , unsigned short inFindNameHash,
							  const RGISSmallNameDatabase * inNameDB ) const
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	for(int n = 0; n < this->DataCount ; n++)
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

		//���O�� index �͓������ł���?
		if (mainD.EkiNameIndex != inFindNameHash)
		{
			continue;	//��v���Ȃ�
		}
		//���O���擪��v���܂���?
		const char * fullname = inNameDB->getPoint(mainD.EkiNameIndex);
		if ( strncmp( fullname , inFindName.c_str() , inFindName.size() ) != 0)
		{
			continue;	//��v���Ȃ�
		}

		LPOINT avgPoint = RGISSmallType::AvgCV(inBlockStart , d.CV , d.CVCount ); 
		//���ꂪ�T���Ă����m�[�h��!
		const char * lineName = inNameDB->getPoint(mainD.NameIndex);
		if (lineName != NULL && lineName[0] != '\0')
		{
			outFindData->push_back(new RGISSmallFindObject(&avgPoint ,  string(fullname) + "(" + lineName +")") );
		}
		else
		{
			outFindData->push_back(new RGISSmallFindObject(&avgPoint ,  fullname ) );
		}
	}
}



COLORREF16 RGISSmallTetudo::selectColor(unsigned char inShubetsu , unsigned short inER) const
{
	if (inER != 0xffff)				return BBananaConfig::getInstance()->getColor(COLOR_EKI);		//�w

	return BBananaConfig::getInstance()->getColor(inShubetsu - 0x43 + COLOR_TETUDO_JR);
/*
	if (inER != 0xffff)				return COLOR_6;		//�w

	if (0x43 == inShubetsu)				return COLOR_6;		//���ʓS��JR
	if (0x44 == inShubetsu)				return COLOR_6;		//���ʓS��
	if (0x45 == inShubetsu)				return COLOR_7;		//�H�ʓd��
	if (0x46 == inShubetsu)				return COLOR_7;		//�n�����S��
	if (0x47 == inShubetsu)				return COLOR_6;		//���̑�

	ASSERT(0);
	return 0;
*/
}

COLORREF RGISSmallTetudo::selectSize(unsigned char inShubetsu , unsigned short inER) const
{
	if (inER != 0xffff)				return 8;		//�w

	if (0x43 == inShubetsu)				return 4;		//���ʓS��JR
	if (0x44 == inShubetsu)				return 4;		//���ʓS��
	if (0x45 == inShubetsu)				return 1;		//�H�ʓd��
	if (0x46 == inShubetsu)				return 1;		//�n�����S��
	if (0x47 == inShubetsu)				return 1;		//���̑�

	ASSERT(0);
	return 0;
}

//�f�[�^�̓ǂݍ���.
void RGISSmallTetudo::readDataMain(RStdioMemory * ioFile, SmallTetudoDataOptimizMainData * outData) const
{
	outData->SB = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//���O
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	//�w�̖��O
	outData->EkiNameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}

//�f�[�^�̓ǂݍ���.
void RGISSmallTetudo::readDataUniq(RStdioMemory * ioFile, SmallTetudoDataOptimizUniqData * outData) const
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
