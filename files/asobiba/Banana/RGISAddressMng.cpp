// RGISAddressMng.cpp: RGISAddressMng �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddressMng.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISAddressMng::RGISAddressMng()
{

}

RGISAddressMng::~RGISAddressMng()
{

}

void RGISAddressMng::Draw(RGISDraw* ioDraw)
{
	Address.Draw(ioDraw , &NameDataBase);
}



void RGISAddressMng::SmallPack(const string inPath,const string inTo,const string inNumString , int inScale)
{

	//�ǂݍ���
	string filename = inPath + "/address/" + inNumString + "JGD.csv";
	Address.Create(filename   , atoi(inNumString.c_str() ) , &NameDataBase , inScale);

	Address.ConvertDisplayScal(1);

	//��������
	string path = inTo + "/temp/" ;

	Address.SmallPack(path , &NameDataBase , &FindHintAddress);

	FindHintAddress.Write(path);
}

//�����X�N���v�g�̎擾.
int RGISAddressMng::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const
{
	ASSERT(inSize >= 2);
	ioScripts[0] = Address.getPackInfo();
	ioScripts[1] = NameDataBase.getPackInfo("NA" , 'M');
	return 2;
}
//�����X�N���v�g�̎擾.
int RGISAddressMng::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const
{
	ASSERT(inSize >= 1);
	ioHeadScripts[0] = FindHintAddress.getPackInfo();
	return 1;
}
