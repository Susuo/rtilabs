// RGISCVCut.h: RGISCVCut �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_)
#define AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS.h"
#include "RGISSlp.h"
#include "RStdioFile.h"
#include "GISType.h"
#include "RGIS2500IntArray.h"
#include "RGISCVCutDataIml.h"
#include "RMmap.h"


class RGISCVCut  
{
	struct PointSet
	{
		UPOINT po;
		unsigned long lastBlock;
	};
public:
	RGISCVCut();
	virtual ~RGISCVCut();

	//��������
	void Write(const string & inDir , const RGISSlp * inSlp,
							const GISINTArray* inArray ,RGISCVCutDataIml * ioData);
	//��������
	void RGISCVCut::Write(const string & inDir , 
						  const RGIS2500PointArray* inArray ,RGISCVCutDataIml * ioData);
	//�_�̏�������
	void Write(const string & inDir ,const UPOINT * inPoint,RGISCVCutDataIml * ioData);
	//�u���b�N�ɕ��f
	static unsigned long selectBlock(const UPOINT * inPT) ;

	static void RGISCVCut::test();
	static UPOINT* DebugRead(const string & inDir ,unsigned short inBlock ,const string & inData  );


	void RGISCVCut::Test2(const string & inDir ,unsigned long block , int inMeshKankaku);
private:
	void WriteMain(const string & inDir ,
		 PointSet* p ,unsigned int trueCVcount , RGISCVCutDataIml * ioData);
	//�_�̏�������
	void PointWrite(RStdioFile *iofile , const UPOINT * inPtArray ,unsigned int inCount);
	//Src ���� Dest �֐����Ƃ��ɋ��E���ɂԂ���|�C���g�̎擾.
	UPOINT getMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const;
	//�ׂ荇���u���b�N��?
	bool RGISCVCut::isTexanRude(unsigned long inA , unsigned long  inB) const;
	//Src ���� Dest �֐����Ƃ��ɋ��E���̎��ɂ���u���b�N�̎擾
	UPOINT RGISCVCut::getTexanRudeMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const;
	//-127 �` 127 �͈̔͂Ɏ��܂�悤�ɕ⊮.
	UPOINT RGISCVCut::LineHoseiChar(const UPOINT * inSrc , const UPOINT * inDest );
	//�����u���b�N�̒��őO�オ char �͈̔͂Ɏ��܂�悤�ɂ���ɕ���
	void PointWrite256(RStdioFile *iofile , unsigned long inBlock , const UPOINT * inPtArray ,unsigned int inCount, RGISCVCutDataIml * ioData);
};

class RGISCVCutMesh
{
public:

	RGISCVCutMesh();
	~RGISCVCutMesh();

	//�_�̏�������
	void Write(const string & inDir ,const UPOINT & inPoint,unsigned char inLevel);
private:
	RMmap				Mem;
	unsigned long		LastBlock;

};

#endif // !defined(AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_)
