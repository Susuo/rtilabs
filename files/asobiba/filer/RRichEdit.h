// RRichEdit.h: RRichEdit �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_)
#define AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RRichEdit;

#include "comm.h"
#include "RWindow.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RNotSupportException.h"
#include "RIOCannotOpenException.h"

//���b�`�G�f�B�b�g�R���g���[���̍쐬.
//�Q�l http://www.kumei.ne.jp/c_lang/sdk2/sdk_130.htm
//     http://www.kumei.ne.jp/c_lang/sdk2/sdk_131.htm
class RRichEdit : public RWindow  
{
public:
	RRichEdit();
	virtual ~RRichEdit();

	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//�ǂݍ���
	//���s RIOCannotOpenException
	void ReadFile(string inFilename) throw();

	//DLL�̓ǂݍ��݁A�G�N�X�|�[�g�𒴊ȒP�ɍs���N���X�B
	static RAutoLibraryWithException AutoLib;
	//�ǂݍ��݂̃R�[���o�b�N.
	static DWORD CALLBACK ReadingProc(DWORD dwCookie, LPBYTE pbBuf, LONG cb, LONG *pcb);
	//�^�C�g�����擾����
	string getTitle() const ;
private:

private:
	HANDLE ReadFileHandle;
};

#endif // !defined(AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_)
