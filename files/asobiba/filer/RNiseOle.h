// RNiseOle.h: RNiseOle �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_)
#define AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNoComplateException.h"
#include "RWin32Exception.h"
#include "RWindow.h"


//Kinder�̃\�[�X���Q�l�ɍ���Ă݂��j�ZOLE�ł��B
//�y����ł���������΍K���ł��B(w
class RNiseOle  : public RWindow
{
public:
	RNiseOle();
	virtual ~RNiseOle();

	//�N��
	void Exec(HINSTANCE inINstance,	HWND isParentWindow,const string inFilename) throw();
	//�^�C�g�����擾����
	string getTitle() const ;
private:
	//�g���q�ɑΉ��������s�t�@�C�����̎擾
	string FindExecutable(const string inFilename) const;

	// EnumWindowsProc�R�[���o�b�N�֐�
	static BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);
private:

	PROCESS_INFORMATION	Procinfo;
	HWND				TargetWindow;
};

#endif // !defined(AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_)
