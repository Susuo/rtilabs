// SolvablePathSpecialFolder.cpp: SolvablePathSpecialFolder �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSpecialFolder.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvablePathSpecialFolder::DoSolvable(string inPath) 
{

	//OK
	return true;
}

//��f�B���N�g�����オ��
string SolvablePathSpecialFolder::UpDirectory() const
{
	return "";
}

//�X�y�V�����t�H���_�̖��O�̎擾
//�Q�l http://www.wac-jp.com/programmers/win32/GetSpecialFolder.shtml
//����ɎQ�l http://www.kab-studio.com/Programing/Codian/ShellExtension/06.html
string SolvablePathSpecialFolder::getSpecialFolder( int inFolder )
{
	//�e�t�H���_
	LPSHELLFOLDER	pCurFolder = NULL;
	//�t�H���_�̒��̃f�[�^
	LPITEMIDLIST pItemIDList = NULL;
	//�f�B�X�v���C�ɕ\������閼�O ���ꂪ�߂�l
    string dispName;
	do
	{
		HRESULT	res;
		//���[�g�t�H���_�ł��� �f�B�X�N�g�b�v�̎擾.
		res = SHGetDesktopFolder( &pCurFolder );
		if (res != NOERROR) break;

		//inFolder �ł���������ʂȃt�H���_���@�t�H���_������킷�A�C�e���ɕϊ�
		res = ::SHGetSpecialFolderLocation(NULL, inFolder, &pItemIDList);
		if (res != NOERROR) break;
			
		ASSERT(pItemIDList);

		//����炩�疼�O�����߂�.
		STRRET stFileName;
		res = pCurFolder->GetDisplayNameOf(pItemIDList, SHGDN_NORMAL , &stFileName);
		if (res != NOERROR) break;

		dispName = TranslationFileName(pItemIDList , &stFileName);
	}
	while(0);

	//�������ׁ[
	LPMALLOC pMalloc = NULL;
	::SHGetMalloc(&pMalloc);

	//�t�H���_�̒��̃f�[�^�̉��
	if (pItemIDList)
	{
		pMalloc->Free( pItemIDList );
	}
	//�e�t�H���_�̉��.
	pCurFolder->Release();

	//����������֌W�̉��.
	pMalloc->Release();

    return dispName;
}

string SolvablePathSpecialFolder::
		TranslationFileName(LPITEMIDLIST p_pIDlist, LPSTRRET p_pStrret)
{
	LPMALLOC pMalloc = NULL;
	::SHGetMalloc(&pMalloc);
	string cRetStr;

	switch (p_pStrret->uType) 
	{
	case STRRET_WSTR:
		{
			char dest[MAX_PATH];
			if (::WideCharToMultiByte(CP_OEMCP, 0, p_pStrret->pOleStr , -1, dest, MAX_PATH, NULL, NULL) != 0)
			{
				cRetStr = dest;
			}
			pMalloc->Free(p_pStrret->pOleStr);
		}
		break;

	case STRRET_OFFSET:
		break;

	case STRRET_CSTR:
		break;
	}
	//����������֌W�̉��.
	pMalloc->Release();

	return cRetStr;
}



void SolvablePathSpecialFolder::Test()
{
}


