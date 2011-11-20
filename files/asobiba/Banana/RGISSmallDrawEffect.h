// RGISSmallDrawEffect.h: RGISSmallDrawEffect �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_)
#define AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMathAngle.h"

enum ParticularLevel
{
	LEVEL_FULL,				//���ׂĂ̐����̕\��
	LEVEL_NOT_BANCHI,		//�Ԓn���x���̕\�����Ƃ���
	LEVEL_NOT_CHIMEI,		//�n�����x���̕\�����Ƃ���
	LEVEL_NOT_KOUKYOU,		//�����@�փ��x���̕\�����Ƃ���
	LEVEL_NOT_LOWLEVEL,		//�H�n�Ȃǂ̏����������x���̕\���̎����
	LEVEL_NOT_MIDDLELEVEL,	//��ʓ��Ȃǂ̕��ʂ̓����x���̕\���̎����
};



#include "RGraphics.h"	//�b��

class RGISSmallDrawEffect  
{
public:

public:
	RGISSmallDrawEffect();
	virtual ~RGISSmallDrawEffect();

	void InitDisplay(const LPOINT * inGlobalJiku ,int inScreenWidth,int inScreenHeight, unsigned int inScale,RAngle inDeg);



	//�\������_�̍��W�ϊ�
	LPOINT RevAttach(const POINT * inXY) const;
	//�\������_�̍��W�ϊ�
	POINT Attach(const LPOINT * inXY) const;
	//�O���[�o�����W��\������_�̍��W�ɕϊ�
	RECT ConvertShowDisplay(const LRECT * inRect) const;
	//�`�悵�Ȃ���΂����Ȃ��ł��낤�n��̎擾.
	const LRECT* getGlobalDrawDisplay() const
	{
		return &GlobalDrawDisplay ;
	}

	//�b��
	void Boxes(int inScreenWidth,int inScreenHeight,unsigned short * image , unsigned char * inLevel    ,const LPOINT * inBlockStart,const COLORREF16 * inPallet) const;

	//�g�債���Ƃ��ɂ͂���郌�x���̌���
	ParticularLevel getLevel() const;
private:
	//�\���X�P�[��.
	unsigned int Scale;

	LPOINT		Jiku;	//��ʒ��S
	LRECT		GlobalDrawDisplay;	//�`�悵�Ȃ���΂����Ȃ��ł��낤�n��
	int			ScreenWidth;
	int			ScreenHeight;
	RAngle		Deg;
	RAngle		RevDeg;
	const RMathAngleFixed	* MathAngle;
};

#endif // !defined(AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_)
