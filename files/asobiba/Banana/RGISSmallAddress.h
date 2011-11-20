// RGISSmallAddress.h: RGISSmallAddress �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLADDRESS_H__1CA470C8_B340_4A24_AC6D_F143D41A310D__INCLUDED_)
#define AFX_RGISSMALLADDRESS_H__1CA470C8_B340_4A24_AC6D_F143D41A310D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"
#include "RGISSmallNameDatabase.h"
#include "RGISSmallFindObject.h"

struct SmallAddressDataOptimizMainData
{
	unsigned short			Code;			//01010 �̂悤�Ȓ��R�[�h.
	unsigned short			NameIndex;
	unsigned char			LoopCount;
};
struct SmallAddressDataOptimizUniqData
{
	unsigned short			Banchi;			//�n��

	SPOINT					xy;
};


class RGISSmallAddress  
{
public:
	RGISSmallAddress();
	virtual ~RGISSmallAddress();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* Buffer) throw(RException);
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);
	//�Z������.
	void FindAddress(const LPOINT *inBlockStart ,FindPool * outFindData , 
		const string &inFindName , unsigned short inFindBanchi, const RGISSmallNameDatabase * inNameDB ) const;
private:

	//�f�[�^�̓ǂݍ���.
	void RGISSmallAddress::readDataMain(RStdioMemory * ioFile, SmallAddressDataOptimizMainData * outData) const;
	//�f�[�^�̓ǂݍ���.
	void RGISSmallAddress::readDataUniq(RStdioMemory * ioFile, SmallAddressDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLADDRESS_H__1CA470C8_B340_4A24_AC6D_F143D41A310D__INCLUDED_)
