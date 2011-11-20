// RMimeType.h: RMimeType �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_)
#define AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RAlphabetHash.h"


class RMimeType  
{
private:		//singleton
	RMimeType();
public:
	virtual ~RMimeType();

	void Load(const string inMimeFilname);

	/**
	 * getMimeType:			Mime�̎擾
	 *
	 * @param inStr				�擾�������g���q(�s���I�h���܂߂Ă͂����Ȃ��B) 
	 * @return const string		Mime
	 */
	const string getMimeType(const string inStr) const
	{
//		map< unsigned long , string>::const_iterator i = MimeType.find( RAlphabetHash(inStr.c_str()) );
		RStringMap::const_iterator i = MimeType.find( inStr );
		if (i == MimeType.end() ) return "text/plain";	//�s���ȂƂ��͎�荇����
		return (*i).second;
	}

	static RMimeType* getInstance();

	static void test();

private:
	//���̋�؂�܂œǂݔ�΂�.
	const char* nextSpace(const char* inStr) const;
	//���̋�؂�ł͂Ȃ������܂œǂݔ�΂�.
	const char* nextNonSpace(const char* inStr) const;

//	map< unsigned long , string>	MimeType;
	RStringMap						MimeType;
};

#endif // !defined(AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_)
