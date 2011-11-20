// RCollect.h: RCollect �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_)
#define AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

template <class _K,class _T>
class RCollect  
{
	typedef map<_K,_T*>	RCOLLECTDEF;
public:
	RCollect()
	{
	}
	virtual ~RCollect()
	{
		//���ׂĉ��.
		for(RCOLLECTDEF::iterator i = Def.begin() ; i != Def.end() ; ++i)
		{
			delete (*i).second;
		}
	}


	class CharProxy
	{
	public:
		//�쐬
		CharProxy(RCollect<_K,_T>& str,_K index) : m_String(str) , m_CharIndex(index){};
	private:
		//���X�N���傫���̂Ŕ�T�|�[�g
		//lvalue my[10] = sting[20];
		CharProxy& operator=(const CharProxy& rhs)
		{
			m_String.Def[this->m_CharIndex] = rhs.Def[rhs.m_CharIndex];
			return *this;
		}
	public:
		//lvalue my[10] = 'c';
		CharProxy& operator=(_T* c)
		{
			RCOLLECTDEF::iterator i = m_String.Def.find(this->m_CharIndex);
			if (i == m_String.Def.end() )
			{
				m_String.Def.insert( pair<_K,_T*>(this->m_CharIndex,c) );
			}
			else
			{
				delete (*i).second;
				(*i).second = c;
			}
			return *this;
		}
		//rvalue char a = my[10];
		operator _T*() const
		{
			return *(this->m_String.Def[this->m_CharIndex] );
		}
		RCollect<_K,_T>& m_String;	//�v���L�V�������镶����
		_K m_CharIndex;	//���̒��ł��̃v���L�V����������

	};
	//String�N���X�̉���
	const CharProxy operator[](_K index) const	//const String �̂���
	{
		return CharProxy( (RCollect<_K,_T>&)*this,index);
	}
	//�� const String �̂���
	CharProxy operator[](_K index)
	{
		return CharProxy(*this,index);
	}
	friend class CharProxy;


	_T* find(_K index) const
	{
		RCOLLECTDEF::const_iterator i = Def.find(index);
		if (i == Def.end() ) return NULL;
		return (*i).second;
	}
private:
	RCOLLECTDEF	Def;	

};

#endif // !defined(AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_)
