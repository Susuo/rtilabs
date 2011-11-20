// Parser.h: Parser �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_)
#define AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
//�g�[�N�����p�[�X����.
#include "TokenParser.h"

// �ċA���~�\�����
//�Q�l: http://www005.upp.so-net.ne.jp/h-masuda/cl/CAlgo/eval01.html
//      http://d.hatena.ne.jp/h_sakurai/20060119
class RecursiveDescentParsing  
{
public:
	RecursiveDescentParsing();
	virtual ~RecursiveDescentParsing();

	//���s
	double Run(TokenParser * poolToken);

	//�e�X�g
	static void test();

private:
	//��
	double Expression();

	//��
	double Term();

	//���q
	double Factor();

	//�f�[�^
	double Data();

	//Zero���������O�ŗ��Ƃ�.
	double CheckZeroDiv(double inValue) const;

	//�����݂̂ō\������Ă��邩�H
	bool IsNumber(const string & inString) const;

private:
	//�����g�[�N���ŕ�����������.
	TokenParser* PoolToken;
};

#endif // !defined(AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_)
