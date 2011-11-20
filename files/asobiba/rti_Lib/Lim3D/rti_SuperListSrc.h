#ifndef __TSUPER_LIST_SERVER_SRC
#define __TSUPER_LIST_SERVER_SRC

#define __RTI_CHECK

#include <stdio.h>

template <class __ALLOC_DATA>
class __TSuperListServerSrc
{
public:
	__TSuperListServerSrc()
	{
		Init();
	};
	virtual ~__TSuperListServerSrc()
	{
		Clear();
	};

//������.
	virtual void Init()
	{
		//�f�[�^�̐擪.
		m_StartMemory	=	NULL;
		//�f�[�^�̏I���
		m_EndMemory		=	NULL;
		//�f�[�^�̌���. m_Size ���܂łł�.
		m_Size			=	0;
	}
//�T�[�r�X.
	//�j��
	virtual void Release()
	{
		delete this;
	}
	//�f�[�^�̊m��.
	virtual __ALLOC_DATA* New()
	{
		//�f�[�^�̍쐬.
		__ALLOC_DATA*	theData = DataNew();
		if (theData== NULL) return NULL;

		theData->m_Next = NULL;

		if (m_StartMemory == NULL)
		{//�擪���Ȃ�������A�쐬.
			m_StartMemory	=	theData;
			m_EndMemory		=	theData;
			__RTI_CHECK(m_Size == 0);
		}
		else
		{//2���ڈȍ~
			m_EndMemory->m_Next = theData;
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
		}
		//�f�[�^��������.
		m_Size++;

		return theData;
	}
	//�f�[�^�̕����I�ȍ폜.
	virtual void RealDel(__ALLOC_DATA* inData)
	{
		Del(inData);

		DataDelete( inData );
	}
	//�f�[�^�̘_���I�ȍ폜.
	virtual void Del(__ALLOC_DATA* inData)
	{
		//���ʏ���.
		m_Size--;
		if (m_StartMemory == inData)
		{//�ŏ��̃f�[�^.
			if (m_EndMemory == inData)
			{//�ŏ��ł���Ō�ł���.
				__RTI_CHECK(m_Size == 0);
				Init();
			}
			else
			{//���̃f�[�^��擪�ɂ���. ���͕ۏ؂���܂�.
				m_StartMemory = (__ALLOC_DATA*)inData->m_Next;
			}
		}
		else if (m_EndMemory == inData)
		{//�Ō�̃f�[�^�̈�O���Z�o. (��O�����邱�Ƃ͕ۏ؂����.)
			m_EndMemory = GetPrev(m_EndMemory);
		}
		else
		{//�^�[�Q�b�g�̈�O�� �����ۏ؂����.
			((__ALLOC_DATA*)GetPrev(inData))->m_Next = inData->m_Next;
		}
	}
	//���ׂẴf�[�^�̔j��.
	virtual void Clear()
	{
		//�f�[�^.
		__ALLOC_DATA*		theIT;
		//�f�[�^.
		__ALLOC_DATA*		theITNext;
		//�f�[�^�̐�.
		unsigned int		theDataNum;

		theDataNum = m_Size;
		theIT = m_StartMemory;

		//���ׂĔj��.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			theITNext = (__ALLOC_DATA*)theIT->m_Next;

			DataDelete( theIT );

			theIT = theITNext;
		}
		Init();
	}
	//���݂̘_���I�T�C�Y�̎擾.
	virtual unsigned int GetCount(){			return m_Size;	}
	//��O�̃��X�g���擾.
	virtual __ALLOC_DATA* GetPrev(__ALLOC_DATA* inData)
	{
		//�f�[�^.
		__ALLOC_DATA*		theIT;
		//�f�[�^�̐�.
		unsigned int		theDataNum;

		theDataNum = m_Size;
		theIT = m_StartMemory;

		//���ׂĂɖ₢���킹.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			if ( ((__ALLOC_DATA*)(theIT->m_Next)) == inData)
			{
				return theIT;
			}
			theIT = (__ALLOC_DATA*)theIT->m_Next;
		}
		return NULL;
	}
	//�w��ꏊ�̃f�[�^���擾.
	virtual __ALLOC_DATA* At(unsigned int inNo)
	{
		if (inNo >= m_Size) return NULL;

		//�f�[�^.
		__ALLOC_DATA*		theIT;
		//�f�[�^�̐�.
		unsigned int		theDataNum;

		theDataNum = inNo;
		theIT = m_StartMemory;

		//���ׂĔj��.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			theIT = (__ALLOC_DATA*)theIT->m_Next;
		}
		return theIT;
	}

	//�f�o�b�O.
	void Test()
	{
		puts("*******");
		printf("m_StartMemory		%p\r\n",m_StartMemory);
		printf("m_EndMemory			%p\r\n",m_EndMemory);
		printf("m_Size				%u\r\n",m_Size);
		puts("*******");
	}
protected:
	//�f�[�^�m��.
	virtual __ALLOC_DATA* DataNew()
	{
		return new __ALLOC_DATA;
	}
	//�f�[�^�j��.
	virtual  void DataDelete(__ALLOC_DATA* inData)
	{
		delete inData;
	}

	//�f�[�^�̐擪.
	__ALLOC_DATA*		m_StartMemory;
	//�f�[�^�̘_���I�ȏI���(���[�U�[�ɑ΂��Ă��m�点����m�ۂ��Ă����)
	__ALLOC_DATA*		m_EndMemory;
	//�f�[�^�̌���. m_Size ���܂łł�.
	unsigned int		m_Size;
};

#endif
