// RHTTPHeaderAttributeDate.cpp: RHTTPHeaderAttributeDate �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RHTTPHeaderAttributeDate.h"
#include "RDateConv.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RHTTPHeaderAttributeDate::RHTTPHeaderAttributeDate()
{
	this->Date = -1;
	this->Enabled = false;
}

RHTTPHeaderAttributeDate::~RHTTPHeaderAttributeDate()
{

}

RHTTPHeaderAttributeDate& RHTTPHeaderAttributeDate::operator=(const char* inHeader)
{
	this->Date = RDateConv::HttpTimeToUnixTime( inHeader );
	if (this->Date != -1)
	{
		this->Enabled = true;
	}
	return (*this);
}

