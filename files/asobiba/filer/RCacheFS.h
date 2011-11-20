// RCacheFS.h: RCacheFS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_)
#define AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "Rfiles.h"


class RCacheFS  
{
private:
	RCacheFS();
public:
	virtual ~RCacheFS();

	static RCacheFS* getInstance();

	//�L���b�V���̌���.
	//������Ȃ������� NULL
	Rfiles* Search(__RFILETYPE inRfileType , string inPath);

	//�L���b�V���̊i�[.
	void Append(Rfiles* inRfiles);

	//�L���b�V���̃N���A.
	void Remove(Rfiles* inRfiles);



private:
	//�L���b�V��
	//�d���Ȃ�悤�������� map �ɂ��Ă���.
	typedef deque<Rfiles*>	RFILESCACHE;
	RFILESCACHE Cache;
};

#endif // !defined(AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_)
