// MSScriptEval.h: MSScriptEval �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_)
#define AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ScriptControl �𗘗p���� jscript eval���Ăяo��
//�Q�l
//http://dobon.net/vb/dotnet/programing/eval.html
//http://www.codeproject.com/KB/COM/scriptdemo.aspx?display=PrintAll&fid=4239&df=90&mpp=25&noise=3&sort=Position&view=Quick&select=1250951
class MSScriptEval  
{
public:
	MSScriptEval();
	virtual ~MSScriptEval();

	//���̎��s
	double Eval(const string & inString);

	//�G���[���ǂ���.
	bool IsError() const;
	//�G���[���b�Z�[�W�̎擾
	string getErrorMessage() const
	{
		return this->ErrorMessage;
	}
	//�G���[�̈ʒu
	int getErrorPosstion() const
	{
		return this->ErrorPosstion;
	}

	//�e�X�g
	static void test();

private:
	//�G���[���b�Z�[�W
	string ErrorMessage;
	//�G���[�̏ꏊ
	int ErrorPosstion;
};

#endif // !defined(AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_)
