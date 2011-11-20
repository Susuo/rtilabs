// RArchiveFactory.h: RArchiveFactory �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVEFACTORY_H__DB191442_6166_422C_A986_E81EF0FD847C__INCLUDED_)
#define AFX_RARCHIVAFECTORY_H__DB191442_6166_422C_A986_E81EF0FD847C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RArchive.h"

class RArchiveFactory  
{
private:
	RArchiveFactory();
	virtual ~RArchiveFactory();
public:
	//�T�|�[�g���Ă��Ȃ���� RNotSupportException
	//!�������ł��Ȃ������� RDoNotInitaliseException
	//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
	static RArchive* New(__ARCHIVETYPE inArchiveType) throw();

};

#endif // !defined(AFX_RARCHIVEFACTORY_H__DB191442_6166_422C_A986_E81EF0FD847C__INCLUDED_)
