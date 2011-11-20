// RArchiveLha.cpp: RArchiveLha �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveLha.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RArchiveLha::RArchiveLha()
{
}

RArchiveLha::~RArchiveLha()
{

}

//!�������ł��Ȃ������� RDoNotInitaliseException
//!�֐����ǂݍ��߂Ȃ������� RNotSupportException
void RArchiveLha::Create() throw()
{
	Lib.Create("UnLha32.dll");

	//����ɂ����Ղ� DLL ����C���|�[�g����ɂ��

	//WORD WINAPI UnlhaGetVersion(VOID)
	UnlhaGetVersion = (UNLHAGETVERSION) Lib.GetProcAddress("UnlhaGetVersion");

	//BOOL WINAPI UnlhaGetRunning(VOID)
	UnlhaGetRunning = (UNLHAGETRUNNING) Lib.GetProcAddress("UnlhaGetRunning");

	//BOOL WINAPI UnlhaGetBackGroundMode(VOID)
	UnlhaGetBackGroundMode = (UNLHAGETBACKGROUNDMODE) Lib.GetProcAddress("UnlhaGetBackGroundMode");

	//BOOL WINAPI UnlhaSetBackGroundMode(const BOOL _bBackGroundMode)
	UnlhaSetBackGroundMode = (UNLHASETBACKGROUNDMODE) Lib.GetProcAddress("UnlhaSetBackGroundMode");

	//BOOL WINAPI UnlhaGetCursorMode(VOID)
	UnlhaGetCursorMode = (UNGETCURSORMODE) Lib.GetProcAddress("UnlhaGetCursorMode");

	//BOOL WINAPI UnlhaSetCursorMode(const BOOL _CursorMode)
	UnlhaSetCursorMode = (UNLHASETCOUSORMODE) Lib.GetProcAddress("UnlhaSetCursorMode");

	//WORD WINAPI UnlhaGetCursorInterval(VOID)
	UnlhaGetCursorInterval = (UNLHAGETCURSORINTERNAL) Lib.GetProcAddress("UnlhaGetCursorInterval");

	//BOOL WINAPI UnlhaSetCursorInterval(const WORD _Interval)
	UnlhaSetCursorInterval = (UNLHASETCURSORINTERNAL) Lib.GetProcAddress("UnlhaSetCursorInterval");

	//int WINAPI Unlha(const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize)
	Unlha = (UNLHA) Lib.GetProcAddress("Unlha");

	//BOOL WINAPI UnlhaCheckArchive(LPCSTR _szFileName,	const int _iMode)
	UnlhaCheckArchive = (UNLHACHECKARCHIVE) Lib.GetProcAddress("UnlhaCheckArchive");

	//int WINAPI UnlhaGetFileCount(LPCSTR _szArcFile)
	UnlhaGetFileCount = (UNLHAGETFILECOUNT) Lib.GetProcAddress("UnlhaGetFileCount");

	//BOOL WINAPI UnlhaConfigDialog(const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode)
	UnlhaConfigDialog = (UNLHACONFIGDIALOG) Lib.GetProcAddress("UnlhaConfigDialog");

	//HARC WINAPI UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode)
	UnlhaOpenArchive = (UNLHAOPENARCHIVE) Lib.GetProcAddress("UnlhaOpenArchive");

	//int WINAPI _export UnlhaCloseArchive(HARC _harc)
	UnlhaCloseArchive = (UNLHACLOSEARCHIVE) Lib.GetProcAddress("UnlhaCloseArchive");

	//int WINAPI UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo)
	UnlhaFindFirst = (UNLHAFINDFIRST) Lib.GetProcAddress("UnlhaFindFirst");

	//int WINAPI UnlhaFindNext(HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo)
	UnlhaFindNext = (UNLHAFINDNEXT) Lib.GetProcAddress("UnlhaFindNext");
}

//���݂̃o�[�W������Ԃ��܂��B
//@return			���݂̃o�[�W����	  1 -> Version 0.01
//										100 -> Version 1.00
WORD RArchiveLha::getVersion() 
{
	return UnlhaGetVersion();
}

//���݁A���쒆���ۂ��𓾂܂��B
bool RArchiveLha::IsRunning() 
{
	return Btob( UnlhaGetRunning() );
}

//�����̎��s
//@inHWND		�֘A�t����WindowHandle;
//@inCmdLine	�R�}���h�̕�����B 
//@outOutput	���ʂ�Ԃ����߂̃o�b�t�@�BLHA.EXE ���R��
//				�\�[���ɕ\������̂Ɠ��l�̂��̂��C���̃o�b�t�@�Ɋi�[��
//				��܂��B
//@inSize		�o�b�t�@�̃T�C�Y�B�Ԃ����ʂ̃T�C�Y���w��
//				���ꂽ�T�C�Y���������ꍇ�͎w��T�C�Y�܂ŏo�͂����B
//				���o�[�W�����ł� 64KB �܂ł̐���������B
//!RArchiveException
void RArchiveLha::Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize) throw()
{
	int ret = Unlha(inHWND,inCmdLine,outOutput,inSize);
	if (ret != 0)
	{
		throw RArchiveException(ret , "RArchiveLha::Command�ŃG���[���������܂����B" );
	}
}

//�t�@�C�����������A�[�J�C���ł��邩�ǂ����`�F�b�N
//@inFilename	�t�@�C����
//@inMode		�`�F�b�N���[�h
//@return		������ true
bool RArchiveLha::CheckArchive(const char* inFilename,const int inMode) 
{
	return Btob( UnlhaCheckArchive(inFilename,inMode) );
}

//�w�肳�ꂽ���Ƀt�@�C���Ɋi�[����Ă���t�@�C�����𓾂܂��B
//@inFilename	�t�@�C����
//@return		�t�@�C����
int	RArchiveLha::GetFileCount(const char* inCmdLine) 
{
	return UnlhaGetFileCount(inCmdLine);
}

//�A�[�J�C�����J��
//@inHWND		�֘A�t����Window
//@inFilename	�Ώۃt�@�C����
//@inMode		�J���Ƃ��Ɏg���I�v�V����
//@return		�A�[�J�C���n���h��
//!RIOCannotOpenException	�J���܂���.
HARC RArchiveLha::OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode)  throw()
{
	HARC handle = UnlhaOpenArchive(inHWND,inFilename,inMode);
	if (handle == NULL)
		throw RIOCannotOpenException("RArchiveLha::�A�[�J�C�� %s ���J�����Ƃ��ł��܂���ł���",inFilename);
	return handle;
}

//OpenArchive�ŊJ�����A�[�J�C�����N���[�Y����.
//@inHandle		�A�[�J�C���n���h��
//!RIOCannotCloseException	�J���܂���.
void RArchiveLha::CloseArchive(HARC inHandle) 
{
	if ( UnlhaCloseArchive(inHandle) != 0)
		throw RIOCannotCloseException("RArchiveLha::�N���[�Y�Ɏ��s���܂���");
}

//�t�@�C���������J�n
//ioHandle		OpenArchive�ŊJ�����n���h��
//@inWildName	��������t�@�C�����@���C���h�J�[�h���i�E�́E�j���I
//@outSubInfo	����
//@return		����			true
//				�����I��		false
//���s	RArchiveException
bool RArchiveLha::FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo)  throw()
{
	int ret = UnlhaFindFirst(ioHandle,inWildName,outSubInfo) ;
	switch(ret)
	{
	case 0:
		return true;
	case -1:
		return false;
	default:
		throw RArchiveException(ret,"RArchiveLha::FindFirst�G���[���������܂���");
	}
	return false;
}

//�t�@�C������ 2�߈ȍ~.
//@ioHandle		OpenArchive�ŊJ�����n���h��
//@outSubInfo	����
//@return		����			true
//				�����I��		false
//���s	RArchiveException
bool RArchiveLha::FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo)  throw()
{
	int ret = UnlhaFindNext(ioHandle,outSubInfo) ;
	switch(ret)
	{
	case 0:
		return true;
	case -1:
		return false;
	default:
		throw RArchiveException(ret,"RArchiveLha::FindFirst�G���[���������܂���");
	}
	return false;
}
