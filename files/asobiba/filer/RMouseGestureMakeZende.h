// RMouseGestureMakeZende.h: RMouseGestureMakeZende �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_)
#define AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RMouseGestureMousePoints.h"

const int MAX_ZONDE = 12;

class RMouseGestureZonde
{
public:
	RMouseGestureZonde()
	{
		ZeroMemory(&Hit , sizeof(unsigned char) * MAX_ZONDE);
	}
	//�q�b�g���̎擾.
	int getHit(int inIndex) const
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		return Hit[inIndex];
	}
	//�q�b�g���̐ݒ�.
	void setHit(int inIndex,unsigned char inVal)
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		Hit[inIndex] = inVal;
	}
	//�q�b�g�����J�E���g�A�b�v
	void upHit(int inIndex)
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		Hit[inIndex] ++;
	}

	//��v���̌v�Z.
	static int Rating(const RMouseGestureZonde & inMy , const RMouseGestureZonde & inTarget)
	{
		return 0;
	}

private:
	//�q�b�g�f�[�^.
	unsigned char	Hit[MAX_ZONDE];
};

//�����̌����� http://www.hidecnet.ne.jp/~sinzan/tips/c/c_tip15.htm ���Q�l�ɂ��܂����B
class RMouseGestureMakeZende  
{
private:
	class LINE	//�����A����
	{
	public:
		LINE(POINT a,POINT b)
		{
			this->a = a;
			this->b = b;
		}
		LINE(int sx,int sy , int ex,int ey)
		{
			this->a.x = sx;
			this->a.y = sy;
			this->b.x = ex;
			this->b.y = ey;
		}
		POINT	a; // �n�_
		POINT	b; // �I�_
	};
public:
	RMouseGestureMakeZende();
	virtual ~RMouseGestureMakeZende();

	//Zonde�@�ɂ���͂��܂��B
	RMouseGestureZonde Convert(MOUSE_GESTURE_MOUSE_POINTS inPoints);
private:
	//�����𔻒�.
	// ���� e1, e2 ���������邩���肷��B
	// ���� e1, e2 ����������ΐ^�A�����Ȃ��΋U��Ԃ�
	bool Intersect( LINE e1, LINE e2 );
	// �_��������ɑ��݂���ꍇ�ɂ́w�������ׂ��x�Ɣ��f����B
	// �_ a, b ������ e ���ׂ��ő��݂��邩���肷��B
	// �_ a, b ������ e ���ׂ��ł���ΐ^�A�����Ȃ��΋U��Ԃ�
	bool Straddle( LINE e, POINT a, POINT b );
	// �_ p ���L������ e(a,b) �̂ǂ��瑤�ɂ��邩���ׂ�B
	// �_ p ���L������ e �́@�E���ɂ���ꍇ  1 ���A
	// �@   �@�L������ e �̒�����ɂ���ꍇ  0 ���A
	// �@   �@�L������ e �́@�����ɂ���ꍇ -1 ��Ԃ�
	int Side( POINT p, LINE e );

};

#endif // !defined(AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_)
