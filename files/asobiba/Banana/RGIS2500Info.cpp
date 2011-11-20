// RGIS2500Info.cpp: RGIS2500Info �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGIS2500Info.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGIS2500Info::RGIS2500Info(const RGIS2500PointArray* inArray,const string & inName )
{
	//CV�̐��̎擾...
	unsigned short count = inArray->getCount();

	UPOINT sumPoint = {0};
	unsigned short trueCVcount = 0;
	//�Ƃ肠�����A���ׂẴf�[�^���u���b�N���Ƃɐ؂蕪���Ă݂܂��B
	for(int i = 0 ;  i < count ; i++)
	{
		//���x�� /100 �ɂ������ʁA�ׂ荇���_���܂����������ꏊ�ɔz�u����Ă��閳�ʂȌ�������܂��B
		//�����r�����ăf�[�^�T�C�Y�����������܂�.
		UPOINT getpoint= inArray->getPoint(i) ;
		sumPoint.x += getpoint.x;
		sumPoint.y += getpoint.y;
	}

	this->XY.x = (unsigned long) (sumPoint.x / count);
	this->XY.y = (unsigned long) (sumPoint.y / count);

	this->Name = inName;
}


RGIS2500Info::RGIS2500Info(const LPOINT* inPoint,const string & inName )
{
	this->XY = *inPoint;

	this->Name = inName;
}

