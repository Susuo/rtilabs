// SolvablePath.h: SolvablePath �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_)
#define AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvablePath;


#include "comm.h"
#include "PathInterface.h"

//�p�X�Ǘ�.
class SolvablePath  : public PathInterface
{

public:
	SolvablePath(SolvablePath* inNext = NULL)
	{
		this->Next = inNext;
	}
	virtual ~SolvablePath()
	{
		delete Next;
	}

	//�������Ă�������
	SolvablePath* PleaseSolve(string inPath)
	{
		//�����łł���? �ł����牴���������@���Ă��ƂŎ������A���B
		if ( this->DoSolvable(inPath) ) return this;
		//���̐l������Ȃ炻�̐l���������Ă����悫���ƁBchain of Responsibility
		if ( this->Next ) return this->Next->PleaseSolve(inPath);
		//�Ȃ�ł����A�����ł��Ȃ����B �������߂ہB
		return NULL;
	}
protected:
	//���̃p�X���������Ă݂�
	//�����ł����ꍇ�͉����������ʂ̃p�X�� �����o�� setPath �ŃZ�b�g���邱��.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ �ȂǂƂȂ�
	//�����ł����ꍇ�� true �����ł��Ȃ������ꍇ�� false
	virtual bool DoSolvable(string inPath)  = 0;

private:
	//���ɉ������������. chain of Responsibility
	SolvablePath*	Next;
};

#endif // !defined(AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_)
