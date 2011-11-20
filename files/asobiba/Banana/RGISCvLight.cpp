// RGISCvLight.cpp: RGISCvLight �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISCvLight.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISCvLight::RGISCvLight()
{

}

RGISCvLight::~RGISCvLight()
{

}

//���̃m�[�h�S�Ɏ����͊܂܂�Ă��܂���?
bool RGISCvLight::isNode(	const RGISNode * inNode , const RGISNodeIntArray * inNodeArray ,
							const RGISSlp * inSlp , const UPOINT srcPoint ) const
{
	int nodeArrayCount = inNodeArray->getCount();
	for(int n = 0 ; n < nodeArrayCount ; n++)
	{
		int nodeAS = inNodeArray->getPoint(n) ;
		int pt = inNode->getPoint( nodeAS - 1)->PT;
		UPOINT destPoint = inSlp->getPoint(pt  - 1 );

		//�m�[�h�ɋL�ڂ���Ă����n�_?
		if (srcPoint.x == destPoint.x && srcPoint.y == destPoint.y)
		{
			return true;
		}
	}
	return false;
}

bool RGISCvLight::DoLightAndNode(const RGISNode * inNode , const RGISNodeIntArray * inNodeArray , 
								 const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const
{
return false;
	//2�_�ȏ�͕K�v.
	if (inSrc->getCount() < 2) return false;

	int count = inSrc->getCount();

	unsigned long * enablePoint = new unsigned long[count];
	int enableCount = 0;
	enablePoint[enableCount++] = inSrc->getPoint(0);	//�擪�͏ȗ��s��.

	int start = 0;
	for(int i = 1 ; i < count - 1 ; i++)
	{
		int t1 = inSrc->getPoint(i) - 1;
		int t2 = inSrc->getPoint(start) - 1;
		int t3 = inSrc->getPoint(i + 1) - 1;
		UPOINT target = inSlp->getPoint(inSrc->getPoint(i) - 1);
		if ( !isNode(inNode , inNodeArray , inSlp , target) &&
			 IsLineOn(	inSlp->getPoint(inSrc->getPoint(start) - 1) ,
						target ,
						inSlp->getPoint(inSrc->getPoint(i + 1) - 1) 
						) ) 
		{
			//���̓_�́A�ȗ��\.
			TRACE("�ȗ����܂� %d\n" , i);
		}
		else
		{
			//���̓_�́A�ȗ��s�\.
			enablePoint[enableCount++] = inSrc->getPoint(i);
			start = i;
		}
	}
	//�I�[�͏ȗ��s��
	enablePoint[enableCount++] = inSrc->getPoint(count - 1);	//�擪�͏ȗ��s��.

	//�ȗ��ł��Ȃ��ꍇ
	if (enableCount == count )
	{
		delete [] enablePoint;
		return false;
	}

	//�ȗ��ł���ꍇ

	//�V�K�ɍ�蒼��.
	outDest->Create(enablePoint , enableCount);

	delete [] enablePoint;

	return true;
}

bool RGISCvLight::DoLight(const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const
{
return false;
	//2�_�ȏ�͕K�v.
	if (inSrc->getCount() < 2) return false;

	int count = inSrc->getCount();

	unsigned long * enablePoint = new unsigned long[count];
	int enableCount = 0;
	enablePoint[enableCount++] = inSrc->getPoint(0);	//�擪�͏ȗ��s��.

	int start = 0;
	for(int i = 1 ; i < count - 1 ; i++)
	{
		if ( IsLineOn(	inSlp->getPoint(inSrc->getPoint(start) - 1) ,
						inSlp->getPoint(inSrc->getPoint(i) - 1) ,
						inSlp->getPoint(inSrc->getPoint(i + 1) - 1) 
						) ) 
		{
			//���̓_�́A�ȗ��\.
			TRACE("�ȗ����܂� %d\n" , i);
		}
		else
		{
			//���̓_�́A�ȗ��s�\.
			enablePoint[enableCount++] = inSrc->getPoint(i);
			start = i;
		}
	}
	//�I�[�͏ȗ��s��
	enablePoint[enableCount++] = inSrc->getPoint(count - 1);	//�擪�͏ȗ��s��.

	//�ȗ��ł��Ȃ��ꍇ
	if (enableCount == count )
	{
		delete [] enablePoint;
		return false;
	}

	//�ȗ��ł���ꍇ

	//�V�K�ɍ�蒼��.
	outDest->Create(enablePoint , enableCount);

	delete [] enablePoint;

	return true;
}

//inSrc �� inDest ��2�_��ʂ钼����� inTarget ������܂���?
bool RGISCvLight::IsLineOn(const UPOINT  inSrc , const UPOINT  inTarget , const UPOINT inDest ) const
{
	if (inSrc.y == inDest.y)
	{
		if (inSrc.y == inTarget.y)
		{
			return true;
		}
	}
	else if (inSrc.x == inDest.x)
	{
		if (inSrc.x == inTarget.x)
		{
			return true;
		}
	}
	else
	{
		double x = (inTarget.y - inSrc.y) / ((double)(inDest.y - inSrc.y) / (inDest.x - inSrc.x))  + inSrc.x;
		if (inTarget.x - 3 >= x && inTarget.x + 3 <= x)	//3�͓K���ȋ��e�͈�.
		{
			return true;
		}
	}
	return false;
}
