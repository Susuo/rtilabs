// IMyEnumString.h: IMyEnumString �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_)
#define AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include <shldisp.h>

class IMyEnumString : public IEnumString  
{
public:
	IMyEnumString(LPUNKNOWN inUnkRef, WStringList inTargetStrings);
	~IMyEnumString();

	// IUnknown�C���^�[�t�F�C�X�̃��\�b�h
	STDMETHODIMP         QueryInterface(REFIID iid, LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// IEnumString�C���^�[�t�F�C�X�̃��\�b�h
	STDMETHODIMP         Next(ULONG cstr, LPOLESTR *rgelt, ULONG *pcstr);
	STDMETHODIMP         Skip(ULONG celt);
	STDMETHODIMP         Reset(void);
	STDMETHODIMP         Clone(IEnumString **ppenum);

private:
	ULONG           m_cRef;     // �Q�ƃJ�E���g
	LPUNKNOWN       m_pUnkRef;  // �Q�ƃJ�E���g�𑝂₷IUnknown�̃|�C���^
	ULONG           m_iCur;     // ���݂̃��X�g�ʒu
	WStringList		TargetStrings;
	IMalloc			*m_pmem;	// IMalloc�C���^�[�t�F�C�X�̃|�C���^
};

#endif // !defined(AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_)
