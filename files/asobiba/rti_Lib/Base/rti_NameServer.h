#include <string.h>

#define TSTRING_HASH_TABLE_MAX					127

class TStringHashTable
{

	class TStringHashData
	{
	public:
		TStringHashData()
		{
			//�L�[���[�h������
			m_KeyWord			= NULL;
			//�L�[���[�h�̒���
			m_KeyWordLen		= 0;
			//�ێ����Ă���f�[�^
			m_Data				= NULL;;
			//���̃f�[�^���Q�Ƃ��Ă��鐔.
			m_ReferenceCount	= 0;
			//�V�m�j�����R�[�h�̏ꍇ�̃��X�g.
			m_Next				= NULL;
		}
		~TStringHashData()
		{
			if (m_KeyWord) delete [] m_KeyWord;
		}
	public:
		//�L�[���[�h�ƈ�v���邩  (��v = true)
		bool	Cmp(const char* inChallengerString , int inChallengerStringLen )
		{
			return (inChallengerStringLen == m_KeyWordLen &&
					memcmp(inChallengerString , m_KeyWord , inChallengerStringLen) == 0);
		}
		//�f�[�^�ƈ�v���邩  (��v = true)
		bool	Cmp(void* inChallengerData )
		{
			return (inChallengerData == m_Data);
		}
		//�f�[�^�̊i�[.
		void	Set(const char* inKeyWord , int inKeyWordLen , void* inData )
		{
			if (m_ReferenceCount == 0)
			{	//������m��.
				m_KeyWord = new char[inKeyWordLen];
				memcpy(m_KeyWord , inKeyWord , inKeyWordLen);
				m_KeyWordLen	=	inKeyWordLen;
				m_Data			=	inData;
			}
			m_ReferenceCount++;
		}
		//�V���ɋ��L�ł��郌�R�[�h��������.
		void	ReferenceCountUp(){ m_ReferenceCount++; }
		//�ێ����Ă���f�[�^�̏�������.
		void	ReNew(void* inData){ m_Data = inData;}
		//�ێ����Ă���f�[�^��Ԃ�.
		void*	GetData(){ return m_Data;}
		//�ێ����Ă���f�[�^��Ԃ�.
		void**	GetDataPointer(){ return &m_Data;}
		//�f�[�^�̍폜.
		void	Release(TStringHashData** inNextPointer)
		{
			m_ReferenceCount--;
			if (m_ReferenceCount <= 0)
			{
				if (inNextPointer != NULL)
				{
					//��O�̂�� Next �̏�������(�댯�ȃv���O�����ł��߂�Ȃ���).
					*inNextPointer = m_Next;
				}
				delete this;
			}
		}
		//����ȍ~�̃f�[�^�̖ⓚ���p�ō폜.
		void	ReleaseAll()
		{
			if ( m_Next != NULL ) m_Next->ReleaseAll();
			delete this;
		}
		//���� Next ���擾.
		TStringHashData*	GetNext(){		return m_Next;	}
		TStringHashData**	GetNextPointer(){return &m_Next;}
	protected:
		//�L�[���[�h������
		char*	m_KeyWord;
		//�L�[���[�h�̒���
		int		m_KeyWordLen;
		//�ێ����Ă���f�[�^
		void*	m_Data;
		//���̃f�[�^���Q�Ƃ��Ă��鐔.
		int		m_ReferenceCount;
		//�V�m�j�����R�[�h�̏ꍇ�̃��X�g.
		TStringHashData*	m_Next;
	};


public:
	TStringHashTable();
	~TStringHashTable();
public:		//�f�[�^����.

	//�f�[�^��ǉ�����.
	bool Add(const char * inKeyWord , void* inData , bool inForce=false);
	//�f�[�^����������.
	void* Get(const char * inKeyWord );
	//�f�[�^����������.
	void** GetPointer(const char * inKeyWord );
	//�f�[�^��ύX����
	bool Change(const char * inKeyWord , void* inData);
	//�f�[�^���폜����
	bool Delete(const char * inKeyWord );
	//�n�b�V���e�[�u���̔j��
	bool Release();

protected:	//�n�b�V������.
	//�n�b�V�������l��Ԃ�.
	unsigned int  GetDataPos(const char * inKeyWord , int inLen);
	
protected:
	TStringHashData*		m_HashTable[TSTRING_HASH_TABLE_MAX];
};
