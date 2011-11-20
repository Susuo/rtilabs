// RCommKeyDown.h: RCommKeyDown �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_)
#define AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include "RFunctionScript.h"

//�������̂��߂ɂ�����Ɖ�����i���Ƃ�܂��B
//KEY_DATA �̏�� 8�o�C�g�͉��z�L�[�R�[�h�A ���� 8�o�C�g��SHIFT�L�[�ȂǂƓ��������̏ꍇ�� ON�ɂȂ�r�b�g�ł��B
typedef unsigned short	KEY_DATA;
#define MAKE_KEY_DATA(key,flg)		((((key)&0xff) << 8) | ((flg)&0xff))

enum	MODIFY_KEY
{
	MODIFY_KEY_NONE		=	0,
	MODIFY_KEY_SHIFT	=	1,
	MODIFY_KEY_CTRL		=	2,
	MODIFY_KEY_ALT		=	4,
	MODIFY_KEY_DBCLICK	=	0xff,
};

class RCommKeyDown  
{
public:
	RCommKeyDown();
	virtual ~RCommKeyDown();

	bool OnKeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey ) const throw (RParseException)
	{
		StringMap::const_iterator it = Keys.find( MAKE_KEY_DATA(inKeyCode,inModifyKey) );
		if (it == Keys.end())	return false;	//����ȃt�@���N�V�����Ȃ��ł��B

		RFunctionScript	rfs;
		rfs.Interpreter( (*it).second );


		return true;
	}
	void SetKey(unsigned char inKeyCode ,MODIFY_KEY inModifyKey , string Script)
	{
		Keys[ MAKE_KEY_DATA(inKeyCode,inModifyKey)  ] = Script;
	}



private:
	//�L�[���������Ƃ��ɔ�������R�}���h����.
	StringMap	Keys;
};

#endif // !defined(AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_)
