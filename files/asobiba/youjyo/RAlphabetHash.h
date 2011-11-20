// RAlphabetHash.h: RAlphabetHash �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_)
#define AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//�A���t�@�x�b�g�������ɔ��肵�܂��B
//�������A������ only �� 5�����܂łł��B
//����ȊO�̌��ʂ͖���`�ł��B
class RAlphabetHash  
{
public:
	RAlphabetHash()
	{
		Hash = 0;
	}
	RAlphabetHash(const char* p)
	{
		this->Hash = getHash(p);
	}
	~RAlphabetHash()	//�p���֎~
	{
	}

	/**
	 * getHash:	�n�b�V�������߂�.
	 *
	 * @param p					�n�b�V�������镶����(������ only �� 5����)
	 * @return unsigned long	�n�b�V���l
	 */
	unsigned long getHash(const char* p) const
	{
#ifdef _DEBUG
		{
			//������ only �� 5���� �ł��邩�ǂ���
			for(int i = 0 ; i < 6 ; i ++)
			{
				if (p[i] == '\0') break;
				ASSERT( islower(p[i]) );
			}
			ASSERT(i <= 5);
		}
#endif
		if (p[0] == '\0') return 0;
		else if (p[1] == '\0') return (p[0] - 97);
		else if (p[2] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6);
		else if (p[3] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12);
		else if (p[4] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18);
		else if (p[5] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18) + ((p[4] - 97) << 24);
		else                   return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18) + ((p[4] - 97) << 24) + ((p[5] - 97) << 30);
	}

	RAlphabetHash& operator=(const char* p)
	{
		this->Hash = getHash(p);
		return (*this);
	}
	bool operator==(const char* p)
	{
		return this->Hash == getHash(p);
	}
	bool operator!=(const char* p)
	{
		return this->Hash != getHash(p);
	}
	bool operator==(unsigned long p)
	{
		return this->Hash == p;
	}
	bool operator!=(unsigned long p)
	{
		return this->Hash != p;
	}


	operator unsigned long() const 
	{
		return this->Hash;
	}
	operator unsigned int() const 
	{
		return this->Hash;
	}

	static void test();

private:
	unsigned long		Hash;
};

#endif // !defined(AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_)
