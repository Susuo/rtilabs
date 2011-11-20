// Rfile.h: Rfile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_)
#define AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Rfile;

#include "comm.h"


#include "RFixedColumn.h"
#include "RNotSupportException.h"
#include "RIOCannotWriteException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotCloseException.h"


enum __RFILETYPE
{
	__RFILETYPE_DEBUG,		//�f�o�b�O
	__RFILETYPE_NONE_DUMMY,			//BLANK�y�[�W
	__RFILETYPE_LOCALFILE_REPORT,	//�ڍו\��
	__RFILETYPE_LOCALFILE_BIGICON,	//�A�C�R���\��
	__RFILETYPE_PREVIEWONE,			//�v���r���[���邽�߂̃t�@�C��
	__RFILETYPE_ARC_BIGICON,		//���k�t�@�C�����A�C�R���\��
	__RFILETYPE_SMBLOGON_REPORT,	//Windows���L�̃��O�C������
	__RFILETYPE_SMBLOGON_BIGICON,	//Windows���L�̃��O�C������
};

enum __RFILEOPENTYPE
{
	__RFILEOPENTYPE_READ,	//�ǂݍ���
	__RFILEOPENTYPE_WRITE,	//��������
};


class Rfile
{
public:
	Rfile()
	{
	}
	virtual ~Rfile()
	{
	}
public:

	//�\�[�g�L�[
	const void*		getSortKey() const	{	return SortKey;	}

	//��ނ�Ԃ�.
	virtual __RFILETYPE	getType() const	= 0;

	//�f�B���N�g�����ǂ������f.
	virtual bool IsDirectory() const = 0;

	//�l�̎擾.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const = 0;
	//�C���[�W�̎擾.
	virtual const int getImageIndex() const = 0;
	//�\�[�g�L�[��o�^���܂��B Attch�̌�ɌĂ�ł�������.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey) = 0;

	//�I�[�v��
	//@inPath	�p�X(Rfile�̓t�@�C�������������Ă��Ȃ�)
	//@inMode	�J�����[�h
	//#�J���Ȃ������ꍇ�� RIOCannotOpenException �𓊂���
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual void Open(string inPath ,__RFILEOPENTYPE inMode) throw()
	{
		throw RNotSupportException("�I�[�v���̓T�|�[�g����Ă��܂���");
	}

	//�N���[�Y
	//#����Ȃ������ꍇ�� RIOCannotCloseException �𓊂���
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual void Close() throw()
	{
		throw RNotSupportException("�N���[�Y�̓T�|�[�g����Ă��܂���");
	}
	//���[�h
	//@outBuf		�o�b�t�@
	//@inBufferSize	�o�b�t�@��
	//#�ǂݍ��ݎ��s�� RIOCannotReadException ��Ԃ�
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual bool Read(char* outBuf,int inBufferSize) throw()
	{
		throw RNotSupportException("�ǂݍ��݂̓T�|�[�g����Ă��܂���");
		return false;
	}

	//���C�g
	//@outBuf		�o�b�t�@
	//@inBufferSize	�o�b�t�@��
	//#�������ݎ��s�� RIOCannotReadException ��Ԃ�
	// �T�|�[�g����Ă��Ȃ������ RNotSupportException �𓊂���
	virtual bool Write(char* outBuf,int inBufferSize) throw()
	{
		throw RNotSupportException("�������݂̓T�|�[�g����Ă��܂���");
		return false;
	}

	//�ǂݍ��ݏI���܂ő҂�.
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual void WaitFor() throw()
	{
	}

	//�I��������ǂ������ׂ�
	//? true	�I�����WaitFor() ���ł���
	//? false	�܂�
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual bool Poll() throw()
	{
		return false ;
	}

	//�����̌��ʂ����炢�܂��B
	//Poll �� true / �܂��́AWaitFor �̃��^�[����Ɏg���Ă�������.
	//#�������ݎ��s�� RIOException ��Ԃ�
	virtual unsigned long GetReport() throw()
	{
		return 0;
	}


protected:
	const void*		SortKey;	//�\�[�g�Ɏg���v�f
};


#endif // !defined(AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_)
