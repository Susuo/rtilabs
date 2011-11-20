// RDialogUtil.cpp: RDialogUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDialogUtil.h"
#include <shlobj.h>
#include <commdlg.h>
#include "RMessageBox.h"
#include "RAutoLibraryWithException.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//�t�@�C�����J���_�C�A���O
string RDialogUtil::FileOpenDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
							 const string & inDefualtFilename , const TCHAR* inFilter)
{
#ifndef _WIN32_WCE
    OPENFILENAME ofn = { 0 };
	char retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		strcpy(retFilenameBuffer , inDefualtFilename.c_str() );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = inTitle.c_str();
	ofn.lpstrInitialDir = inPath.c_str();

    if (!GetOpenFileName(&ofn)) 
	{
		return "";
	}

	return retFilenameBuffer;
#else
	MY_USES_CONVERSION;

    OPENFILENAME ofn = { 0 };
	TCHAR retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		lstrcpy(retFilenameBuffer , _A2W(inDefualtFilename.c_str()) );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = _A2W(inTitle.c_str());
	ofn.lpstrInitialDir = _A2W(inPath.c_str());

	try
	{
		//gsgetfile.dll ���g����Ȃ�g��.
		RAutoLibraryWithException gsgetfile;
		gsgetfile.Create("gsgetfile.dll");
		typedef BOOL (WINAPI *gsGetOpenFileNameDef)(OPENFILENAME* pofn) ;

		gsGetOpenFileNameDef gsGetOpenFileName = (gsGetOpenFileNameDef) gsgetfile.GetProcAddress("gsGetOpenFileName");
		if (! gsGetOpenFileName(&ofn) )
		{
			return "";
		}
		return _W2A(retFilenameBuffer);
	}
	catch(RException)
	{
		//nop
	}

	//gsgetfile ���g���Ȃ��悤�Ȃ̂ŁAOS�̂��g��.
	if (!GetOpenFileName(&ofn)) 
	{
		return "";
	}


	return _W2A(retFilenameBuffer);
#endif
}

//�t�@�C����ۑ��_�C�A���O
string RDialogUtil::FileSaveDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
							 const string & inDefualtFilename ,const TCHAR* inFilter)
{
#ifndef _WIN32_WCE
    OPENFILENAME ofn = { 0 };
	char retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		strcpy(retFilenameBuffer , inDefualtFilename.c_str() );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = inTitle.c_str();
	ofn.lpstrInitialDir = inPath.c_str();

    if (!GetSaveFileName(&ofn)) 
	{
		return "";
	}

	return retFilenameBuffer;
#else
	MY_USES_CONVERSION;

    OPENFILENAME ofn = { 0 };
	TCHAR retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		lstrcpy(retFilenameBuffer , _A2W(inDefualtFilename.c_str()) );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = _A2W(inTitle.c_str());
	ofn.lpstrInitialDir = _A2W(inPath.c_str());

	try
	{
		//gsgetfile.dll ���g����Ȃ�g��.
		RAutoLibraryWithException gsgetfile;
		gsgetfile.Create("gsgetfile.dll");
		typedef BOOL (*gsGetSaveFileNameDef)(OPENFILENAME* pofn) ;
		gsGetSaveFileNameDef gsGetSaveFileName = (gsGetSaveFileNameDef) gsgetfile.GetProcAddress("gsGetSaveFileName");
		if (! gsGetSaveFileName(&ofn) )
		{
			return "";
		}
		return _W2A(retFilenameBuffer);
	}
	catch(RException)
	{
		//nop
	}

	//gsgetfile ���g���Ȃ��悤�Ȃ̂ŁAOS�̂��g��.
    if (!GetSaveFileName(&ofn)) 
	{
		return "";
	}

	return _W2A(retFilenameBuffer);
#endif
}

//�p�X�I���_�C�A���O
string RDialogUtil::SelectPath(HWND inWindowHandle,const string &inTitle,const string &inPath) 
{
#ifndef _WIN32_WCE
	char retPathBuffer[MAX_PATH] = {0};

	bool ret = false;
	IMalloc *m;
	if(SUCCEEDED(SHGetMalloc(&m)))
	{
		BROWSEINFO b;

		b.hwndOwner = inWindowHandle;
		b.pidlRoot = NULL;
		b.pszDisplayName = retPathBuffer;
		b.lpszTitle = inTitle.c_str() ;
		b.ulFlags = BIF_RETURNONLYFSDIRS; //�t�H���_�̂�
		b.lpfn = NULL;
		b.lParam = 0;
		b.iImage = 0;
		LPITEMIDLIST id = SHBrowseForFolder(&b); //�_�C�A���O�̕\��
		if(id)
		{
			SHGetPathFromIDList(id, retPathBuffer); //�p�X���𓾂�

			if (retPathBuffer[lstrlen(retPathBuffer)-1]!='\\') 
			{
				strcat(retPathBuffer,"\\"); //�Ō�� \\ 
			}
			m->Free(id);
		}
		m->Release();
	}
	return retPathBuffer;
#else
	RMessageBox::Message("Wince �ł͂��̋@�\�͎�������Ă��Ȃ��̂Ŏg���܂���B");
	return inPath;
#endif
}

