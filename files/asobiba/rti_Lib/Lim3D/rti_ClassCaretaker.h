#ifndef	___LIM3D_CLASSCARETAKER__H	//�d���h�~
#define ___LIM3D_CLASSCARETAKER__H

#include <list>
/*
//�R���N�^�[�e���v���[�g�N���X.
template<class _Caretation>
class TClassCaretaker
{
public:
	//�R���X�g���N�^
	TClassCaretaker(){ m_IsClear = false;}		
	//�f�X�g���N�^
	~TClassCaretaker(){Clear();}
	//�V�K�f�[�^�쐬.
	_Caretation*	New()
	{
		//�̈�m��
		_Caretation*	puf = new _Caretation;
		//�̈�m�ێ��s�̏ꍇ...
		if (puf == NULL) return NULL;	
		//�m�ۂ����̈�ɐe��������.
		if (Mount(puf) == false)
		{
			delete puf;
			return NULL;
		}
		//���X�g�ɒǉ�.
		m_CaretationList.push_back(puf);
		//�m�ۂ����̈��Ԃ�.
		return puf;
	}
	//�K�y�[�W�R���N�V�������폜(�ӂ[�͎g��Ȃ��ł�������.)
	bool DeleteCaretation(_Caretation* const Target)
	{
		//���݃N���A���ɂ��A���X�g�����b�N���Ă��܂�.
		if (m_IsClear==true)	return true;

		std::list<_Caretation*>::iterator it = 	m_CaretationList.begin();
		std::list<_Caretation*>::iterator end= 	m_CaretationList.end();
		for( ; it != end ; it++)	
		{
			if (*it == Target)
			{	//����.
				m_CaretationList.erase(it);
				return true;
			}
		}
		return false;
	}
	//�N���A
	void Clear()
	{
		//���݃N���A���ɂ��A���X�g�����b�N���Ă��܂�.
		m_IsClear = true;

		std::list<_Caretation*>::iterator it = 	m_CaretationList.begin();
		std::list<_Caretation*>::iterator end= 	m_CaretationList.end();
		for( ; it != end ; it++)	
		{
			(*it)->Release();
		}
		m_CaretationList.clear();
		//���X�g�ɑ΂��郍�b�N�̉���.
		m_IsClear = false;

	}
	//�j��
	bool Release()
	{
		delete this;
		return true;
	}
	//�R���N�V�����̌����擾.
	int GetCount(){return m_CaretationList.size(); }
	//�R���N�V�����f�[�^���擾�J�n.
	_Caretation*	FindFirst()
	{
		m_ListIT = m_CaretationList.begin();
		if (m_ListIT == m_CaretationList.end() )	return NULL;
		return *m_ListIT;
	}
	//�R���N�V�����f�[�^���擾�p��.
	_Caretation*	FindNext()
	{
		m_ListIT ++;
		if (m_ListIT == m_CaretationList.end() )	return NULL;
		return *m_ListIT;
	}
	//�w��ꏊ�̃f�[�^���擾.
	_Caretation* At(int No)
	{
		std::list<_Caretation*>::iterator	theCaretaker = m_CaretationList.begin();
		std::list<_Caretation*>::iterator	theEnd       = m_CaretationList.end();
		for( ; theCaretaker != theEnd ; theCaretaker++ )
		{
			if (No == 0) return *theCaretaker;
			No--;
		}
		//������Ȃ�...
		return NULL;
	}
	bool Alloc(int Many)
	{
		m_CaretationList.resize(Many);
		return true;
	}
protected:
	//�쐬�����N���X�Ɏ������e���Ɠ`����.
	virtual	bool Mount(_Caretation* AllocData) = 0;

	std::list<_Caretation*>				m_CaretationList;	//�K�y�[�W�R���N�V�������Ă�����e.
	std::list<_Caretation*>::iterator	m_ListIT;			//�����Ɏg���C�e���[�^
	bool								m_IsClear;			//Clear�̒��������� true
private:
};
*/
#endif				//�d���h�~