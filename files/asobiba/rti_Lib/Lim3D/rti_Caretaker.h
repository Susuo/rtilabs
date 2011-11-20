#ifndef	___CARETAKER__H	//�d���h�~
#define ___CARETAKER__H

/**********************************************************/
//�R���N�V�����e���v���[�g�N���X (C)Copylight R.T.I since 2000...
/**********************************************************/
#include <windows.h>
#include <./Base/rti_debug.h>
#include <./rti_define.h>

//�R���N�^�[�e���v���[�g�N���X.
template<class _Caretation , class InputData = _Caretation >
class _TCaretaker
{
public:
	/*********************/
	//���\�b�h
	/*********************/
	//�R���X�g���^
	_TCaretaker()
	{
		m_Caretation = NULL;
		m_CaretationNubmer = 0;
		m_CaretationRealNumber = 0;
	}

	//�f�X�g���N�^
	~_TCaretaker()	{		Clear();	}

	//�V�K�Ƀf�[�^���Z�b�g����.
	bool			Set(InputData* _c ,			
						int Many , bool SizeNotChange = true)
	{
		//�������}�C�i�X�͖v.
		if (Many <= 0) return false;

		//�̈�̊m��
		Alloc(Many,SizeNotChange);
		//���̗̈�Ƀf�[�^��]�����܂�.
		CopyData( m_Caretation , _c , Many);
		//�f�[�^�̌������X�V����.
		m_CaretationNubmer = Many;
		return true;
	}
	//�V�K�ǉ�.
	bool			Add(InputData* _c ,			
						int Many , bool SizeNotChange = true)
	{
		//�������}�C�i�X�͖v.
		if (Many <= 0) return false;

		//���݂̗̈���ێ����Ȃ���A�V�K�̈�m��.
		Alloc(m_CaretationNubmer+Many,SizeNotChange,false);
		//���̗̈�Ƀf�[�^��]�����܂�.
		CopyData( m_Caretation+m_CaretationNubmer , _c , Many);
		//�f�[�^�̌������X�V����.
		m_CaretationNubmer = m_CaretationNubmer+Many;
		return true;
	}

	//�V�K�폜
	bool			Del(int Start , int Count = 1)
	{
		int i;
		//�͈͊O�̍폜�͖v.
		if ( Start+Count  <= m_CaretationNubmer) return false;
		//�p�����[�^�̓}�C�i�X�͋֎~.
		if ( Start < 0  || Count < 0) return false;
		
		//�]��
		for( i = Start+Count ; i < m_CaretationNubmer ; i++)
		{
			m_Caretation[ i ] = m_Caretation[ i+Count ];
		}
		//�f�[�^�̌������X�V����.
		m_CaretationNubmer = m_CaretationNubmer-Count;
		return true;
	}
	
	//�̈�m��
	bool			Alloc(int Many , bool SizeNotChange = true , bool ClearAlloc = true)
	{
		if (m_Caretation == NULL)
		{	//���߂Ă̎g�p�ł�.	�̈���m�ۂ��܂�.
			m_Caretation = (_Caretation*)::malloc(sizeof(_Caretation) * (Many + 1) );
			m_CaretationRealNumber = Many;	//���ۊm�ۂ��Ă���̈�̍X�V.
		}
		else
		{	//m_Caretation �ɗ̈�����łɊm�ۂ��Ă���ꍇ.
			if (SizeNotChange == true)
			{	//�҂������m�ۂ��Ȃ��Ă�����.
				if (m_CaretationRealNumber < Many)
				{	//�m�ۂ����������̕����A���݊m�ۂ��Ă���̈��菬�����ꍇ.�Ċm��.
					NewAlloc(Many ,SizeNotChange ,  ClearAlloc);
				}
			}
			else
			{	//�҂�����Ɋm�ۂ��Ȃ��Ă͂Ȃ�Ȃ��ꍇ.
				if (m_CaretationRealNumber != Many)
				{	//���݊m�ۂ��Ă��鐔�Ɗm�ۂ������̈悪��������ł͂Ȃ��ꍇ�A�Ċm��.
					NewAlloc(Many ,SizeNotChange ,  ClearAlloc);
				}
			}
		}
		return true;
	}

	//�̈����.
	void			Clear()						
	{
		if (m_Caretation != NULL)	::free(m_Caretation);
		m_Caretation = NULL;
		m_CaretationNubmer = 0;
		m_CaretationRealNumber = 0;
	}

	//�j��
	void			Release()	{	delete this;	}

	//�T�[�`.
	int				Search(_Caretation* const inSearchTarget , int inStartPos = 0)
	{
		int theI ;
		//�O�̂��ߔ͈̓`�F�b�N.
		if (inStartPos < 0 || inStartPos >= m_CaretationNubmer) return -1;
		//�����J�n.
		for(theI = inStartPos; theI < m_CaretationNubmer ; theI ++)
		{
			if ( memcmp( &m_Caretation[theI]  , 
						inSearchTarget ,sizeof(_Caretation) ) == 0)
			{	//�~�c�J�b�^!!
				return theI;
			}
		}
		//������Ȃ��ꍇ�́A -1.
		return -1;
	}

	//�f�[�^�擾.
	_Caretation*		At(int Num)	
	{
		if (Num < 0 || Num >= m_CaretationNubmer)	return NULL;
		return &m_Caretation[Num];
	}

	//[]���Z�q�I�[�o�[���[�h
	_Caretation*		operator[](int _P) const
	{
		//__RTI_CHECKER(_P >= 0 && _P < m_CaretationNubmer);
		return &(*(m_Caretation + _P)); 
	}

	//�擪�̃f�[�^���擾.
	_Caretation*	GetStart()	{		return m_Caretation;	}
	//�Ō�̃f�[�^���擾.
	_Caretation*	GetEnd()	{		return &m_Caretation[m_CaretationNubmer];	}

	//�����擾.
	int				GetCount()	{	return m_CaretationNubmer;	}

	//�f�[�^�X���b�v!    ���̃��\�b�h�͔��Ɋ댯�ł�!
	void			SwapData(_Caretation*	inCaretation , int inNum , int inReal)
	{
		//���݂̃f�[�^��j��.
		if (m_Caretation != NULL) ::free( m_Caretation );
		//�f�[�^�����ւ��܂�.
		m_Caretation = inCaretation;
		//�f�[�^�̌�������ւ��܂�.
		m_CaretationNubmer     = inNum;
		m_CaretationRealNumber = inReal;
	}
	/*
	//�w�肵���f�[�^�̃R�s�[�Ɏ������Ȃ�.
	void CopyForm(_TCaretaker* inSrc)
	{
		//�܂��A�����̗̈���N���A.
		Clear();
		//�V�K�ɗ̈���m�ۂ��܂�.
		Alloc( inSrc->GetCount() );
		//���݂̐ݒ�����������܂�.
		memcpy( m_Caretation , inSrc->GetStart() , );
	}
	*/
protected:
	/*********************/
	//���\�b�h
	/*********************/
	//�f�[�^�R�s�[
	virtual	void	CopyData(_Caretation* DestData ,	
							InputData* SrcCaretation , int Many)
	{	//����������́A _Caretation �� InputData �̕����Ⴄ�Ƒ�ςȂ��ƂɂȂ�̂Œ���!!
		memcpy(DestData , SrcCaretation , sizeof(_Caretation) * Many);
	}
	//�V�K�Ƀ�������ǉ������������߂ɁA�������u���b�N���ω������ꍇ�ɌĂ΂��.
	//�������A�����߂����āA�������A���P�[�V�������[�`�������˂� >> ��. �΂����ˁ[.
	//����: OldUseSize �� ���ł�.
	virtual	void	ReAllocMemory(_Caretation* OldMemoryAddress , 
								  _Caretation* NewMemoryAddress ,
								  int OldUseSize)	
	{
	}
	//�V�K�̈�m��.
	bool			NewAlloc(int Many , bool SizeNotChange , bool ClearAlloc)
	{
		//�̂̃������̃T�C�Y(���ł邱�Ƃɒ��ӂ���).
		int				OldUseSize	= m_CaretationRealNumber ;

		if (ClearAlloc == true)
		{//���܂ł̗̈�̕ی�͂��Ȃ��ŐV�K�m��.
			if (m_Caretation != NULL)
			{
				::free( m_Caretation );
			}
			m_Caretation = (_Caretation*)::malloc( sizeof(_Caretation) * (Many + 1) );
			m_CaretationRealNumber = Many;	//���ۊm�ۂ��Ă���̈�̍X�V.
		}
		else
		{//���܂ł̗̈���ێ����Ċm��.
			_Caretation*	NewCaretation;
			//�V�K�̈�m��.
			NewCaretation = 
				(_Caretation*)::realloc( m_Caretation , sizeof(_Caretation) * (Many + 1) );
			if (NewCaretation != m_Caretation && NewCaretation != NULL)
			{
				//���̈ړ��Ŗ����ɂȂ����|�C���^��L���ȏꏊ�ֈړ�������.
				ReAllocMemory(m_Caretation , NewCaretation , min(OldUseSize , Many) );
			}
			//�e��ݒ�̍X�V.
			m_Caretation = NewCaretation;
			m_CaretationRealNumber = Many;	//���ۊm�ۂ��Ă���̈�̍X�V.
		}

		return true;
	}

	/*********************/
	//�����o
	/*********************/
	int				m_CaretationNubmer;			//���̌�.
	_Caretation*	m_Caretation;				//�R���N�V��������f�[�^(�z��ɂȂ�)
	int				m_CaretationRealNumber;		//���Ŏ��ۂɊm�ۂ��Ă���̈�.
private:
};


#endif
