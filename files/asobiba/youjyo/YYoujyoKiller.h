// YYoujyoKiller.h: YYoujyoKiller �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_)
#define AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"

class YYoujyoKiller  : public RThread  
{
public:
	YYoujyoKiller();
	virtual ~YYoujyoKiller();

	/**
	 * Run:	���s.
	 *
	 * @return virtual unsigned int 
	 */
	virtual unsigned int Run();

	/**
	 * Shutdown:	��~���ߑ��M.
	 *
	 */
	void Shutdown();

private:
	char	Buffer[THREAD_WORK_AREA];
};

#endif // !defined(AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_)
