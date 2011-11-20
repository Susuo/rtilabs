// DisplayFactory.cpp: DisplayFactory �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayFactory.h"
#include "DisplayDebugPrintf.h"
#include "DisplayWin32.h"
#include "DisplayBlank.h"
#include "DisplayListViewBigIconWin32.h"
#include "DisplayTextView.h"
#include "DisplayBitmapView.h"
#include "DisplayWebBrowserView.h"
#include "DisplayMediaView.h"
#include "DisplayPDFView.h"
#include "DisplayNiseOLE.h"
#include "RNotSupportException.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DisplayFactory::DisplayFactory()
{

}

DisplayFactory::~DisplayFactory()
{

}

//Display �N���X�̍쐬.
//inDisplayType	�쐬����f�B�X�v���C�̎��
//IsLeft		�����ɔz�u?
//inRfileType	�f�B�X�v���C�ŕ\������ RFILE �̎��
//�T�|�[�g���Ă��Ȃ� inDisplayType ��\������� RNotSupportException ����������B
Display* DisplayFactory::New(__DISPLAYTYPE inDisplayType, Window* inWindow , __RFILETYPE inRfileType)
{
	Display* p = NULL;
	try
	{
		switch(inDisplayType)
		{
		case __DISPLAYTYPE_DEBUG:
			p = new DisplayDebugPrintf(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_BLANK:
			p = new DisplayBlank(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_LISTVIEW_REPORT:
			p = new DisplayWin32(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			p = new DisplayListViewBigIconWin32(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_TEXT:
			p = new DisplayTextView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_BITMAP:
			p = new DisplayBitmapView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_WEBBROWSER:
			p = new DisplayWebBrowserView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_MEDIA:
			p = new DisplayMediaView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_PDF:
			p = new DisplayPDFView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_NISEOLE:
			p = new DisplayNiseOLE(inWindow,inRfileType);
			return p;
		}
	}
	catch(RDoNotInitaliseException e)
	{
		if (p) p->Dec();
		throw ;
	}

	throw RNotSupportException("DisplayFactory::�w�肳�ꂽ�f�B�X�v���C %d �̓T�|�[�g���Ă܂���",inDisplayType);
	return NULL;
}

