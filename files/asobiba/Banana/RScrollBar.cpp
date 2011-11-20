// RScrollBar.cpp: RScrollBar �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RScrollBar.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RScrollBar::RScrollBar()
{

}

RScrollBar::~RScrollBar()
{

}

void RScrollBar::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{
//	RWindowBase::Create(inINstance,WC_LISTVIEWA,isParentWindow,inID,inFlg,0);
}

//��Ԃ̃Z�b�g.
int RScrollBar::SetScrollInfo(int inFlg ,const SCROLLINFO * inInfo , BOOL inRedraw )
{
	return ::SetScrollInfo( this->getWindowHandle() , inFlg , inInfo , inRedraw );
}

//��Ԃ̎擾.
int RScrollBar::GetScrollInfo(int inFlg ,SCROLLINFO * outInfo ) const
{
	return ::GetScrollInfo( this->getWindowHandle() , inFlg , outInfo );
}

//���݈ʒu�̎擾
int RScrollBar::getPos(int inFlg) const
{
	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_POS;
	GetScrollInfo(inFlg , &info);

	return info.nPos;
}

//�ʒu�̐ݒ�.
void RScrollBar::setPos(int inPos , int inFlg) 
{
	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_POS;
	info.nPos = inPos;
	SetScrollInfo(inFlg , &info , TRUE);
}


//���ۂ̃X�N���[��(WM_VSCROLL / WM_HSCROLL) �����������Ƃ��� �X�N���[���o�[�̈ʒu�𒲐����܂�.
//true ���A���Ă����ꍇ�́A �����ɓ��ꂽ ioTargetScrollInfo �� SetScrollInfo �Őݒ肷��Ƃ����ł��傤.
bool RScrollBar::CalsScroll(SCROLLINFO * ioTargetScrollInfo , WPARAM inWParam) const
{
	int inFlg	=	LOWORD(inWParam);
	int inValue	=	HIWORD(inWParam);

	int val;
	switch(inFlg)
	{
		case SB_LINEUP:
			val = -1;
			break;
		case SB_LINEDOWN:
			val = 1;
			break;
		case SB_PAGEUP:
			val = -1 * ioTargetScrollInfo->nPage;
			break;
		case SB_PAGEDOWN:
			val = ioTargetScrollInfo->nPage;
			break;
		case SB_THUMBTRACK:
			val = inValue - ioTargetScrollInfo->nPos;
			break;
		case SB_TOP:
//		case SB_LEFT:
			val = ioTargetScrollInfo->nMin - ioTargetScrollInfo->nPos;
			break;
		case SB_BOTTOM:
//		case SB_RIGHT:
			val = ioTargetScrollInfo->nMax - ioTargetScrollInfo->nPos;
			break;
		default:
			val = 0;
			break; 
	}
	val = max(-1 * ioTargetScrollInfo->nPos, min(val, ioTargetScrollInfo->nMax - ioTargetScrollInfo->nPos));
	if (val != 0)
	{
		ioTargetScrollInfo->nPos += val;

		return true;
	} 
	return false;
}
	