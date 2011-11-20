// RExpr.h: RExpr �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_)
#define AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"


//�������g�[�N���ŕ������ĕԂ�.
class RExprToken
{
public:
	void setExpression(string inExpression);
	string getExpression();
	string getToken() throw(RParseException);

	static void test();
private:
	string		Expression;
	int			Pos;
};

//�ċA�I�������\����͂ɂ�鐔�����
//�Q�l http://www3.justnet.ne.jp/~h-masuda/ProText/CAlgo/eval01.html

//�Ȃ��A
//�����_�A���̐��@�ɂ͑Ή����Ă܂���̂Œ��ӂ��Ă�������.
class RExpr  
{
public:
	int Expr(string inExpression) throw(RParseException);
	static void test();
private:
	//��
	int expression() throw(RParseException);
	//��
	int term() throw(RParseException);
	//���q
	int factor() throw(RParseException);

	//���̃g�[�N�����Z�b�g.
	void getToken()
	{
		NextToken = Token.getToken();
	}
	string NextToken;
	RExprToken	Token;
};


#endif // !defined(AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_)
