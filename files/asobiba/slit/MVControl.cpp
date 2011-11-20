// MVControl.cpp: MVControl �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVControl.h"
#include "MVConfig.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MVControl::MVControl()
{
	this->Page = 0;
	this->PageData = NULL;
}

MVControl::~MVControl()
{
}

/**
 * GotoPage:			�w��y�[�W�ɃW�����v
 *
 * @param inPage 
 * @return void 
 */
void MVControl::GotoPage(int inPage) throw (RException)
{
//		RWaitCursor waitCursor;

	int a = getPageCount();
	if (inPage >= getPageCount()  || inPage < 0)
	{
		return ;
	}
	//�y�[�W�����[�h
	this->PageData = this->Manager.GotoPage(inPage);

	this->Page = inPage;
	this->ScrollXY = 0;
}


/**
 * Scroll:		�y�[�W���X�N���[������
 *
 * @param inUp	ture	��
 *				false	��
 * @return void 
 */
void MVControl::Scroll(bool inUp) throw (RException)
{
	if (!  this->PageData )
	{
		//�܂��A�y�[�W���ǂݍ��܂�Ă��Ȃ�.
		return ;
	}

	int scrollStop = 0;
	switch ( this->ImageConvertParam.getScrollType() )
	{
	case MVConfig::YOKO:
	default:
		scrollStop = this->PageData->getWidth() - this->ImageConvertParam.getScreenWidth();
		break;
	case MVConfig::TATE:
		scrollStop = this->PageData->getHeight() - this->ImageConvertParam.getScreenHeight();
		break;

	}

	const int scrollCount = MVConfig::getInstance()->getScrollCount() ;
	const int scroolSize = abs( (scrollStop) / scrollCount) ;

	if (inUp)
	{
		if (this->ScrollXY <= 0)
		{
			//���łɈ�ԏ�̏ꍇ�A��O�̃y�[�W�ɖ߂�.
			GotoPage(getNowPage() - 1);
			this->ScrollXY = scroolSize * scrollCount ;
		}
		else
		{
			this->ScrollXY -= scroolSize;
			if (this->ScrollXY <= scrollCount)
			{//�␳
				this->ScrollXY =0;
			}
		}
	}
	else
	{
		if (this->ScrollXY >= scrollStop )
		{
			//���łɈ�ԉ��̏ꍇ�A����̃y�[�W�ɐi��.
			GotoPage(getNowPage() + 1);
		}
		else
		{
			this->ScrollXY += scroolSize;
			if (this->ScrollXY >= (scrollStop - scrollCount) )
			{//�␳
				this->ScrollXY = scrollStop;
			}
		}
	}
}

/**
 * Draw:				�`��
 *
 * @param ioHDC 
 * @return void 
 */
void MVControl::Draw(HDC ioHDC) 
{
	if (!  this->PageData )
	{
		//�܂��A�y�[�W���ǂݍ��܂�Ă��Ȃ�.
		return ;
	}

	switch ( this->ImageConvertParam.getScrollType() )
	{
	case MVConfig::YOKO:
	default:
		BitBlt(ioHDC , 0 , 0 , this->ImageConvertParam.getScreenWidth() , this->ImageConvertParam.getScreenHeight() ,
					this->PageData->getDC() , this->ScrollXY , 0 , SRCCOPY);
		break;
	case MVConfig::TATE:
		BitBlt(ioHDC , 0 , 0 , this->ImageConvertParam.getScreenWidth() , this->ImageConvertParam.getScreenHeight() ,
					this->PageData->getDC() , 0 , this->ScrollXY , SRCCOPY);
		break;

	}
}

