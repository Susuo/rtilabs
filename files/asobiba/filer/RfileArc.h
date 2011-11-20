// RfileArc.h: RfileArc �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_)
#define AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "Rfile.h"

class RfileArc : public Rfile  
{
public:
	RfileArc();
	virtual ~RfileArc();

	//�o�^���܂��B
	virtual void Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute) = 0;

	//�I�[�v��
	//@inPath	�p�X(Rfile�̓t�@�C�������������Ă��Ȃ�)
	//@inMode	�J�����[�h
	//#�J���Ȃ������ꍇ�� RIOCannotOpenException �𓊂���
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual void Open(string inPath ,__RFILEOPENTYPE inMode) throw();

	//�N���[�Y
	//#����Ȃ������ꍇ�� RIOCannotCloseException �𓊂���
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual void Close() throw();

	//���[�h
	//@outBuf		�o�b�t�@
	//@inBufferSize	�o�b�t�@��
	//#�ǂݍ��ݎ��s�� RIOCannotReadException ��Ԃ�
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual bool Read(char* outBuf,int inBufferSize) throw();

	//���C�g
	//@outBuf		�o�b�t�@
	//@inBufferSize	�o�b�t�@��
	//?	>= 0 �������񂾃o�C�g��
	//  <  0 Eof
	//#�������ݎ��s�� RIOCannotReadException ��Ԃ�
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual bool Write(char* outBuf,int inBufferSize) throw();

	//�ǂݍ��ݏI���܂ő҂�.
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual void WaitFor() throw();

	//�I��������ǂ������ׂ�
	//? true	�I�����WaitFor() ���ł���
	//? false	�܂�
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual bool Poll() throw();

	//�����̌��ʂ����炢�܂��B
	//Poll �� true / �܂��́AWaitFor �̃��^�[����Ɏg���Ă�������.
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual unsigned long GetReport() throw();
};

#endif // !defined(AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_)
