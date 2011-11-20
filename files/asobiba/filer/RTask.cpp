// RTask.cpp: RTask �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNotify.h"
#include "RTask.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTask::RTask(Rfiles* inNewFiles, Display* inDisp , Window* inCallback) : Request()
{

	//�������˗����� window
	Win = inCallback;
	//�������� Rfiles*
	Files = inNewFiles;
	//�������� Display*
	Disp = inDisp;
}

RTask::~RTask()
{

}

//���݃^�[�Q�b�g�ɂȂ��Ă���E�B���h�E���擾���܂��B
const Window*	RTask::getWindow()
{
	return Win;
}

//��~�𑣂��܂��B
void RTask::Stop()
{
	// ���̂����������܂�(w.
}


//�R�[���o�b�N���s.
void	RTask::CallBack()
{
	//�X�V�K�v�����B
	RNotify<RTask,Window>	Nofity;
	Nofity.CallBack(this,Win);
}
//�G���[�R�[���o�b�N���s.
void	RTask::ErrorCallBack()
{
	//�X�V�K�v�����B
	RNotify<RTask,Window>	Nofity;
	Nofity.CallBack(this,NULL);
}

//�^�X�N�I����A���C���X���b�h����Ăяo�����
//�����ȃ^�X�N�ł��邽�ߔj������
void RTask::Invalid()
{
	if (Files) Files->Dec(); // delete Files;

//	delete Disp;

	Files = NULL;
	Disp = NULL;
}

//�^�X�N�I����A���C���X���b�h����Ăяo�����
//�L���ł���̂ōX�V����
void RTask::WindowUpdate()
{
	if (Win)	Win->UpdateWindow(Files,Disp);
}

