// RStatueControl.cpp: RStatueControl �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RStatueControl.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStatueControl::RStatueControl()
{

}

RStatueControl::~RStatueControl()
{

}

//�쐬.
void RStatueControl::Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,STATUSCLASSNAME,isParentWindow,inID,inFlg,0);
}

