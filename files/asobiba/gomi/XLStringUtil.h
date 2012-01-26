// XLStringUtil.h: XLStringUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
#define AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLStringUtil  
{
public:
	XLStringUtil();
	virtual ~XLStringUtil();

	static string UrlDecode(const string & inUrl);

	//���E�̋󔒂̏���
	static string chop(const string & inString);

	//�u��
	static string Replace(const string &inTarget ,const string &inOld ,const string &inNew);

	//�^�O���G�X�P�[�v ��{�I�� PHP �� htmlspecialchars �Ɠ���.
	//http://search.net-newbie.com/php/function.htmlspecialchars.html
	static string htmlspecialchars(const string &inStr);

	//\n��<br>�� ��{�I�� PHP �� nl2br �Ɠ���.
	static string nl2br(const string &inStr);

	//inTarget �� inStart �` inEnd �܂ł��擾
	static string Cut(const string &inTarget , const string & inStart , const string &inEnd , string * outNext = NULL);

	static void test();

};

#endif // !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
