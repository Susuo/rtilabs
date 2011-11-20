// YDATCache1.h: YDATCache �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_)
#define AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YDAT.h"
#include "RSpinLock.h"
#include "RAlreadyException.h"

class YDATCap : public YDAT
{
public:
	YDATCap(DATNUMBER	inDatNumber) : YDAT(inDatNumber)
	{
		Count = 0;
		LastAccess = time(NULL);
		OnMemoryTime = LastAccess;
	}
	void Use()
	{
		Count ++;
		LastAccess = time(NULL);
	}
	void UnUse()
	{
		Count --;
	}

	bool isUse()
	{
		return (Count != 0);
	}

	//�f�B�X�N�ɏ����߂���
	void WriteBack()
	{
		OnMemoryTime = time(NULL);
	}

	//���ݎ����Ƃ̍������߂�
	DWORD	DifferenceTime(DWORD inNow)
	{
		return inNow - LastAccess;
	}
	//���ݎ����Ƃ̍������߂�
	DWORD	DifferenceOnMemoryTime(DWORD inNow)
	{
		return inNow - OnMemoryTime;
	}
private:
	int		Count;
	//�ŏI�A�N�Z�X����
	DWORD	LastAccess;
	//��������ɂ������݂��Ă��Ȃ�����
	DWORD	OnMemoryTime;
};

class YDATCache  
{
public:
	YDATCache();
	virtual ~YDATCache();

	//DAT�̎擾.
	YDAT* getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException);
	//�V�K����
	YDAT* getNewDAT(DATNUMBER inDATNumber) throw (RAlreadyException);
	//DAT�g�p���~�߂�
	void Release(YDAT* inDAT);
	/**
	 * Collecter:		�K�y�[�W�R���N�g
	 *
	 * @return void 
	 */
	void YDATCache::Collecter();

	void setBoardName(const string inBoardName)
	{
		this->BoardName = inBoardName;
	}
	/////////////////�Ǘ��җp����
	//���ׂẴL���b�V�����f�B�X�N�Ƀt���b�V������.
	void Flush() const;
	//�g�p�����o��
	void Dump() const;


private:
	/**
	 * DeleteWhere:			�폜
	 *
	 * @param inPoolTime	�ێ��������
	 * @return void 
	 */
	void YDATCache::DeleteWhere(int inPoolTime);
	//�����ƃ�������ɂ������݂��Ă��� DAT �͊�Ȃ��̂Ńf�B�X�N�ɏ�������.
	void YDATCache::Saver(int inPoolTime);

private:
	string		BoardName;
	typedef map<DATNUMBER , YDATCap*>	DATCACHE;
	DATCACHE	Cache;
	mutable RSpinLock	Lock;

	int			UnHit ;
};

#endif // !defined(AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_)
