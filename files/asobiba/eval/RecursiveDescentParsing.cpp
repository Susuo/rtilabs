// Parser.cpp: Parser �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "RecursiveDescentParsing.h"
#include "XLException.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RecursiveDescentParsing::RecursiveDescentParsing()
{

}

RecursiveDescentParsing::~RecursiveDescentParsing()
{

}

//���s
double RecursiveDescentParsing::Run(TokenParser * poolToken)
{
	this->PoolToken = poolToken;
	double ans = this->Expression();
	if ( this->PoolToken->GetToken() != "" )
	{
		throw XLException(format("�s���Ȓl(%s)��������܂���" , this->PoolToken->GetToken().c_str()) );
	}
	return ans;
}

//��
double RecursiveDescentParsing::Expression()
{
	double ans = this->Term();
	while(1)
	{
		if ( this->PoolToken->GetToken() == "+" )
		{//�����Z
			this->PoolToken->NextToken();
			ans = ans + this->Term();
		}
		else if ( this->PoolToken->GetToken() == "-" )
		{//�����Z
			this->PoolToken->NextToken();
			ans = ans - this->Term();
		}
		else
		{
			break;
		}
	}
	return ans;
}

//��
double RecursiveDescentParsing::Term()
{
	double ans = this->Factor();
	while(1)
	{
		if(this->PoolToken->GetToken() == "*")
		{//�|���Z
			this->PoolToken->NextToken();
			ans = ans * this->Factor();
		}
		else if(this->PoolToken->GetToken() == "/")
		{//����Z
			this->PoolToken->NextToken();
			ans = ans / this->CheckZeroDiv( this->Factor() );
		}
		else if(this->PoolToken->GetToken() == "^")
		{//�ׂ���
			this->PoolToken->NextToken();
			ans = pow(ans,this->Factor());
		}
		else
		{
			break;
		}
	}
	return ans;

}

//���q
double RecursiveDescentParsing::Factor()
{
    double  ans;

    if(this->PoolToken->GetToken() == "(")
	{//����
        this->PoolToken->NextToken();
        ans = this->Expression();
        if(this->PoolToken->GetToken() != ")")
		{
            throw XLException("�����ʂ�����܂���");
        }
        this->PoolToken->NextToken();
    }
    else if(this->PoolToken->GetToken() == "+" )
	{//+�����t
		this->PoolToken->NextToken();
		ans = this->Factor();
    }
    else if(this->PoolToken->GetToken() == "-" )
	{//-�����t
		this->PoolToken->NextToken();
		ans = - this->Factor();
    }
	else
	{//�����܂��͊֐�
        ans = this->Data();
	}

	return ans;
}

//�f�[�^
double RecursiveDescentParsing::Data()
{
	if ( IsNumber(this->PoolToken->GetToken()) )
	{//����
		double ans = atof( this->PoolToken->GetToken().c_str() );
		this->PoolToken->NextToken();
		return ans;
	}
	//�ȉ��֐�
	else if (this->PoolToken->GetToken() == "sin")
	{
		this->PoolToken->NextToken();
		return sin(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "cos")
	{
		this->PoolToken->NextToken();
		return cos(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "tan")
	{
		this->PoolToken->NextToken();
		return tan(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "sqrt")
	{
		this->PoolToken->NextToken();
		return sqrt(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "pi")
	{
		this->PoolToken->NextToken();
		return 3.14;
	}
	else if (this->PoolToken->GetToken() == "piyutori")
	{
		this->PoolToken->NextToken();
		return 3;	//���悻3
	}

	throw XLException( format("�s���Ȋ֐�(%s)���Ăяo����܂���", this->PoolToken->GetToken().c_str() ) );
}

//Zero���������O�ŗ��Ƃ�.
double RecursiveDescentParsing::CheckZeroDiv(double inValue) const
{
	if ( inValue == 0 )
	{
		throw XLException("�[���ŏ��Z���悤�Ƃ��܂���");
	}
	return inValue;
}

//�����݂̂ō\������Ă��邩�H
bool RecursiveDescentParsing::IsNumber(const string & inString) const
{
	int len = inString.size();
	for(int i = 0 ; i < len ; i ++)
	{
		if (! (::isdigit( inString[i] )  ||  inString[i] == '.') )
		{
			return false;
		}
	}
	return true;
}

//�e�X�g
void RecursiveDescentParsing::test()
{
	RecursiveDescentParsing rdp;
	TokenParser tp;
	double rr;
	ASSERT( tp.Set("1 + 2 + 3") );
	ASSERT( (rr = rdp.Run(&tp) ) == 6 );

	ASSERT( tp.Set("1 + (2 - 3)") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("sin(0)") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("-1 + 1") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("-1 + 2 * 5") );
	ASSERT( (rr = rdp.Run(&tp) ) == 9 );

	ASSERT( tp.Set("-(2+3)") );
	ASSERT( (rr = rdp.Run(&tp) ) == -5 );

	ASSERT( tp.Set("(-1)+2*3+(4+(5-2))") );
	ASSERT( (rr = rdp.Run(&tp) ) == 12 );

	ASSERT( tp.Set("1.5 + 1.5") );
	ASSERT( (rr = rdp.Run(&tp) ) == 3 );
}
