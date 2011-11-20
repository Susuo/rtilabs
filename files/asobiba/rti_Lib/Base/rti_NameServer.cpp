#include <./Base/rti_NameServer.h>

TStringHashTable::TStringHashTable()
{
	memset( m_HashTable , 0 , sizeof(TStringHashData*) * TSTRING_HASH_TABLE_MAX );
}

TStringHashTable::~TStringHashTable()
{
	Release();
}


/*********************************************************/
//�f�[�^��ǉ�����.
//����				inKeyWord			�ǉ�����L�[���[�h
//					inData				�ǉ�����f�[�^
//					inForce				�����L�[���������ꍇ�̓L�[���[�h%d �ł͂ߍ���
//�߂�l			���� = true
/*********************************************************/
bool TStringHashTable::Add(const char * inKeyWord , void* inData , bool inForce/*=false*/)
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;
	TStringHashData**	theOldSHD;

	//�n�b�V���l�����߂�.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//�n�b�V���e�[�u������������.
	theSHD = m_HashTable[theDataPos];
	theOldSHD = &m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{//�����L�[�̃f�[�^�͂�����.
			if ( theSHD->Cmp(inData) )
			{//�f�[�^��������!!
				theSHD->ReferenceCountUp();
				return true;
			}
			return false;
		}
		theOldSHD = theSHD->GetNextPointer();
		theSHD = theSHD->GetNext() ;
	}
	//�V�K�쐬.
	theSHD		= new TStringHashData;
	theSHD->Set(inKeyWord , theLen , inData );

	*theOldSHD	= theSHD;

	return true;
}

/*********************************************************/
//�f�[�^����������.
//����				inKeyWord			��������L�[���[�h
//�߂�l			���������f�[�^	���s=NULL
/*********************************************************/
void* TStringHashTable::Get(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//�n�b�V���l�����߂�.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//�n�b�V���e�[�u������������.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//���̃f�[�^��Ԃ�.
			return theSHD->GetData();
		}
		theSHD = theSHD->GetNext() ;
	}
	return NULL;
}

/*********************************************************/
//�f�[�^����������.
//����				inKeyWord			��������L�[���[�h
//�߂�l			���������f�[�^	���s=NULL
/*********************************************************/
void** TStringHashTable::GetPointer(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//�n�b�V���l�����߂�.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//�n�b�V���e�[�u������������.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//���̃f�[�^��Ԃ�.
			return theSHD->GetDataPointer();
		}
		theSHD = theSHD->GetNext() ;
	}
	return NULL;
}

/*********************************************************/
//�f�[�^��ύX����
//����				inKeyWord			�X�V����L�[���[�h
//					inData				�X�V����f�[�^
//�߂�l			���� = true
/*********************************************************/
bool TStringHashTable::Change(const char * inKeyWord , void* inData)
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//�n�b�V���l�����߂�.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//�n�b�V���e�[�u������������.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//���̃f�[�^������������.
			theSHD->ReNew(inData);
			return true;
		}
		theSHD = theSHD->GetNext() ;
	}
	return false;
}

/*********************************************************/
//�f�[�^���폜����
//����				inKeyWord			�폜����L�[���[�h
//�߂�l			���� = true
/*********************************************************/
bool TStringHashTable::Delete(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData**	theOldSHD;
	TStringHashData*	theSHD;

	//�n�b�V���l�����߂�.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//�n�b�V���e�[�u������������.
	theSHD = m_HashTable[theDataPos];
	theOldSHD = & ( m_HashTable[theDataPos] );

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//���̃f�[�^������.
			theSHD->Release(theOldSHD);
			return true;
		}
		theOldSHD = theSHD->GetNextPointer();
		theSHD = theSHD->GetNext() ;
	}
	return false;
}

/*********************************************************/
//�n�b�V���e�[�u���̔j��
//����				�Ȃ�.
//�߂�l			���� = true
/*********************************************************/
bool TStringHashTable::Release()
{
	TStringHashData*	theSHD;
	int					theI ;
	for( theI = 0 ; theI < TSTRING_HASH_TABLE_MAX ; theI++)
	{
		theSHD = m_HashTable[theI];
		if (theSHD != NULL)	theSHD->ReleaseAll();
	}

	memset( m_HashTable , 0 , sizeof(TStringHashData*) * TSTRING_HASH_TABLE_MAX );
	return true;
}

/*********************************************************/
//�n�b�V�������l��Ԃ�.
//����				inKeyWord		�n�b�V������L�[���[�h
//					inLen			�L�[���[�h�̒���.
//�߂�l			�n�b�V���l.
/*********************************************************/
unsigned int  TStringHashTable::GetDataPos(const char * inKeyWord , int inLen)
{
	unsigned int		theHash = 0;
	char*	theKeyWordPoint = (char*)inKeyWord;

	int theI;
	for(theI = 0 ; theI < inLen ; theI++) theHash += *(theKeyWordPoint++);

	return theHash % TSTRING_HASH_TABLE_MAX;
}




