#pragma once;
#include <fstream>
#include <iostream>
#include <sstream>
#include "rule.h"
#include "xreturn.h"
#include "windows_encoding.h"


class JuliusPlus
{

public:
	JuliusPlus();
	virtual ~JuliusPlus();

	rule Grammer;

	bool JuliusPlus::RuleCommit(std::ostream* dfa , std::ostream* dict  );
	//�����F�����[�����\�z���܂��B ���K�\���ɂ��Ή����Ă��܂��B
	xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) ;
	//�����F�����[����o�^���镔���̏ڍׂȎ��s�ł��B���K�\���̃l�X�g������̂ōċN���Ă܂��B
	xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle);

private:
	int LocalCaptureRuleNodeCount;
	int GlobalRuleNodeCount;
};
