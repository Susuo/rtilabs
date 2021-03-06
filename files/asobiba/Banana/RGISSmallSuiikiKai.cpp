// RGISSmallSuiikiKai.cpp: RGISSmallSuiikiKai クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallSuiikiKai.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISSmallSuiikiKai::RGISSmallSuiikiKai()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallSuiikiKai::~RGISSmallSuiikiKai()
{
}

void RGISSmallSuiikiKai::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}
/*
	this->Count = inDataCount;
	this->Points = new SmallSuiikiKaiData[this->Count + 1];

	RStdioMemory memory;
	memory.Open(ioBuffer , inDataSize);
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		SmallSuiikiKaiData* p = &this->Points[i];
		memory.fread( &p->SR , sizeof(unsigned char) );
		ASSERT(!memory.IsEof());

		p->CV.Create(&memory,inBlock);
	}

	ASSERT( memory.IsEof() );
}
*/

//線/点の描画と塗りつぶし
void RGISSmallSuiikiKai::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallSuiikiKaiDataOptimizMainData mainD;
	SmallSuiikiKaiDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	ParticularLevel level = inDraw->getLevel();
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

		inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,  selectSize(mainD.SR) , selectColor(mainD.SR) );
	}

}

//塗りつぶし
void RGISSmallSuiikiKai::Draw2(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallSuiikiKaiDataOptimizMainData mainD;
	SmallSuiikiKaiDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	ParticularLevel level = inDraw->getLevel();
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

//		inDraw->Paint(inBlockStart, d.CV , d.CVCount , selectColor(mainD.SR) );
	}

}




COLORREF RGISSmallSuiikiKai::selectColor(unsigned char inShubetsu) const
{
	return BBananaConfig::getInstance()->getColor(COLOR_MIZU);
/*
	if (0x50 == inShubetsu)				return COLOR_9;		//水涯線かれ川
	if (0x51 == inShubetsu)				return COLOR_9;		//水涯線または湖岸線
	if (0x52 == inShubetsu)				return COLOR_9;		//海岸線
	if (0x53 == inShubetsu)				return COLOR_9;		//河口
	if (0x54 == inShubetsu)				return COLOR_9;		//湖沼と河川の境界

	ASSERT(0);
	return 0;
*/
}

COLORREF RGISSmallSuiikiKai::selectSize(unsigned char inShubetsu) const
{
	if (0x50 == inShubetsu)				return 1;		//水涯線かれ川
	if (0x51 == inShubetsu)				return 1;		//水涯線または湖岸線
	if (0x52 == inShubetsu)				return 1;		//海岸線
	if (0x53 == inShubetsu)				return 1;		//河口
	if (0x54 == inShubetsu)				return 1;		//湖沼と河川の境界

	ASSERT(0);
	return 0;
}

//データの読み込み.
void RGISSmallSuiikiKai::readDataMain(RStdioMemory * ioFile, SmallSuiikiKaiDataOptimizMainData * outData) const
{
	outData->SR = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

}

//データの読み込み.
void RGISSmallSuiikiKai::readDataUniq(RStdioMemory * ioFile, SmallSuiikiKaiDataOptimizUniqData * outData) const
{
	//CVデータ(点)の取得
	outData->CVCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//点データの読み飛ばし
	outData->CV = ioFile->ptr();
	ioFile->fseek(  sizeof(unsigned short) * 2, SEEK_CUR );
	ioFile->fseek(  (outData->CVCount - 1) *  sizeof(unsigned char) * 2, SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);

}