// MSScriptEval.cpp: MSScriptEval �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "MSScriptEval.h"
#include <atlbase.h>

//msscript �G���W���̐ݒ���Ԃ�����.
#import "msscript.ocx" no_namespace 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MSScriptEval::MSScriptEval()
{
	this->ErrorMessage = "";
	this->ErrorPosstion = 0;
}

MSScriptEval::~MSScriptEval()
{

}

//���̎��s
double MSScriptEval::Eval(const string & inString)
{
	this->ErrorMessage = "";
	this->ErrorPosstion = 0;

	CComPtr<IScriptControl> scriptControl;
	//�I�u�W�F�N�g�̍\�z
	HRESULT hr = scriptControl.CoCreateInstance(__uuidof(ScriptControl));
	if (FAILED(hr))
	{
		_com_issue_error(hr);
	}


	_variant_t v;
	try
	{
		//����́Ajscript ��I��
		scriptControl->PutAllowUI( VARIANT_TRUE );
		scriptControl->PutLanguage(_bstr_t("jscript"));

		//���̎��s
		v = scriptControl->Eval(_bstr_t(inString.c_str()));
	}
	catch(_com_error)
	{
		//���s���̃G���[
		IScriptErrorPtr error = scriptControl->GetError();
		if (error != NULL)
		{
			this->ErrorMessage = (const char*) error->GetDescription();
			this->ErrorPosstion = (int) error->GetColumn() + 1;	//�ŏ���1�����ڂɂ�����
		}
		else
		{
			this->ErrorMessage = "���m�̃G���[���������܂���";
			this->ErrorPosstion = 0;
		}
	}
	return (double)v;
}

//�G���[���ǂ���.
bool MSScriptEval::IsError() const
{
	//���\���������Ȏ����ł���...
	return ( this->ErrorMessage != "");
}

//�e�X�g
void MSScriptEval::test()
{
	{
		//����n
		MSScriptEval mse;
		double rr;
		ASSERT((rr = mse.Eval("1+1")) == 2);
		ASSERT(!mse.IsError());
	}
	{
		//�ُ�n
		MSScriptEval mse;
		mse.Eval("1+1abc");
		ASSERT(mse.IsError());
	}
}
