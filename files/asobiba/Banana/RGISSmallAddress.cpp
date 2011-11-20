// RGISSmallAddress.cpp: RGISSmallAddress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallAddress.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RGISSmallAddress::RGISSmallAddress()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallAddress::~RGISSmallAddress()
{
}



void RGISSmallAddress::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;

}


void RGISSmallAddress::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	//�ڍׂ̃��x���� LEVEL_NOT_CHIMEI ��荂����Ε\�����Ȃ�
	if (inDraw->getLevel() >= LEVEL_NOT_BANCHI ) return ;

	SmallAddressDataOptimizMainData mainD;
	SmallAddressDataOptimizUniqData d;
	mainD.LoopCount = 0;

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

		inDraw->Point(inBlockStart, &d.xy , 1 , RGB24TOBGB16(RGB(255,255,255)) , d.Banchi );

		ASSERT(mainD.NameIndex < inName->getCount() );
	}
}


//�Z������.
void RGISSmallAddress::FindAddress(const LPOINT *inBlockStart ,FindPool * outFindData , 
	const string &inFindName , unsigned short inFindBanchi, const RGISSmallNameDatabase * inNameDB ) const
{
	SmallAddressDataOptimizMainData mainD;
	SmallAddressDataOptimizUniqData d;
	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	if ( inFindName.size() != 0)
	{
		//�ʏ팟��
		for(int n = 0; n < DataCount ; n++)
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

			const char * fullname = inNameDB->getPoint(mainD.NameIndex);
			if (strncmp( fullname , inFindName.c_str() , inFindName.size() ) != 0)
			{
				continue;	//��v���Ȃ�
			}
			if (inFindBanchi != 0 && inFindBanchi != d.Banchi)
			{
				continue;	//��v���Ȃ�
			}

			LPOINT point ;
			point.x = inBlockStart->x + d.xy.x ;
			point.y = inBlockStart->y + d.xy.y ;

			//���ꂪ�T���Ă����m�[�h��!
			outFindData->push_back(new RGISSmallFindObject(&point ,string("") + fullname + num2str(d.Banchi) + "��") );
		}
	}
	else
	{
		//���ڂ��ȗ�����Ă���̂ŁA��������q�b�g����̂�������邽�߂Ɋe�����ЂƂÓ���܂��B
		int lastHit = -1;
		for(int n = 0; n < DataCount ; n++)
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

			if (lastHit == mainD.NameIndex)
			{
				continue;	//�������ǉ�����
			}

			lastHit = mainD.NameIndex;

			LPOINT point ;
			point.x = inBlockStart->x + d.xy.x ;
			point.y = inBlockStart->y + d.xy.y ;

			const char * fullname = inNameDB->getPoint(mainD.NameIndex);
			outFindData->push_back(new RGISSmallFindObject(&point ,string("") + fullname ) );
		}
	}
}

//�f�[�^�̓ǂݍ���.
void RGISSmallAddress::readDataMain(RStdioMemory * ioFile, SmallAddressDataOptimizMainData * outData) const
{
	outData->Code = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);


	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}
//�f�[�^�̓ǂݍ���.
void RGISSmallAddress::readDataUniq(RStdioMemory * ioFile, SmallAddressDataOptimizUniqData * outData) const
{
	outData->Banchi = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->xy.x = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
	outData->xy.y = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
}
