// RMouseGestureMousePoints.cpp: RMouseGestureMousePoints �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMouseGestureMousePoints.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMouseGestureMousePoints::RMouseGestureMousePoints()
{
	CompensationDistrance = 10;
}

RMouseGestureMousePoints::~RMouseGestureMousePoints()
{

}

//�O�Ղ�ǉ�
//�ǉ����ꂽ�ꍇ�� true
//�|���V�[�ᔽ�Œǉ�����Ȃ��ꍇ�� false .
bool RMouseGestureMousePoints::Insert(int x,int y)
{
	int size = Points.size();
	if (size >= 1)
	{
		//�O��̒l
		POINT last = Points[size - 1];

		//�O��̒l���� Distrance �s�N�Z������Ă��邱��
		if ( abs(last.x - x) <= this->CompensationDistrance && abs(last.y - y) <= this->CompensationDistrance )	return false;

		if (size >= 2)
		{
			//�O�X�� (2�O)�Ɓ@�O��(1�O)�̌X��������������������A
			//�O��̏������A����̒l����͂���.
			POINT moreLast = Points[size - 2];
			if (	Normalize(moreLast.x - last.x) == Normalize(last.x - x) &&
					Normalize(moreLast.y - last.y) == Normalize(last.y - y)	)
			{
				//�O��̂��㏑��
				POINT p;	p.x = x;	p.y = y;
				Points[size - 1] = p;
				return false;
			}
		}
	}
	//�����A�ǉ����Ă݂����B
	POINT p;	p.x = x;	p.y = y;
	Points.push_back( p );

	return true;

}

