// REasyAI.h: REasyAI �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_)
#define AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

template<class _T , class _RATINGFUNC , int _USER_TEACH_DEFAULT_RATE = 2>
class REasyAI  
{
public:
	REasyAI()
	{
	}
	virtual ~REasyAI()
	{
	}

	//�p�^�[���Ɉ�ԋ߂��Ǝv����֘A�t����ꂽ���O�̎擾.
	string Place(const _T & inPattern)
	{
		string name = "";
		int maxRate = INT_MIN;
		for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
		{
			int rate = (*i).getRate(inPattern);
			if ( maxRate < rate)
			{
				name = (*i).getName();
				maxRate = rate;
			}
		}
		return name;
	}

	//������������.
	//inPattern		�����̃p�^�[��
	//inRight		����
	//inError		�Ԉ���Ď��ʂ��Ă��܂�������
	void Teach(const _T & inPattern , string inRight , string inError = "")
	{
		if (! inError.empty() )
		{
			//�Ԉ���ĔF���������̓_����������.
			for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
			{
				if ((*i).getName() == inError)
				{
					(*i).Down();
				}
			}
		}
		bool BetterData = false;
		//�_����90�_�ȏ�̃f�[�^�����݂����炻�����グ�A�����͉����Ȃ�
		//�_����75�_�ȏ�̃f�[�^�����݂����炻�����グ��A
		//�_����30�_�ȉ��̃f�[�^�ł�Γ_��������ɉ�����.
		for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
		{
			//���O�������łȂ��ƒ��ׂĂ����Ӗ�...
			if ((*i).getName() == inRight)
			{
				int rate = (*i).getRate(inPattern);
				if ( rate >= 90)		//���悿���@�i�G�L�D`�jʧʧ.
				{
					BetterData = true;
					(*i).Up();
				}
				else if (rate <= 75)	//���΂炵��
				{
					(*i).Up();
				}
				else if (rate <= 30)	//�ԓ_��[. �{���N���[�Y.
				{
					(*i).Down();
				}
			}
		}

		//�ň��l�̂��𖕏�.
		for( i = Memory.begin() ; i != Memory.end() ; )
		{
			if ( (*i).IsWarstData() )
			{
				THE_MEMORY::iterator  del = i;
				++i;
				Memory.erase(del);
			}
			else		++i;
		}

		//90�_�ȏ�̃f�[�^���Ȃ������琳����������.
		if (BetterData)
		{	//������������.
			Memory.push_back( LimitMemory(inRight,inPattern) );
		}
	}
private:
	class LimitMemory
	{
	public:
		LimitMemory(const string inName ,const _T & inPattern)
		{
			Name = inName;
			Pattern = inPattern;
			RateExperience = _USER_TEACH_DEFAULT_RATE;
		}
		//�G���[��������_����������.
		void Down()
		{
			RateExperience--;
		}
		//���������̂œ_�����グ��.
		void Up()
		{
			RateExperience--;
		}
		//�ň��l���X�V���Ă܂�?
		bool IsWarstData()
		{
			return RateExperience < 0;
		}
		//�_���t�� 100�_���_.
		int getRate(const _T & inPattern)
		{
			return _RATINGFUNC::Rating(inPattern , Pattern);
		}
		string getName()
		{
			return Name;
		}
	private:

		//�p�^�[����
		string		Name;
		//�f�[�^
		_T			Pattern;
		//���܂ł̈�v�֌W	//�}�C�i�X�ɂȂ�Ə���.
		int			RateExperience;
	};
private:
	typedef deque<LimitMemory>	THE_MEMORY;
	THE_MEMORY					Memory;
};

#endif // !defined(AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_)
