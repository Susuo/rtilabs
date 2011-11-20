// RExpr.cpp: RExpr �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RExpr.h"
#include "RConv.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�v�Z�����Z�b�g.
void RExprToken::setExpression(string inExpression)
{
	this->Expression = inExpression;
	this->Pos = 0;
}

string RExprToken::getExpression()
{
	return this->Expression;
}

//�g�[�N������.
string RExprToken::getToken() throw(RParseException)
{
	const char* p = this->Expression.c_str() ;
	int i = this->Pos;
	for( ; TRIM_SPACE( p[i] ) ; i++);
	if (p[i] == '\0' ) return "";

	switch(p[i])
	{
	case '+':	case '-':	case '*':	case '/':	case '(':	case ')':
		this->Pos = i+1;
		return Expression.substr(i,1);
	case '0':	case '1':	case '2':	case '3':	case '4':	case '5':
	case '6':	case '7':	case '8':	case '9':
		{
			int hit = i;
			//�����ȊO�������̂�҂�.
			for(i++; p[i] >= '0' && p[i] <= '9' ; i++);

			this->Pos = i;
			return Expression.substr(hit ,i - hit);
		}		
	}
	throw RParseException("���� %s �� %d �Ԗڂɉ�͕s�\�̕���������܂���\n",this->Expression.c_str() ,i );
}



void RExprToken::test()
{
	RExprToken token;
	token.setExpression("1 + 1 * 2	+ 1 / 455/ 2324 * 12 +343     + 2456*1123");

	ASSERT( token.getToken() == "1");
	ASSERT( token.getToken() == "+");
	ASSERT( token.getToken() == "1");
	ASSERT( token.getToken() == "*");
	ASSERT( token.getToken() == "2");
	ASSERT( token.getToken() == "+");
	ASSERT( token.getToken() == "1");
	ASSERT( token.getToken() == "/");
	ASSERT( token.getToken() == "455");
	ASSERT( token.getToken() == "/");
	ASSERT( token.getToken() == "2324");
	ASSERT( token.getToken() == "*");
	ASSERT( token.getToken() == "12");
	ASSERT( token.getToken() == "+");
	ASSERT( token.getToken() == "343");
	ASSERT( token.getToken() == "+");
	ASSERT( token.getToken() == "2456");
	ASSERT( token.getToken() == "*");
	ASSERT( token.getToken() == "1123");
}

//�v�Z�J�n.
int RExpr::Expr(string inExpression) throw(RParseException)
{
	Token.setExpression(inExpression);

	getToken();

	return expression();
}

//��
int RExpr::expression() throw(RParseException)
{
	int ans = term();
	while(1)
	{
		if (NextToken == "+")
		{
			getToken();
			ans = ans + term();
		}
		else if (NextToken == "-")
		{
			getToken();
			ans = ans - term();
		}
		else break;
	}
	return ans;
}

//��
int RExpr::term() throw(RParseException)
{
	int ans = factor();
	while(1)
	{
		if (NextToken == "*")
		{
			getToken();
			ans = ans * factor();
		}
		else if (NextToken == "/")
		{
			getToken();
			ans = ans / factor();
		}
		else break;
	}
	return ans;
}

//���q
int RExpr::factor() throw(RParseException)
{
	if (NextToken == "(")
	{
		getToken();
		int ans = this->expression();

		if (NextToken != ")") RParseException("���� %s �Ŋ��ʂ��Ή����Ă��Ȃ��悤�ł�\n",Token.getExpression().c_str() );

		getToken();
		return ans;
	}
	else
	{
		int ans = atoi( NextToken.c_str() );
		getToken();

		return ans;
	}

}


void RExpr::test()
{
	RExpr expr;
	ASSERT(expr.Expr("1+1") == 2);
	ASSERT(expr.Expr("1+1*10") == 11);
	ASSERT(expr.Expr("1+1*10+2") == 13);
	ASSERT(expr.Expr("(1+1)*10+2") == 22);
}

