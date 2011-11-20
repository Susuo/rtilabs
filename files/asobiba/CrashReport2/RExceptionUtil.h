// RExceptionUtil.h: RExceptionUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
#define AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>
#include <atlbase.h>


/**
 * class RExceptionUtil:	��O���쐬����w���p�[�֐��W
 *
 * @author 
 */
class RExceptionUtil  
{
public:
	RExceptionUtil();
	virtual ~RExceptionUtil();

	/**
	 * WindowsCodeToString:	�w�肳�ꂽWindows�̃G���[�R�[�h(::GetLastError()) ����{��G���[�ɕϊ�����.
	 *
	 * @param inErrorCode	�G���[�R�[�h(::GetLastError())
	 * @return static string �G���[���b�Z�[�W
	 */
	static string WindowsCodeToString(DWORD inErrorCode);

	/**
	 * RExceptionUtil:	COM �̃G���[���b�Z�[�W�ɕϊ�����
	 *
	 * @param inComError	�G���[(::GetLastError())
	 * @return static string �G���[���b�Z�[�W
	 */
	string RExceptionUtil::ComErrorToString(_com_error &inComError);
};

#endif // !defined(AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
