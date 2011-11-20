// RDiskUtil.h: RDiskUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
#define AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

class RDiskUtil  
{
public:
	RDiskUtil();
	virtual ~RDiskUtil();

	//�R�s�[(�f�B���N�g���̏ꍇ�ȉ��S��)
	static void Copy(const string & inFilename,const string & inTo, bool isSilent = true) throw(RWin32Exception);
	//�ړ�(�f�B���N�g���̏ꍇ�ȉ��S��)
	static void Move(const string & inFilename,const string & inTo, bool isSilent = true) throw(RWin32Exception);
	//�폜(�f�B���N�g���̏ꍇ�ȉ��S��)
	static void Delete(const string & inFilename, bool isSilent = true) throw(RWin32Exception);
	//�S�~����
	static void Gomi(const string & inFilename, bool isSilent = true) throw(RWin32Exception);
	//�f�B���N�g���̍쐬
	static void MakeDirectory(const string & inDirectoryName);
	//�t�@�C�������݂��邩?
	static bool Exist(const string & inFileName);

private:
	//�t�@�C������ \0\0 �ŏI����Ă���K�v�����邽�ߕt������
	static char* getDoubleNULLFilename(const string & inStr);
};

#endif // !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
