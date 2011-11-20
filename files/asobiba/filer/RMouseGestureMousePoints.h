// RMouseGestureMousePoints.h: RMouseGestureMousePoints �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_)
#define AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

typedef vector<POINT>	MOUSE_GESTURE_MOUSE_POINTS;

class RMouseGestureMousePoints  
{
public:
	RMouseGestureMousePoints();
	virtual ~RMouseGestureMousePoints();

	//�O�Ղ�ǉ�
	//�ǉ����ꂽ�ꍇ�� true
	//�|���V�[�ᔽ�Œǉ�����Ȃ��ꍇ�� false .
	bool Insert(int x,int y);

	//�O�Ճf�[�^������
	void Clear()
	{
		Points.clear();
	}
	//�|�C���g���擾
	//�C�����C���W�J�������������.
	const MOUSE_GESTURE_MOUSE_POINTS	getPoints() const
	{
		return Points;
	}
private:
	// 1 , 0 , -1 �̉��ꂩ�ɓ��ꂵ�܂��B
	int Normalize(int inSa)
	{
		//�܂������͂Ԃꂪ����̂ŕ␳���܂��B
		if ( ::abs(inSa) <= this->CompensationDistrance) return 0;
		if ( inSa > 0) return 1;
		return -1;
	}
private:
	//�␳���鋗��
	int				CompensationDistrance;
	//�}�E�X�̋O��. �����Ȃ������Ղ��Č�����ł���.
	MOUSE_GESTURE_MOUSE_POINTS	Points;
};

#endif // !defined(AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_)
