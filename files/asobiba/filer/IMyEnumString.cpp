// IMyEnumString.cpp: IMyEnumString �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "IMyEnumString.h"
#include <shlobj.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

IMyEnumString::IMyEnumString(LPUNKNOWN inUnkRef,WStringList inTargetStrings)
{
	m_cRef = 0;
	m_pUnkRef = inUnkRef;

	m_iCur = 0;
	TargetStrings = inTargetStrings;

	//IMalloc �m��
	SHGetMalloc(&m_pmem);
}

IMyEnumString::~IMyEnumString()
{
	//IMalloc ���
	m_pmem->Release();
	return;
}

STDMETHODIMP IMyEnumString::QueryInterface(REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	if (IID_IUnknown == riid || IID_IEnumString == riid)
		*ppv = (LPVOID)this;

	if (*ppv)
	{
		((LPUNKNOWN)*ppv)->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) IMyEnumString::AddRef(void)
{
	// ���̃N���X�̎Q�ƃJ�E���g���C���N�������g����
	// �e�N���X������΂�����C���N�������g
	++m_cRef;
	if(m_pUnkRef) 
		m_pUnkRef->AddRef();
	return m_cRef;
}

STDMETHODIMP_(ULONG) IMyEnumString::Release(void)
{
	//IMalloc �����.
	if (m_pmem)
		m_pmem->Release();

	// ���̃I�u�W�F�N�g�����
	// �e������΂�������
	if(m_pUnkRef) 
		m_pUnkRef->Release();

	if (0L != --m_cRef)
		return m_cRef;

	delete this;
	return 0;
}



/*
 * IEnumString::Next
 *
 * �ړI -> ���̃A�C�e����n��
 *
 * ����:
 *  cstr    ULONG      pstr�ɕԂ�������̌�
 *  pstr    LPOLESTR*  �߂�l������|�C���^
 *  pulstr  ULONG*     ���ۂɕԂ�����������ϐ��ւ̃|�C���^
 *
 * �߂�l:
 *          HRESULT    ������		S_OK
 *                     ���s��		S_FALSE
 */
STDMETHODIMP IMyEnumString::Next(ULONG cstr, LPOLESTR *pstr, ULONG *pcstr)
{
	ULONG cReturn = 0L;
	ULONG maxcount = cstr;

	*pcstr = 0L;	// ���߂�l��������
	*pstr = NULL;	// ��

	// ���̃N���X��������������ĂȂ������玸�s
	if (TargetStrings.empty() )
		return S_FALSE;

	// pcstr��NULL��������A�P�̃A�C�e�����擾���ȊO�͎��s
	if (!pcstr)
	{
		if (cstr != 1L)
			return E_POINTER;
	}

	int stringCount = TargetStrings.size() ;

	// ���X�g�̍Ōゾ�����玸�s
	if (m_iCur >= stringCount )
		return S_FALSE;

	// �v���������܂ŕԂ��邾���Ԃ�
	while (m_iCur < stringCount && cstr > 0)
	{
		int	size;

		// �R�s�[���镶����̕��������v�Z
		size = (wcslen( TargetStrings[m_iCur].c_str() )+1);

		// �������m��
		pstr[cReturn] = (WCHAR*)m_pmem->Alloc(size * sizeof(WCHAR));
		if(pstr[cReturn])
			wcscpy(pstr[cReturn], TargetStrings[m_iCur].c_str() );

		// ���̃A�C�e���Ɉڂ�
		m_iCur++;
		cReturn++;
		cstr--;
	}

	if (pcstr)
		*pcstr = cReturn;

	if (cReturn == maxcount)
		return S_OK;
	return S_FALSE;
}

/*
 * IEnumString::Skip
 *
 * �ړI -> �A�C�e����n��΂�
 *
 * ����:
 *  cSkip  ULONG  ��΂���
 *
 * �߂�l:
 *  HRESULT	 ������                        S_OK
 *			 �v�������������Ƃ΂��Ȃ��Ƃ�  S_FALSE
 */
STDMETHODIMP IMyEnumString::Skip(ULONG cSkip)
{
	if (((m_iCur + cSkip) >= TargetStrings.size() ) || TargetStrings.empty() )
		return S_FALSE;

	m_iCur += cSkip;
	return S_OK;
}

/*
 * IEnumString::Reset
 *
 * �ړI -> ���݈ʒu���ŏ��ɖ߂�
 *
 * ���� -> �Ȃ�
 */
STDMETHODIMP IMyEnumString::Reset(void)
{
	m_iCur = 0;
	return S_OK;
}

/*
 * IEnumString::Clone
 *
 * �ړI -> ����Ɠ�����Ԃ�IEnumString�����
 *
 * ����:
 *  ppEnum LPENUMSTRING* ������V�����I�u�W�F�N�g�̃|�C���^���󂯎��ϐ�
 */
STDMETHODIMP IMyEnumString::Clone(LPENUMSTRING *ppEnum)
{
	*ppEnum = NULL;

	// �����̂����
	IMyEnumString	*pNew = new IMyEnumString(m_pUnkRef, TargetStrings);

	if (!pNew)
		return E_OUTOFMEMORY;

	// �Q�ƃJ�E���g�𑝂₷
	pNew->AddRef();

	// �ʒu�����낦��
	pNew->m_iCur = m_iCur;

	*ppEnum = pNew;
	return S_OK;
}
