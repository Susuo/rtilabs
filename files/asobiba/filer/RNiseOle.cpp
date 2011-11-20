// RNiseOle.cpp: RNiseOle �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNiseOle.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RNiseOle::RNiseOle()
{
	TargetWindow = NULL;
}

RNiseOle::~RNiseOle()
{
	//�c���Ă���悤�ł���΋����I��
	if (::WaitForSingleObject(Procinfo.hProcess, 0) == WAIT_TIMEOUT)
	{
		TerminateProcess(Procinfo.hProcess, 0);
	}
	//�J��.
	::CloseHandle(Procinfo.hProcess);
}

//�N��
//��O RNoComplateException RNoComplateException
void RNiseOle::Exec(HINSTANCE inINstance,HWND isParentWindow ,const string inFilename) throw()
{
	//�N������\���̍쐬.
	string executableCommand = FindExecutable(inFilename);
	string executableCommandLine;
	//EXE��NiseOLE����n���̂��߂�
	if ( stricmp(executableCommand.c_str() , inFilename.c_str() ) != 0)
	{
		executableCommandLine = " \"" +  inFilename + "\"";
	}

	ZeroMemory(&Procinfo,sizeof(Procinfo));

	//�N������܂ő҂�
	//�Q�l http://hp.vector.co.jp/authors/VA016117/waitidle.html
	STARTUPINFO	stinfo;

	stinfo.cb=sizeof(stinfo);
	stinfo.lpReserved=NULL;
	stinfo.lpDesktop=NULL;
	stinfo.lpTitle=NULL;
	stinfo.dwFlags=0;
	stinfo.cbReserved2=0;
	stinfo.lpReserved2=NULL;
	stinfo.wShowWindow = SW_HIDE;

	char commandLine[MAX_PATH * 2];
	strcpy( commandLine , executableCommandLine.c_str() );
	//�J�����g�f�B���N�g���� �N������v���O�����̒����ł��B
	const int lastpos = MultiString::getLastDirectoryPos(executableCommand);
	string currentDir = executableCommand.substr( 0 , lastpos ) ;

	if ( ! CreateProcess(executableCommand.c_str() , commandLine ,NULL,NULL,FALSE,0,NULL,currentDir.c_str() ,&stinfo,&Procinfo) )
	{
		throw RWin32Exception("���s���邱�Ƃ��ł��܂���ł���\n" + executableCommand);
	}
	if ( WaitForInputIdle(Procinfo.hProcess,10000))
	{
		throw RWin32Exception("�N���ł��܂���ł���\n" + executableCommand);
	}

	//�N���������A�v���P�[�V�����̃E�B���h�E�̎擾
	//�Q�l http://techtips.belution.com/ja/vc/0024/
	TargetWindow = NULL;
	EnumThreadWindows(Procinfo.dwThreadId, &RNiseOle::FindWindowProc, (LPARAM)&TargetWindow);
	if (!TargetWindow)
	{
		throw RNoComplateException("�N���������E�B���h�E��������܂���");
	}

	//Kinder�̃\�[�X���Q�l�ɂ��܂����B
	//�e�̕ύX
	SetParent(TargetWindow , isParentWindow);
	//�X�^�C���̕ύX
	DWORD Style;
	Style = GetWindowLong(TargetWindow,GWL_EXSTYLE);
	Style = Style | WS_EX_MDICHILD;
	SetWindowLong(TargetWindow,GWL_EXSTYLE,Style);

	Style = GetWindowLong(TargetWindow,GWL_STYLE);
	Style = Style & (~WS_CAPTION);
	Style = Style & (~WS_THICKFRAME);
	SetWindowLong(TargetWindow,GWL_STYLE,Style);

	//RWindow �ɓo�^.
	MountWindow(TargetWindow);

}

//�g���q�ɑΉ��������s�t�@�C�����̎擾
string RNiseOle::FindExecutable(const string inFilename) const
{
	//�擾�������s����R�}���h��
    char  szExeFilePath[MAX_PATH];
    //  �g���q�ɑΉ��������s�t�@�C�����̎擾
    ::FindExecutable( inFilename.c_str(), NULL, szExeFilePath );

	return szExeFilePath;
}

// EnumWindowsProc�R�[���o�b�N�֐�
BOOL CALLBACK RNiseOle::FindWindowProc(HWND hwnd, LPARAM lParam)
{
    HWND* pphWnd = (HWND*)lParam;
    *pphWnd = hwnd;
    return FALSE; 
}


//�^�C�g�����擾����
string RNiseOle::getTitle() const 
{
	char buf[MAX_PATH];
	::GetWindowText(TargetWindow , buf , MAX_PATH);

	return buf;
}

