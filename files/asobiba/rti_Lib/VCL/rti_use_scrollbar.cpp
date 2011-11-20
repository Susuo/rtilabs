/*****************************************************************/
//�X�N���[���o�[��������Ƃ��Ɏg�����
/*****************************************************************/
// rti_vcl_scroll �Ƃ͕ʌɍ��̂́A�X�N���[���o�[�̐�������̖��
// ������ HWND �̈����́A�X�N���[���o�[������E�B���h�̃n���h��
//						 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "StartWithExeFile.h"
#ifdef SCROLLBAR_USE_LIFE
#include <windows.h>
#include <.\VCL\rti_use_scrollbar.h>

/*****************************************************************/
//�X�N���[���o�[�C�x���g
/*****************************************************************/
void ScrollProc(HWND hwnd,int ScrollFlg,
	int wParam,short Range , short *ScreenPos,ScrollFeedBackProc FeedBackProc)
{
	short sUdLr;//����
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:        //�X�N���[�����
			sUdLr = -1;
			break;
		case SB_LINEDOWN:      //�X�N���[�����
			sUdLr = 1;
			break;
		case SB_PAGEUP:        //�X�N���[���V���t�g 
            sUdLr = -10;
			break;
		case SB_PAGEDOWN:      //�X�N���[���V���t�g 
            sUdLr = 10;
			break;
		case SB_THUMBTRACK:    //�X�N���[���܂݂������ꂽ
			sUdLr = 0;
			break;
		case SB_THUMBPOSITION: //�����ꂽ�Ƃ�
            sUdLr = HIWORD(wParam)-*ScreenPos;
			break;
		default:
			sUdLr = 0;
			break;
		}
        sUdLr = max(-(*ScreenPos), min(sUdLr, Range - *ScreenPos ));
        if (sUdLr != 0) {
                *ScreenPos += sUdLr;
                SetScrollPos(hwnd, ScrollFlg, *ScreenPos, TRUE);

				//�t�B�[�h�o�b�N���[�`���ɓ`����
				FeedBackProc(hwnd,ScrollFlg,sUdLr,*ScreenPos);
//                ScrollWindow(hwnd, 0, -sUdLr, NULL, NULL);
//                UpdateWindow(hwnd);
        }
}
 
void ScrollReSize(HWND hwnd,int ScrollFlg,
				int lParam,short *Range , short *ScreenPos)
{
	if (ScrollFlg & SB_VERT)
	{ //�c
	    *Range = GetSystemMetrics(SM_CYSCREEN) - HIWORD(lParam);
	}
	else
	{ //���Ƃ������Ƃɂ���(�蔲��)
	    *Range = GetSystemMetrics(SM_CXSCREEN) - LOWORD(lParam);
	}
    *ScreenPos = min(*ScreenPos, *Range);
    SetScrollRange(hwnd, ScrollFlg, 0, *Range, FALSE);
    SetScrollPos(hwnd, ScrollFlg, *ScreenPos, TRUE);
}

#endif
