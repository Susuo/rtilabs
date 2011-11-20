#ifndef __TSUPER_LIST_SERVER
#define __TSUPER_LIST_SERVER

#include "rti_SuperListSrc.h"

class __TSuperListClient;

template <class __ALLOC_DATA>
class __TSuperListServer : public __TSuperListServerSrc<__ALLOC_DATA>
{
public:
	//�f�X�g���N�^
	virtual ~__TSuperListServer(){}
//������.
	virtual void Init()
	{
		m_IsClear = false;
		__TSuperListServerSrc<__ALLOC_DATA>::Init();
	}
//�T�[�r�X.
	//�f�[�^�̘_���I�ȍ폜.
	virtual void Del(__ALLOC_DATA* inData)
	{
		if (m_IsClear == true) return;

		__TSuperListServerSrc<__ALLOC_DATA>::Del(inData);
	}

	//���ׂẴf�[�^�̔j��.
	virtual void Clear()
	{
		m_IsClear = true;

		__TSuperListServerSrc<__ALLOC_DATA>::Clear();

		m_IsClear = false;
	}

	//�ŏ��̃f�[�^�̎擾.
	__ALLOC_DATA* FindFirst()
	{
		m_FindFirstService = m_StartMemory;
		return m_StartMemory;
	}
	//���̃f�[�^�̎擾.
	__ALLOC_DATA* FindNext()
	{
		//���͂Ȃ�����.
		if ( m_FindFirstService == NULL) return NULL;

		if ( m_FindFirstService == m_EndMemory)
		{//���ꂪ�Ō�̃f�[�^.
			m_FindFirstService = NULL;
			return m_FindFirstService;
		}
		
		//���̃f�[�^�̎擾.
		m_FindFirstService = (__ALLOC_DATA*)m_FindFirstService->m_Next;
		return m_FindFirstService;
	}

protected:
	//�f�[�^�m��.
	virtual __ALLOC_DATA* DataNew()
	{
		register __ALLOC_DATA* theRet = new __ALLOC_DATA;
		theRet->Mount(this);
		return theRet;
	}
	//�f�[�^�j��.
	virtual  void DataDelete(__ALLOC_DATA* inData)
	{
		inData->Release();
	}
private:
	//Clear ���������� true
	bool				m_IsClear;
	//FindFirst FindNext �̃��[�N.
	__ALLOC_DATA*		m_FindFirstService;
};
#endif