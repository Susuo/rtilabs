// XLConfigFile.h: XLConfigFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLCONFIGFILE_H__5F9ED5B9_6207_4319_ACC1_D3625FF3F3ED__INCLUDED_)
#define AFX_XLCONFIGFILE_H__5F9ED5B9_6207_4319_ACC1_D3625FF3F3ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class XLConfigFile  
{
public:
	XLConfigFile();
	virtual ~XLConfigFile();
public:
	/**
	 * Load:				�ݒ�t�@�C����ǂݍ���.
	 *
	 * @param inFilename	�t�@�C����
	 * @return bool 
	 */
	bool Load(const string & inFilename);

	/**
	 * Save:				��������
	 *
	 * @param inFilename	�t�@�C����
	 * @return bool 
	 */
	bool Save(const string & inFilename) ;


public:	//�T�[�r�X
	//��{�`.
	template<typename _T>
	_T LoadOrDefault(const string & inKeyName ,_T inDefualt  ) const
	{
		XLStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() );
	}

	//string �̏ꍇ.
	template<>
	string LoadOrDefault<string>(const string & inKeyName ,string inDefualt  ) const
	{
		XLStringMap::const_iterator i = Setting.find(inKeyName);
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
		XLStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() ) == 0 ? false : true;
	}
	//double
	template<>
	double LoadOrDefault<string>(const string & inKeyName ,double inDefualt  ) const
	{
		XLStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atof( (*i).second.c_str() );
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
	template<>
	void Upsert<bool>(const string & inKeyName ,double  inValue)
	{
		Setting[inKeyName] = format("%f", inValue);
	}

	void Remove(const string & inKeyName ) 
	{
		XLStringMap::iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return ;

		Setting.erase(i);
	}

	static void test();
public:
	XLStringMap		Setting;

};

#endif // !defined(AFX_XLCONFIGFILE_H__5F9ED5B9_6207_4319_ACC1_D3625FF3F3ED__INCLUDED_)
