// TokenParser.h: TokenParser �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_)
#define AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��������ŏ��P�ʂɕ�������.
class TokenParser  
{
public:
	TokenParser();
	virtual ~TokenParser();

	//������
	void Clear()
	{
		this->ErrorPosstion = 0;
		this->TokenList.clear();
		this->ActivePoint = TokenList.begin();
	}
	//�g�[�N���̃p�[�X
	bool Set(const string & inString);

	//���̃g�[�N�����擾����.
	string GetToken() const
	{
		if (this->ActivePoint == TokenList.end())
		{
			return "";
		}
		return *this->ActivePoint;
	}
	//���̃g�[�N�����擾���A�g�[�N������i�߂�
	string GetNextToken() 
	{
		if (this->ActivePoint == TokenList.end())
		{
			return "";
		}
		return *this->ActivePoint++;
	}
	//���̃g�[�N���Ɉړ�����.
	bool NextToken() 
	{
		if (this->ActivePoint == TokenList.end())
		{
			return false;
		}
		this->ActivePoint++;
		return true;
	}

	//�G���[�̏ꏊ���擾����.
	int getErrorPosstion() const
	{
		return this->ErrorPosstion + 1;	//1�s�ڂ���J�n������.
	}
	//�e�X�g
	static void test();

private:
	//�I�y���[�^���ǂ���.
	bool IsOperator(char inChar) const;
	//�������ǂ���
	bool IsNumber(char inChar) const;

private:
	//�g�[�N�����i�[������ꕨ
	XLStringList TokenList;
	//���ݓǂݍ���ł���ꏊ.
	XLStringList::const_iterator ActivePoint;
	//�G���[�̏ꏊ
	int ErrorPosstion;
};

#endif // !defined(AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_)
