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
	void Load(const string inFilename);

public:	//�T�[�r�X
	//��{�`.
	template <class _T>
	_T LoadOrDefault(const string inKeyName ,_T inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		string a = ((*i).second);
		return _T( a );
	}

	//string �̏ꍇ.
	template<>
	string LoadOrDefault<const string>(const string inKeyName ,string inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return (*i).second;
	}
	//int �̏ꍇ.
	template<>
	int LoadOrDefault<int>(const string inKeyName ,int inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() );
	}

	static void test();
public:
	RStringMap		Setting;
};

#endif // !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
