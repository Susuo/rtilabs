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
		//�f�[�^�̕����I�ȏI���(���ۂɊm�ۂ��Ă����)
		m_RealMemory	=	NULL;
		//�f�[�^�̘_���I�ȏI���(���[�U�[�ɑ΂��Ă��m�点����m�ۂ��Ă����)
		m_EndMemory		=	NULL;
		//���ۂɊm�ۂ��Ă���T�C�Y.
		m_RealSize		=	0;
		//�f�[�^�̌���. m_Size ���܂łł�.
		m_Size			=	0;
	}
//�T�[�r�X.
	//�j��
	virtual void Release()
	{
		delete this;
	}
	//�����I�ȃf�[�^�ʂ𑝂₷.
	virtual void RealAlloc(unsigned int inAddNum)
	{
		int i = inAddNum;
		//�擪���Ȃ�������A�쐬.
		if (m_StartMemory == NULL)
		{
			m_StartMemory = DataNew();
			m_RealMemory = m_StartMemory;
			m_EndMemory	 = m_StartMemory;
			//����Ō��݂̌�₪1�ւ�����.
			i --;
			//�O�̂��� NULL.
			m_StartMemory->m_Next = NULL;
			m_RealMemory->m_Next = NULL;
			m_EndMemory->m_Next = NULL;
		}

		for(  ; i > 0 ; i --)
		{
			m_RealMemory->m_Next = DataNew();
			m_RealMemory = (__ALLOC_DATA*)m_RealMemory->m_Next;
		}
		//�����I�Ɋm�ۂ��Ă��郁�����𑝂₷.
		m_RealSize += inAddNum;
		//�O�̂��� NULL.
		m_RealMemory->m_Next = NULL;
	}

	//�f�[�^�̊m��.
	virtual __ALLOC_DATA* New()
	{
		//���łɊm�ۍς݂̃�������������...
		if (m_RealMemory != m_EndMemory)
		{
			//���ݎg�p�T�C�Y�𑝂₷.
			m_Size ++;
			//���̃�������. �������������m�ۍς݂ł��邱�Ƃ͕ۏ؂���Ă���.
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
			//���ݏI���̃�������Ԃ�.
			return m_EndMemory;
		}
		//���������̃f�[�^���Ȃ�������...
		if (m_StartMemory == NULL)
		{
			//�d���Ȃ��̂Ńf�[�^���m��.
			RealAlloc(1);

			m_Size = 1;
			return m_StartMemory;
		}
		//���ꂪ�Ō�̃f�[�^�Ȃ�...
		//if (m_RealMemory == m_EndMemory)  << ���̂Ƃ��낱�� if �͕s�v.
		{
			//�d���Ȃ��̂Ńf�[�^���m��.
			RealAlloc(1);
			//���ݎg�p�T�C�Y�𑝂₷.
			m_Size ++;
			//���̃�������. �������������m�ۍς݂ł��邱�Ƃ͕ۏ؂���Ă���.
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
			//���ݏI���̃�������Ԃ�.
			return m_EndMemory;
		}
		//���߂���.
		return NULL;
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
		m_RealSize--;
		m_Size--;
		if (m_RealMemory == inData)
		{//����́A�����I�ɍŌ�̃f�[�^.
			__RTI_CHECK(m_RealSize == 0 && m_Size == 0);
			Init();
		}
		else	if (m_StartMemory == inData)
		{//�ŏ��̃f�[�^.
			if (m_StartMemory == m_EndMemory)
			{//�ŏ��ƍŌオ�������傾����A���� 0 �ɂȂ�.
				__RTI_CHECK(m_Size == 0);
				m_StartMemory = NULL;
				m_EndMemory = NULL;
			}
			else
			{
				//���̃f�[�^��擪�ɂ���. ���͕ۏ؂���܂�.
				m_StartMemory = (__ALLOC_DATA*)inData->m_Next;
			}
		}
		else	if (m_EndMemory == inData)
		{//����͘_���I�ȍŌ�̃f�[�^.
			//�_���I�ɍŌ�̃f�[�^�̈�O���Z�o. (��O�����邱�Ƃ͕ۏ؂����.)
			m_EndMemory = GetPrev(m_EndMemory);
		}
		else
		{
			//�^�[�Q�b�g�̈�O�� �����ۏ؂����.
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

		theDataNum = m_RealSize;
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
	//���݂̕����I�T�C�Y�̎擾.
	virtual unsigned int GetRealCount(){		return m_RealSize;	}
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
		printf("m_RealMemory		%p\r\n",m_RealMemory);
		printf("m_EndMemory			%p\r\n",m_EndMemory);
		printf("m_RealSize			%u\r\n",m_RealSize);
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
	//�f�[�^�̕����I�ȏI���(���ۂɊm�ۂ��Ă����)
	__ALLOC_DATA*		m_RealMemory;
	//�f�[�^�̘_���I�ȏI���(���[�U�[�ɑ΂��Ă��m�点����m�ۂ��Ă����)
	__ALLOC_DATA*		m_EndMemory;
	//���ۂɊm�ۂ��Ă���T�C�Y.
	unsigned int		m_RealSize;
	//�f�[�^�̌���. m_Size ���܂łł�.
	unsigned int		m_Size;
};

#endif
