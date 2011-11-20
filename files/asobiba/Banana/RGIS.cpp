// RGIS.cpp: RGIS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGIS.h"
#include "RStdioFile.h"




//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGIS::RGIS()
{

}

RGIS::~RGIS()
{

}

//Key�𕶎���Ƃ��ĕԂ��܂�. Key�͕K�� 5���̕�����ɂȂ�.
string RGIS::KeyToString(int inKey) const
{
	char buffer[MAX_PATH];
	sprintf(buffer , "%05d" , inKey);

	return buffer;
}

void RGIS::SmallPack(const string inPath,const string inTo,int inKey ,int inScale)
{
	const string keyString = KeyToString(inKey);

	Gis2500.SmallPack(inPath , inTo , keyString ,&Gis2500Info ,&NameDatabase , inScale);
	Gis25000.SmallPack(inPath , inTo , keyString,&Gis2500Info ,&NameDatabase , inScale);
	AddressMng.SmallPack(inPath , inTo , keyString , inScale);



}

//�����X�N���v�g�̎擾.
int RGIS::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) 
{
	RGIS25000		Gis25000;
	RGIS2500		Gis2500;
	RGISAddressMng	AddressMng;

	//�����X�N���v�g�̎擾.
	int segmentCount = Gis25000.getJoinScript(&ioScripts[0] , inSize);
	segmentCount += Gis2500.getJoinScript(&ioScripts[segmentCount] , inSize - segmentCount);
	segmentCount += AddressMng.getJoinScript(&ioScripts[segmentCount] , inSize - segmentCount);
	ioScripts[segmentCount++] = RGISNameDatabase::getPackInfo("NX" , 'N');



	return segmentCount;
}



//�����X�N���v�g�̎擾.
int RGIS::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) 
{
	RGIS25000		Gis25000;
	RGIS2500		Gis2500;
	RGISAddressMng	AddressMng;

	//�����X�N���v�g�̎擾.
	int segmentCount = Gis25000.getJoinHeadScript(&ioHeadScripts[0] , inSize);
	segmentCount += Gis2500.getJoinHeadScript(&ioHeadScripts[segmentCount] , inSize - segmentCount);
	segmentCount += AddressMng.getJoinHeadScript(&ioHeadScripts[segmentCount] , inSize - segmentCount);

	return segmentCount;
}
