// RDialogUtil.h: RDialogUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_)
#define AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDialogUtil  
{
public:
	//�t�@�C�����J���_�C�A���O
	static string FileOpenDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
		const string & inDefualtFilename ,const TCHAR* inFilter);
	//�t�@�C����ۑ��_�C�A���O
	static string FileSaveDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
		const string & inDefualtFilename ,const TCHAR* inFilter);
	//�p�X�I���_�C�A���O
	static string SelectPath(HWND inWindowHandle,const string &inTitle,const string &inPath);

};

#endif // !defined(AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_)
