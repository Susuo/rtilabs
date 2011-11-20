// RPDFView.h: RPDFView �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPDFVIEW_H__2F6970C8_6307_4C52_A4D7_42ACE6096C08__INCLUDED_)
#define AFX_RPDFVIEW_H__2F6970C8_6307_4C52_A4D7_42ACE6096C08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RActiveXWindow.h"
#import "C:\Program Files\Adobe\Acrobat 5.0\Reader\ActiveX\pdf.ocx" named_guids


class RPDFView  : public RActiveXWindow
{
public:
	RPDFView();
	virtual ~RPDFView();
	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	
	//�\��.
	void Open(const string inFilename);
	//�^�C�g�����擾����
	string getTitle() const ;
private:
	CComPtr<PdfLib::_DPdf>	PDF;

};

#endif // !defined(AFX_RPDFVIEW_H__2F6970C8_6307_4C52_A4D7_42ACE6096C08__INCLUDED_)
