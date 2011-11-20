// RConfigFile.h: RConfigFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
#define AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RConfigFile  
{
public:
	RConfigFile();
	virtual ~RConfigFile();

public:
	/**
	 * Load:				�ݒ�t�@�C����ǂݍ���.
	 *
	 * @param inFilename	�t�@�C����
	 * @return void 
	 */
	void Load(const string & inFilename);

	/**
	 * Save:				��������
	 *
	 * @param inFilename	�t�@�C����
	 * @return void 
	 */
	void Save(const string & inFilename) ;

public:	//�T�[�r�X
	//��{�`.
	template<typename _T>
	_T LoadOrDefault(const string & inKeyName ,_T inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() );
	}

	//string �̏ꍇ.
	template<>
	string LoadOrDefault<string>(const string & inKeyName ,string inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() )
		{
			return inDefualt;
		}
		return (*i).second;
	}
	//bool
	template<>
	bool LoadOrDefault<string>(const string & inKeyName ,bool inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() ) == 0 ? false : true;
	}

	//��{�`.
	template <typename _T>
	void Upsert(const string & inKeyName ,_T inValue  ) 
	{
		Setting[inKeyName] = num2str(inValue);
	}
	template<>
	void Upsert<string>(const string & inKeyName ,string  inValue)
	{
		Setting[inKeyName] = inValue;
	}
	template<>
	void Upsert<bool>(const string & inKeyName ,bool  inValue)
	{
		Setting[inKeyName] = num2str(inValue == true ? 1 : 0);
	}

	void Remove(const string & inKeyName ) 
	{
		RStringMap::iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return ;

		Setting.erase(i);
	}

	static void test();
public:
	RStringMap		Setting;
};

#endif // !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
