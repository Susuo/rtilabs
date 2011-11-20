// Filer.cpp: Filer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Filer.h"
#include "RTaskViewWin32.h"
#include "RTaskManager.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Filer::Filer()
{
	Left = NULL;
	Right = NULL;

	LeftWindowFocus = true;	//�Ƃ肠����
}

Filer::~Filer()
{
}

//singleton
Filer* Filer::getFiler() 
{
	static Filer filer;
	return &filer;
}

//�E�B���h�E�����T�C�Y���ꂽ�Ƃ��ɌĂ΂��
void Filer::OnWindowResize()
{
	Left->OnWindowResize();
	Right->OnWindowResize();

	TaskView->OnSize();
}

//�L�[�������ꂽ�Ƃ�
void Filer::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case 'L':
		Left->NewWindow("c:\\WINNT") ;
		break;
	case 'H':
		Left->NewWindow("http://www.yahoo.co.jp/") ;
		break;
	case 'K':
		Left->NewWindow("c:\\test3\\kibafla2.swf") ;
		break;
	case 'J':
		Left->NewWindow("c:\\test3\\01 - ���E��NEOHAPPY.mp3") ;
		break;
	case 'I':
		Left->NewWindow("c:\\test3\\pdftest.pdf") ;
		break;
	case 'U':
		Left->NewWindow("c:\\test3\\�����A���o�� - �c�������.RM") ;
		break;
	case 'Y':
		Left->NewWindow("c:\\test3\\g09.jpg") ;
		break;
	case 'T':
		Left->NewWindow("c:\\test3\\mozilla.txt") ;
		break;
	case 'R':
		Left->NewWindow("\\\\192.168.1.10") ;
		break;
	case 'E':
		Left->NewWindow("smb://") ;
		break;
	case 'W':
		Left->NewWindow("c:\\test3\\�T�|�e�b�N�D��.bmp") ;
		break;
	case VK_F5:
		Left->NewWindow("e:\\down") ;
		break;
	}
}

//�L���ȃE�B���h�E���ǂ������ׂ�.
bool Filer::IsValidWindow(const Window* inWin)
{
	return (Left->Search(inWin) || Right->Search(inWin));
}

//�J�n����
void Filer::OnCreate()
{
	Left = new FilerSub(true);
	Right = new FilerSub(false);
	TaskView = new RTaskViewWin32;
}

//�I������
void Filer::OnDestory()
{
	delete Left;
	delete Right;
	delete TaskView;

	Left = NULL;
	Right = NULL;
	TaskView = NULL;
}

//�����Ƀt�H�[�J�X������܂���?
bool Filer::IsLeftFocus() const
{
	return LeftWindowFocus;
}

//�t�H�[�J�X�����E�B���h�E���ړ������Ƃ��ɌĂ΂��.
void Filer::OnChangeFocus(bool isLeftWindow)
{
	LeftWindowFocus = isLeftWindow;
}

//�t�H�[�J�X���ς�������Ƃ�ʒm.
void Filer::OnChangeFocus()
{
	TaskView->OnSize();
}

