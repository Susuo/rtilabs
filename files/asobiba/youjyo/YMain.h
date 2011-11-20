// YMain.h: YMain �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YMAIN_H__3A83281E_7949_4410_B7FE_E52ED435DBF8__INCLUDED_)
#define AFX_YMAIN_H__3A83281E_7949_4410_B7FE_E52ED435DBF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RSocket.h"
#include "RRequest.h"
#include "YBBSManager.h"
#include "RAutoOverLapped.h"
#include "YConnectNotify.h"

#include "RMmap.h"


class YMain  : public RThread
{
public:
	YMain();
	virtual ~YMain();

	//���ꂪ���s�����̂ł���[
	virtual unsigned int Run();
	//��~�𖽂���
	//�����͂����ɋA���Ă��܂��B
	//��~�t���O�𗧂Ă܂��B
	virtual void	HoldUp();

private:
	//���C������
	bool YMain::Execute();

	RMmap	Mmap;

};

#endif // !defined(AFX_YMAIN_H__3A83281E_7949_4410_B7FE_E52ED435DBF8__INCLUDED_)
