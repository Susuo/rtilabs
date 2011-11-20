// RGISAddressToCode.cpp: RGISAddressToCode �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddressToCode.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISAddressToCode::RGISAddressToCode()
{

}

RGISAddressToCode::~RGISAddressToCode()
{
	Delete();
}

void RGISAddressToCode::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISAddressToCode::Create(const string & inCodeFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inCodeFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	this->Count = RFileUtil::LineCount(inCodeFilename);
	if (this->Count <= 0) return ;

	//�v�f�̑����̎擾.
	this->Points = new AdressConvertData[this->Count];

	char TodoufukenBuffer[1024];
	char CityBuffer[1024];
	int Number;
	for(int i = 0  ; !file.IsEof(); i++ )
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;
		//�ǂݍ���.
		AdressConvertData* p = &Points[i];
		//
		int rsize = sscanf(buffer.c_str() , "%1024[^,],%1024[^,],%d" , 
			TodoufukenBuffer,CityBuffer,&Number);
		if (rsize != 3) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inCodeFilename.c_str() , rsize , buffer.c_str()  ) );

		p->Todoufuken = TodoufukenBuffer;
		p->City = CityBuffer;
		p->Code = Number;
	}
}

const string RGISAddressToCode::Convert(int inCode) const
{
	for(int i = 0  ; i < this->Count ; i++ )
	{
		const AdressConvertData* p = &Points[i];
		if (p->Code == inCode)	return p->Todoufuken + p->City;
	}
	ASSERT(0);
	return "";
}
