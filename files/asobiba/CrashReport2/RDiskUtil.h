// RDiskUtil.h: RDiskUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
#define AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDiskUtil  
{
public:
	RDiskUtil();
	virtual ~RDiskUtil();

	//�t�@�C�������݂��邩?
	static bool Exist(const string & inFileName);

	//�J�����g�f�B���N�g���̎擾.
	static string RDiskUtil::getCurrentDirectory() ;
	
private:
};

#endif // !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
