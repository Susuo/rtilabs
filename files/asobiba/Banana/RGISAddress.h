// RGISAddress.h: RGISAddress �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_)
#define AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISDraw.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"
#include "RGISFindHintAddress.h"

struct AddressData
{
//	string	Todoufuken;		//�s���{����
//	string	City;			//�s�撬����
	string	Machi;			//�厚�E�����ږ�
	unsigned short NameIndex;
	int		Banchi;			//�n��
	int		Code;			//01010 �̂悤�Ȓ��R�[�h.
	UPOINT	xy;				//���W(UTM)
	bool	Use;
};


class RGISAddress  
{
public:
	RGISAddress();
	virtual ~RGISAddress();
	void Create(const string & inFilename,int inCode , RGISNameDatabase * ioNameDatabase, int inScale) throw(RException);
	void Delete();

	//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
	void ConvertDisplayScal(int inShift) throw(RException);

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const AddressData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw ,const RGISNameDatabase * ioNameDatabase) const;


	void SmallPack(const string & inDir, const RGISNameDatabase* inNameData, RGISFindHintAddress * ioFindHintAddress)  const throw(RException);

	JoinTempFormatScript getPackInfo() const;

	static void test();
private:
	// " " �ł������Ă���P��̃N�N�����͂����܂��B
	const string RGISAddress::noQoute(const string inStr);

private:
	unsigned int				Count;
	AddressData*	Points;

};

#endif // !defined(AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_)
