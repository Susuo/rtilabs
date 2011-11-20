// BAddressMatching.h: BAddressMatching �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_)
#define AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOException.h"
#include "RGISSmallFindCityToNumberConvert.h"


class BAddressParse
{
public:
	BAddressParse()
	{
	}
	virtual ~BAddressParse()
	{
	}
	bool BAddressParse::Parse(const string &inAddress ,const  AddressToCodeMatchingTable* inMatchTable , int inTableSize);

	int getCode() const
	{
		return Code;
	}
	string getMachiCyoume() const
	{
		return MachiCyoume;
	}
	int getBanchi() const
	{
		return Banchi;
	}

	static void test();

private:
	//���ڂƂ����w�������m�ɂ���ꍇ...
	const char * BAddressParse::getChyoume(const char * inStr,const string inPrefix , int *outSkip) const;
	const char * BAddressParse::getChyoume(const char * inStr, int *outSkip) const;
	const char * BAddressParse::FindNot2Byte(const char *inStr) const;
	//�s���{������������ǂݔ�΂�.
	int BAddressParse::SkipTodoufuKen(const string & inAddress) const;

	const AddressToCodeMatchingTable* BAddressParse::ToCode(const string & inAddress , const  AddressToCodeMatchingTable* inMatchTable , int inTableSize) const;

private:
	int Code;				//01101 �̂悤�ȃR�[�h
	string MachiCyoume;		//��������������
	int	Banchi;				//�~��
};

#endif // !defined(AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_)
