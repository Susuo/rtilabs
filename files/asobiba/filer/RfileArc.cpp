// RfileArc.cpp: RfileArc �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileArc.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RfileArc::RfileArc()
{

}

RfileArc::~RfileArc()
{

}

//�I�[�v��
//@inPath	�p�X(Rfile�̓t�@�C�������������Ă��Ȃ�)
//@inMode	�J�����[�h
//#�J���Ȃ������ꍇ�� RIOCannotOpenException �𓊂���
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
void RfileArc::Open(string inPath ,__RFILEOPENTYPE inMode) throw()
{
}

//�N���[�Y
//#����Ȃ������ꍇ�� RIOCannotCloseException �𓊂���
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
void RfileArc::Close() throw()
{
}

//���[�h
//@outBuf		�o�b�t�@
//@inBufferSize	�o�b�t�@��
//#�ǂݍ��ݎ��s�� RIOCannotReadException ��Ԃ�
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
bool RfileArc::Read(char* outBuf,int inBufferSize) throw()
{
	return true;
}

//���C�g
//@outBuf		�o�b�t�@
//@inBufferSize	�o�b�t�@��
//?	>= 0 �������񂾃o�C�g��
//  <  0 Eof
//#�������ݎ��s�� RIOCannotReadException ��Ԃ�
// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
bool RfileArc::Write(char* outBuf,int inBufferSize) throw()
{
	return true;
}

//�ǂݍ��ݏI���܂ő҂�.
//#�������ݎ��s�� RIOException ��Ԃ�
void RfileArc::WaitFor() throw()
{
}

//�I��������ǂ������ׂ�
//? true	�I�����WaitFor() ���ł���
//? false	�܂�
//#�������ݎ��s�� RIOException ��Ԃ�
bool RfileArc::Poll() throw()
{
	return false;
}


//�����̌��ʂ����炢�܂��B
//Poll �� true / �܂��́AWaitFor �̃��^�[����Ɏg���Ă�������.
//#�������ݎ��s�� RIOException ��Ԃ�
unsigned long RfileArc::GetReport() throw()
{
	return 0;
}
