// MVControl.h: MVControl �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_)
#define AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MVFileList.h"
#include "MVThreadManager.h"
#include "RWaitCursor.h"

class MVControl  
{
public:
	MVControl();
	virtual ~MVControl();

	/**
	 * ChangeOption:	��ʂ̕��ƍ���
	 *
	 * @param inImageConvertParam	�ϊ��Ɏg�p����p�����[�^
	 * @return void 
	 */
	void ChangeOption(const MVImagePreConvert* inImageConvertParam)
	{
		this->ImageConvertParam = *inImageConvertParam;

		this->Manager.ChangeOption( & this->ImageConvertParam );
	}

	/**
	 * Update:			�{���J��
	 *
	 * @param inBookFilename	book.zip �t�@�C��
	 * @param inPage			�y�[�W
	 * @return void 
	 */
	void Update( const string & inBookFilename , int inPage) throw (RException)
	{
		if (inBookFilename.empty() ) return;

		this->Manager.Update(inBookFilename);
		GotoPage(inPage);
	}

	/**
	 * GotoPage:			�w��y�[�W�ɃW�����v
	 *
	 * @param inPage 
	 * @return void 
	 */
	void GotoPage(int inPage) throw (RException);

	/**
	 * getPageCount:		���y�[�W��
	 *
	 * @return int 
	 */
	int getPageCount()
	{
		return this->Manager.getPageCount();
	}

	/**
	 * getNowPage:		���̃y�[�W��
	 *
	 * @return int 
	 */
	int  getNowPage() const
	{
		return this->Page;
	}

	/**
	 * Scroll:		�y�[�W���X�N���[������
	 *
	 * @param inUp	ture	��
	 *				false	��
	 * @return void 
	 */
	void Scroll(bool inUp) throw (RException);

	/**
	 * Draw:				�`��
	 *
	 * @param ioHDC 
	 * @return void 
	 */
	void Draw(HDC ioHDC) ;


private:
	MVThreadManager		Manager;
	const MVPageData*			PageData;

//	int			ScrollWidth;
//	int			Width;
//	int			Height;
	int			Page;
//	bool		ScreenType;

	int			ScrollXY;

	MVImagePreConvert	ImageConvertParam;
};

#endif // !defined(AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_)
