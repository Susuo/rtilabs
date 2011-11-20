// BConf.h: BConf �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCONF_H__AA6C4AD8_961F_4983_A8C3_22F39DAB8923__INCLUDED_)
#define AFX_BCONF_H__AA6C4AD8_961F_4983_A8C3_22F39DAB8923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class BConf  
{
public:
	virtual ~BConf();
	
	static BConf* getConf();

	void Create();

	//�J�����g�f�B���N�g���̎擾.
	const string getCurrentDirectory() const
	{
		return CurrentDirectory;
	}

private:
	//�J�����g�f�B���N�g���̎擾 �����p.
	string CommonCurrentDirectory() const;

private:
	BConf();	//singleton

private:
	string CurrentDirectory;
};

#endif // !defined(AFX_BCONF_H__AA6C4AD8_961F_4983_A8C3_22F39DAB8923__INCLUDED_)
