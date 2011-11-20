// RRichEdit.cpp: RRichEdit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RRichEdit.h"
#include "richedit.h"

RAutoLibraryWithException RRichEdit::AutoLib;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RRichEdit::RRichEdit()
{
	this->ReadFileHandle = NULL;
}

RRichEdit::~RRichEdit()
{
	if (this->ReadFileHandle) ::CloseHandle(this->ReadFileHandle);
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RRichEdit::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	if ( ! AutoLib.IsReading() )
	{
		AutoLib.Create("RICHED32.DLL");
	}

	RWindow::Create(inINstance,"RICHEDIT",isParentWindow,inID,inFlg,0);

}

//�ǂݍ���.
//���s RIOCannotOpenException
void RRichEdit::ReadFile(string inFilename) throw()
{
	if (this->ReadFileHandle) throw RIOCannotOpenException("���ł� �t�@�C���͊J����Ă��܂��B");

	this->ReadFileHandle = ::CreateFile(inFilename.c_str() , GENERIC_READ | GENERIC_WRITE , 0 ,NULL ,OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);
	if (this->ReadFileHandle == NULL) throw RIOCannotOpenException("�t�@�C�� %s ���J���܂���ł���" , inFilename);

	EDITSTREAM eds = {0};
	eds.dwCookie = (DWORD)this->ReadFileHandle;
	eds.dwError = 0;
	eds.pfnCallback = ReadingProc;

	::SendMessage(this->getWindowHandle() , EM_STREAMIN, SF_TEXT, (LPARAM)&eds);
}

DWORD CALLBACK RRichEdit::ReadingProc(DWORD dwCookie, LPBYTE pbBuf, LONG cb, LONG *pcb)
{
	::ReadFile( (HANDLE)dwCookie, pbBuf, cb, (LPDWORD)pcb, NULL);
	return FALSE; 
}


//�^�C�g�����擾����
string RRichEdit::getTitle() const 
{
	return "";
}