// RTaskView.h: RTaskView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKVIEW_H__9CAB39DF_D362_4B49_93B9_03E5D6DBCC41__INCLUDED_)
#define AFX_RTASKVIEW_H__9CAB39DF_D362_4B49_93B9_03E5D6DBCC41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


class RTaskView  
{
protected:
	RTaskView();
public:
	virtual ~RTaskView();

	//�V�����^�X�N��ǉ����܂��B
	virtual void InsertTask(unsigned long inID,string inMessage) = 0;
	//�i���̍X�V
	virtual void UpdateGoing(unsigned long inID,int inParsent) = 0;
	//�^�X�N�̍폜
	virtual void DeleteTask(unsigned long inID) = 0;
	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnSize() = 0;

};

#endif // !defined(AFX_RTASKVIEW_H__9CAB39DF_D362_4B49_93B9_03E5D6DBCC41__INCLUDED_)
