// RArchiveLha.h: RArchiveLha �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_)
#define AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RArchive.h"

//LHA ���� / ���k���邽�߂̃N���X.
//����ȏ�A�p���֎~
class RArchiveLha : public RArchive  
{
public:
	RArchiveLha();
	virtual ~RArchiveLha();

	//!�������ł��Ȃ������� RDoNotInitaliseException
	//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
	virtual void Create() throw();

	//���݂̃o�[�W������Ԃ��܂��B
	//@return			���݂̃o�[�W����	  1 -> Version 0.01
	//										100 -> Version 1.00
	virtual WORD getVersion() ;

	//���݁A���쒆���ۂ��𓾂܂��B
	virtual bool IsRunning() ;

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
	virtual void Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize) throw();

	//�t�@�C�����������A�[�J�C���ł��邩�ǂ����`�F�b�N
	//@inFilename	�t�@�C����
	//@inMode		�`�F�b�N���[�h
	//@return		������ true
	virtual bool CheckArchive(const char* inFilename,const int inMode) ;

	//�w�肳�ꂽ���Ƀt�@�C���Ɋi�[����Ă���t�@�C�����𓾂܂��B
	//@inFilename	�t�@�C����
	//@return		�t�@�C����
	virtual	int	GetFileCount(const char* inCmdLine) ;

	//�A�[�J�C�����J��
	//@inHWND		�֘A�t����Window
	//@inFilename	�Ώۃt�@�C����
	//@inMode		�J���Ƃ��Ɏg���I�v�V����
	//@return		�A�[�J�C���n���h��
	//!RIOCannotOpenException	�J���܂���.
	virtual HARC OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode) throw();

	//OpenArchive�ŊJ�����A�[�J�C�����N���[�Y����.
	//@inHandle		�A�[�J�C���n���h��
	//!RIOCannotOpenException	�J���܂���.
	virtual void CloseArchive(HARC inHandle) throw();

	//�t�@�C���������J�n
	//ioHandle		OpenArchive�ŊJ�����n���h��
	//@inWildName	��������t�@�C�����@���C���h�J�[�h���i�E�́E�j���I
	//@outSubInfo	����
	//@return		����			true
	//				�����I��		false
	//���s	RArchiveException
	virtual bool FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo) throw();

	//�t�@�C������ 2�߈ȍ~.
	//@ioHandle		OpenArchive�ŊJ�����n���h��
	//@outSubInfo	����
	//@return		����			true
	//				�����I��		false
	//���s	RArchiveException
	virtual bool FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo) throw();

private:
	//DLL���ȒP�Ɏg�����߂̃N���X.
	RAutoLibraryWithException  Lib;

//����ɂ����Ղ� DLL ����C���|�[�g����ɂ��
	//WORD WINAPI UnlhaGetVersion(VOID)
	typedef WORD (WINAPI *UNLHAGETVERSION)    (VOID);
	UNLHAGETVERSION UnlhaGetVersion;

	//BOOL WINAPI UnlhaGetRunning(VOID)
	typedef BOOL (WINAPI *UNLHAGETRUNNING)    (VOID);
	UNLHAGETRUNNING UnlhaGetRunning;

	//BOOL WINAPI UnlhaGetBackGroundMode(VOID)
	typedef BOOL (WINAPI *UNLHAGETBACKGROUNDMODE)    (VOID);
	UNLHAGETBACKGROUNDMODE UnlhaGetBackGroundMode;

	//BOOL WINAPI UnlhaSetBackGroundMode(const BOOL _bBackGroundMode)
	typedef BOOL (WINAPI *UNLHASETBACKGROUNDMODE)    (const BOOL _bBackGroundMode);
	UNLHASETBACKGROUNDMODE UnlhaSetBackGroundMode;

	//BOOL WINAPI UnlhaGetCursorMode(VOID)
	typedef BOOL (WINAPI *UNGETCURSORMODE)    (VOID);
	UNGETCURSORMODE UnlhaGetCursorMode;

	//BOOL WINAPI UnlhaSetCursorMode(const BOOL _CursorMode)
	typedef BOOL (WINAPI *UNLHASETCOUSORMODE)    (const BOOL _CursorMode);
	UNLHASETCOUSORMODE UnlhaSetCursorMode;

	//WORD WINAPI UnlhaGetCursorInterval(VOID)
	typedef WORD (WINAPI *UNLHAGETCURSORINTERNAL)    (VOID);
	UNLHAGETCURSORINTERNAL UnlhaGetCursorInterval;

	//BOOL WINAPI UnlhaSetCursorInterval(const WORD _Interval)
	typedef BOOL (WINAPI *UNLHASETCURSORINTERNAL)    (const WORD _Interval);
	UNLHASETCURSORINTERNAL UnlhaSetCursorInterval;

	//int WINAPI Unlha(const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize)
	typedef int (WINAPI *UNLHA)    (const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize);
	UNLHA Unlha;

	//BOOL WINAPI UnlhaCheckArchive(LPCSTR _szFileName,	const int _iMode)
	typedef BOOL (WINAPI *UNLHACHECKARCHIVE)    (LPCSTR _szFileName,	const int _iMode);
	UNLHACHECKARCHIVE UnlhaCheckArchive;

	//int WINAPI UnlhaGetFileCount(LPCSTR _szArcFile)
	typedef int (WINAPI *UNLHAGETFILECOUNT)    (LPCSTR _szArcFile);
	UNLHAGETFILECOUNT UnlhaGetFileCount;

	//BOOL WINAPI UnlhaConfigDialog(const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode)
	typedef BOOL (WINAPI *UNLHACONFIGDIALOG)    (const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode);
	UNLHACONFIGDIALOG UnlhaConfigDialog;

	//HARC WINAPI UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode)
	typedef HARC (WINAPI *UNLHAOPENARCHIVE)    (const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode);
	UNLHAOPENARCHIVE UnlhaOpenArchive;

	//int WINAPI _export UnlhaCloseArchive(HARC _harc)
	typedef int (WINAPI *UNLHACLOSEARCHIVE)    (HARC _harc);
	UNLHACLOSEARCHIVE UnlhaCloseArchive;

	//int WINAPI UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo)
	typedef int (WINAPI *UNLHAFINDFIRST)    (HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo);
	UNLHAFINDFIRST UnlhaFindFirst;

	//int WINAPI UnlhaFindNext(HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo)
	typedef int (WINAPI *UNLHAFINDNEXT)    (HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo);
	UNLHAFINDNEXT UnlhaFindNext;



};

#endif // !defined(AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_)
