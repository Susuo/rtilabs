// DeleteObject.h: DeleteObject �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELETEOBJECT_H__902034AF_5B39_49F2_B566_E638C7B04375__INCLUDED_)
#define AFX_DELETEOBJECT_H__902034AF_5B39_49F2_B566_E638C7B04375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//EffectiveSTL ���{���� 37�y�[�W���
class DeleteObject  
{
public:
	DeleteObject(){};

	template<typename T>
	void operator() (const T* ptr) const
	{
		TRACE("%p\n",str);
		delete ptr;
	}
};

#endif // !defined(AFX_DELETEOBJECT_H__902034AF_5B39_49F2_B566_E638C7B04375__INCLUDED_)
