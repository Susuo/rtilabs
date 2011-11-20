// RAddressBar.h: RAddressBar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADDRESSBAR_H__62F7F84E_47F4_4E03_919D_784041F389EE__INCLUDED_)
#define AFX_RADDRESSBAR_H__62F7F84E_47F4_4E03_919D_784041F389EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class RAddressBar  ;
#include "comm.h"


#include "FilerSub.h"

class RAddressBar  
{
public:
	RAddressBar(FilerSub* inFilerSub);
	virtual ~RAddressBar();

	//�T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnSize() = 0;
	//�p�X���ύX���ꂽ�Ƃ��ɌĂяo�����.
	virtual void OnAddressChange(string inNewPath) = 0;

protected:
	FilerSub*		FSub;

};

#endif // !defined(AFX_RADDRESSBAR_H__62F7F84E_47F4_4E03_919D_784041F389EE__INCLUDED_)
