// XLExceptionUtil.h: XLExceptionUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
#define AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * class XLExceptionUtil:	��O���쐬����w���p�[�֐��W
 *
 * @author 
 */
class XLExceptionUtil  
{
public:
	XLExceptionUtil();
	virtual ~XLExceptionUtil();

	/**
	 * WindowsCodeToString:	�w�肳�ꂽWindows�̃G���[�R�[�h(::GetLastError()) ����{��G���[�ɕϊ�����.
	 *
	 * @param inErrorCode	�G���[�R�[�h(::GetLastError())
	 * @return static string �G���[���b�Z�[�W
	 */
	static string WindowsCodeToString(DWORD inErrorCode);
};

#endif // !defined(AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
