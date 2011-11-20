// RTask.h: RTask �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_)
#define AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTask  ;

#include "comm.h"
#include "window.h"
#include "WorkerThread.h"

class RTask  : public Request
{
public:
	RTask(Rfiles* inNewFiles, Display* inDisp , Window* inCallback);
	virtual ~RTask();
public:
	//�i���� �J�n0 ----> ���S100 
	virtual int Parsent() = 0;
	//�^�X�N�r���[�ɕ\�����郁�b�Z�[�W
	virtual string getMessage() =0;

	//���݃^�[�Q�b�g�ɂȂ��Ă���E�B���h�E���擾���܂��B
	const Window*	getWindow();

	//��~�𑣂��܂��B
	void Stop();

	//�^�X�N�I����A���C���X���b�h����Ăяo�����
	//�����ȃ^�X�N�ł��邽�ߔj������
	void Invalid();
	//�^�X�N�I����A���C���X���b�h����Ăяo�����
	//�L���ł���̂ōX�V����
	void WindowUpdate();
protected:
	//�R�[���o�b�N���s.
	void	CallBack();
	//�G���[�R�[���o�b�N���s.
	void	ErrorCallBack();

protected:
	//�������˗����� window
	Window*			Win;
	//�������� Rfiles*
	Rfiles*			Files;
	//�������� Display*
	Display*		Disp;

};

#endif // !defined(AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_)
