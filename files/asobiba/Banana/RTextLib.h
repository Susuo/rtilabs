// RTextLib.h: RTextLib �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTEXTLIB_H__862FC1F4_7BC2_4933_B460_345B6DB07A22__INCLUDED_)
#define AFX_RTEXTLIB_H__862FC1F4_7BC2_4933_B460_345B6DB07A22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RTextLib  
{
public:
	RTextLib();
	virtual ~RTextLib();

	//inFind �̎��̕�����܂œǂݔ�΂�
	static const string Skip(const string & inTarget , const string & inFind);
	//inFind �̒��O�܂œǂݔ�΂�.
	static const string SkipBefor(const string & inTarget , const string & inFind);
	//inStart �` inEnd �܂ł̎擾.
	static const string Cut(const string & inTarget , const string & inStart , const string & inEnd);
};

#endif // !defined(AFX_RTEXTLIB_H__862FC1F4_7BC2_4933_B460_345B6DB07A22__INCLUDED_)
