// RPDFView.cpp: RPDFView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RPDFView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RPDFView::RPDFView()
{

}

RPDFView::~RPDFView()
{
 
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RPDFView::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RActiveXWindow::Create(inINstance,"PDF.PdfCtrl.5" , PdfLib::DIID__DPdf, (void**)&PDF,isParentWindow,0, 0 ,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);

	puts("Control is OK");
}

//�\��.
void RPDFView::Open(const string inFilename)
{
	USES_CONVERSION;

	BSTR a = T2OLE( inFilename.c_str() );
	PDF->LoadFile(a);
}

//�^�C�g�����擾����
string RPDFView::getTitle() const 
{
	return "";
}



