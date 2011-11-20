// TokenParser.cpp: TokenParser �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "TokenParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TokenParser::TokenParser()
{
	this->Clear();
}

TokenParser::~TokenParser()
{

}
//�g�[�N���̃p�[�X
bool TokenParser::Set(const string & inString)
{
	this->Clear();

	int length = inString.size();
	for(int i = 0 ; i < length ; )
	{
		if ( inString[i] == ' ' )
		{//�X�y�[�X�͖���
			i ++;
			continue;
		}
		else if ( this->IsOperator( inString[i]  ) )
		{//�I�y���[�^�[
			this->TokenList.push_back( inString.substr(i , 1 ) );
			i++;
		}
		else if ( this->IsNumber(inString[i]) )
		{//����
			int start = i;
			for( ; this->IsNumber(inString[i]) ; i ++)
			{
				//nop
			}
			//����̏����������ł��邱�Ƃ�����A�����񂪋�ɂȂ邱�Ƃ͂��肦�Ȃ�.
			ASSERT(i > start);

			this->TokenList.push_back(inString.substr(start , i-start ));
		}
		else if ( ::isalpha(inString[i]) )
		{//�p��
			int start = i;
			for( ; ::isalpha(inString[i]) ; i ++)
			{
				//nop
			}
			//����̏������p���ł��邱�Ƃ�����A�����񂪋�ɂȂ邱�Ƃ͂��肦�Ȃ�.
			ASSERT(i > start);

			this->TokenList.push_back(inString.substr(start , i-start ));
		}
		else 
		{//�s��..
			this->ErrorPosstion = i;
			return false;
		}
	}
	//�C�e���[�^�ɏ����l�ʒu���Z�b�g����.
	this->ActivePoint = TokenList.begin();
	return true;
}

//�I�y���[�^���ǂ���.
bool TokenParser::IsOperator(char inChar) const
{
	return inChar == '+' || inChar == '-' || inChar == '*' || inChar == '/' || inChar == '(' || inChar == ')' || inChar == '^';
}

//�������ǂ���
bool TokenParser::IsNumber(char inChar) const
{
	return (inChar >= '0' && inChar <= '9')   || inChar == '.';
}


//�e�X�g
void TokenParser::test()
{
	{
		//����n
		TokenParser tp;
		ASSERT( tp.Set("sin(0)") );
	}
	{
		//����n
		TokenParser tp;
		string r;
		//�p�[�X
		ASSERT( tp.Set("1 + 2 + 3.14") );
		//�g�[�N���̎��o��
		ASSERT( (r = tp.GetToken()) == "1");
		//2��Ăяo���Ă����ʂ͈ꏏ
		ASSERT( (r = tp.GetToken()) == "1");
		//���̃g�[�N����
		ASSERT( tp.NextToken() );
		ASSERT( (r = tp.GetToken()) == "+");
		//���̃g�[�N���� ���@2
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "2");
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "3.14");
		//����ȏ�Ȃ��̂ŋ�ɂȂ�.
		ASSERT( (r = tp.GetNextToken()) == "");
		//����ȏ�Ȃ��̂ŋ�ɂȂ�.
		ASSERT( (r = tp.GetToken()) == "");
		//�ړ��ł��Ȃ��̂� false
		ASSERT( ! tp.NextToken() );
	}
	{
		//����n2
		TokenParser tp;
		string r;
		//�p�[�X
		ASSERT( tp.Set("1 + (2 - 4)/2") );
		ASSERT( (r = tp.GetNextToken()) == "1");
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "(");
		ASSERT( (r = tp.GetNextToken()) == "2");
		ASSERT( (r = tp.GetNextToken()) == "-");
		ASSERT( (r = tp.GetNextToken()) == "4");
		ASSERT( (r = tp.GetNextToken()) == ")");
		ASSERT( (r = tp.GetNextToken()) == "/");
		ASSERT( (r = tp.GetNextToken()) == "2");
	}
	{
		//���������������s���Ă��邱�Ƃ̊m�F.
		TokenParser tp;
		string r;

		//����ȏ�Ȃ��̂ŋ�ɂȂ�.
		ASSERT( (r = tp.GetNextToken()) == "");
		//����ȏ�Ȃ��̂ŋ�ɂȂ�.
		ASSERT( (r = tp.GetToken()) == "");
		//�ړ��ł��Ȃ��̂� false
		ASSERT( ! tp.NextToken() );
	}
	{
		//�p�[�X�G���[
		TokenParser tp;
		int rr;
		//�p�[�X�G���[
		ASSERT( ! tp.Set("1 + ### 2 + 3.14") );
		//5�����ڂɃG���[
		ASSERT( (rr = tp.getErrorPosstion()) == 5);
	}

	
}

