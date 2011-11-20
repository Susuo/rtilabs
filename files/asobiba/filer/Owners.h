// Owners.h: Owners �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_)
#define AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Owners;

#include "comm.h"


//�t�@�C�����������Ă���I�[�i�[���񂽂�.
//Composite�p�^�[��.
class Owners  
{
private:
	//����ɃI�[�i�[������ꍇ.
	Owners*		MoreOwners;
	string		Name;
public:
	//���Ȃ炸 new �Ŋm�ۂ��邱��!!
	Owners(string inName , Owners* inMoreOwners = NULL)
	{
		Name = inName;
		MoreOwners = inMoreOwners;
	}
private:
	//new �ȊO�ł̍쐬���֎~���邽�߂̏��u.
	virtual ~Owners()
	{
		if ( MoreOwners ) MoreOwners->Delete();
	}
public:
	//new ���ȊO���֎~���邽�߂� �f�X�g���N�^�� private �ɂ������߂�
	//�O������delete �ł��Ȃ��Ȃ����B�@���̂��߂Ɏ������\�b�h��p�ӂ���
	void		Delete(){	delete this;	}
	//�I�[�i�[�̖��O��Ԃ�.
	string		getName(){	return Name;	}
	//���̃I�[�i�[���� NULL �ŏI��.
	Owners*		next(){		return MoreOwners;	}

	//�e�X�g
	static void test()
	{
		Owners * p = new Owners("�ƂƂ�", new Owners("�߂�", new Owners("����")) );

		Owners * d;
		d = p;

		string debugBuffer;
		while(d)
		{
			debugBuffer += d->getName();
			d = d->next();
		}

		if (debugBuffer != "�ƂƂ�߂�����")
		{
			ASSERT(0);
		}

		p->Delete();
	}
};

#endif // !defined(AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_)
