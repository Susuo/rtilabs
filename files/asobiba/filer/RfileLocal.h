// RfileLocal1.h: RfileLocal �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_)
#define AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileLocal;

#define FILE_EXIST(name)	(0xFFFFFFFF != GetFileAttributes(name))

#include "comm.h"
#include "Rfile.h"
#include "Rfiles.h"
//���[�J���t�@�C���̓ǂݍ��݁A�������݂̕�����S�����܂��B
//���ꂾ���ł͂܂��g���Ȃ��̂ŁA
//����ɏ�ʂɌp�����Ďg���K�v������܂��B
class RfileLocal : public Rfile  
{
public:
	RfileLocal();
	virtual ~RfileLocal();

	//�o�^���܂��B
	virtual void Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib) = 0;

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

private:
	HANDLE		Handle;
	OVERLAPPED	OverLapped;

};

#endif // !defined(AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_)
