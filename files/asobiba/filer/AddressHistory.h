// AddressHistory.h: AddressHistory �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_)
#define AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AddressHistory  ;

#include "comm.h"
#include "RNotFoundException.h"

class AddressHistory  
{
private:
	typedef deque<string>	HISTORY;
	HISTORY					History;
	int						HistoryPointer;
public:
	AddressHistory();
	virtual ~AddressHistory();

	//�i�߂��?
	bool IsNext() const;
	//�߂��?
	bool IsBack() const;
	//�߂�!
	//! �߂�Ȃ��Ƃ��ɂ� RNotFoundException  �𓊂���.
	string Back() throw(RNotFoundException);
	//�i��!
	//! �i�߂Ȃ��Ƃ��ɂ� RNotFoundException  �𓊂���.
	string Next() throw(RNotFoundException);
	//��O�̃p�X���擾���܂��B
	//! �擾�ł��Ȃ��Ƃ��� RNotFoundException  �𓊂���.
	string getLastPath() const throw(RNotFoundException);

	//�p�X�̒ǉ�
	void Append(string inAddress);

	static void test();
};

#endif // !defined(AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_)
