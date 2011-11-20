// RFModView.cpp: RFModView �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "fmod_define.h"
#include "RFModView.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RFModView::RFModView()
{
}

RFModView::~RFModView()
{
	
}

void RFModView::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RNormalWindow::Create(inINstance,isParentWindow,0,
		WS_CHILD | WS_VISIBLE  ,0);
}


//�T�|�[�g���Ă܂�?
bool RFModView::IsSupport(const string inFilename)
{
	//�Ƃ肠�����A�g���q����.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "MID" || ext == "S3M" || ext == "XM" || ext == "IT" || ext == "RMI" ||
		ext == "SGT" || ext == "WAV" || ext == "MP2" || ext == "MP3" || ext == "OGG" || 
		ext == "WMA" || ext == "ASF" );
}


//�^�C�g���̎擾
string RFModView::getTitle() const 
{
	return "";
}

