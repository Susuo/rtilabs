// YYoujyoGC.h: YYoujyoGC �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_)
#define AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"

class YYoujyoGC  : public RThread  
{
public:
	YYoujyoGC();
	virtual ~YYoujyoGC();

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

};

#endif // !defined(AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_)
