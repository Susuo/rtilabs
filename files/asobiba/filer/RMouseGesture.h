// RMouseGesture.h: RMouseGesture �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTURE_H__E9568F35_0EFA_4B07_B889_ACB4E526B3AF__INCLUDED_)
#define AFX_RMOUSEGESTURE_H__E9568F35_0EFA_4B07_B889_ACB4E526B3AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	

#include "comm.h"
#include "REasyAI.h"
#include "RMouseGestureMouseCapture.h"
#include "RMouseGestureMousePoints.h"
#include "RMouseGestureMakeZende.h"

//singleton
class RMouseGesture  
{
private:
	RMouseGesture();
public:
	virtual ~RMouseGesture();

	//singleton
	static RMouseGesture* getInstance();

	//����
	string Place(const RMouseGestureMouseCapture & inCapture)
	{
		RMouseGestureMakeZende	makeZonde;
		//Zonde �@�ɂ�莯�ʂ��܂��B
		this->LastZonde = makeZonde.Convert(inCapture.getPoints() );
		AI.Place(this->LastZonde);
	}

	//������?
	void Teach(string inRight , string inError )
	{
		if (inRight == inError)	inError = "";
		AI.Teach( this->LastZonde , inRight , inError);
	}

	RMouseGestureZonde	LastZonde;
	REasyAI<RMouseGestureZonde, RMouseGestureZonde , 2>	AI;

};

#endif // !defined(AFX_RMOUSEGESTURE_H__E9568F35_0EFA_4B07_B889_ACB4E526B3AF__INCLUDED_)
