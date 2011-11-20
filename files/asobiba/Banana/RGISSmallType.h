// RGISSmallType.h: RGISSmallType �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_)
#define AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS.h"
#include "RStdioMemory.h"
#include "RGISAllTest.h"





//�m�[�h�̊Ǘ�
class RGISSmallNode
{
	unsigned char	Count;
	unsigned char*	Points;
public:
//	RGISSmallNode();
	~RGISSmallNode();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const unsigned char getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	void Delete();

	static void test();

	static UPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock);
private:

};

//�����_�̊Ǘ�
class RGISSmallType  
{
	unsigned char	Count;
	LPOINT*	Points;
public:
//	RGISSmallType();
	~RGISSmallType();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//�n�_�̌��̎擾.
	unsigned int getCount() const
	{
		return Count;
	}
	//�n�_�̎擾
	const LPOINT* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Delete();

	static void test();

	static LPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock);
	static LPOINT AvgCV(const LPOINT * inBlockStart , const char* inCVData , int inCVCount);
	//�u���b�N�ɕ��f
	static unsigned long selectBlock(const LPOINT * inPT) ;
private:

};

//�P��_�̊Ǘ�.
class RGISSmallPoint
{
	LPOINT	Point;
public:
//	RGISSmallPoint();
	~RGISSmallPoint();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//�n�_�̎擾
	const LPOINT* getPoint() const
	{
		return &Point;
	}

	static void test();
private:
	LPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock) const;

};

#endif // !defined(AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_)
