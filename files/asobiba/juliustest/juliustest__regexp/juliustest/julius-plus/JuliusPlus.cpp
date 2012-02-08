#include "JuliusPlus.h"

#include <sstream>
template<typename _INT> static const std::string num2str(_INT i)
{
	std::stringstream out;
	out << i;
	return out.str();
}
template<typename _INT> static const std::wstring num2wstr(_INT i)
{
	std::wstringstream out;
	out << i;
	return out.str();
}

JuliusPlus::JuliusPlus() : Grammer("topgrammer")
{
	this->LocalCaptureRuleNodeCount = 0;
	this->GlobalRuleNodeCount = 0;
}

JuliusPlus::~JuliusPlus()
{
}

bool JuliusPlus::RuleCommit(std::ostream* dfa , std::ostream* dict  )
{
	struct reverse_automaton
	{
		std::ostream* dfa;
		std::ostream* dict;

		int dfaNumber ;
		int dictNumber ;

		void fire(const rule* toprule,std::ostream* dfa , std::ostream* dict)
		{
			this->dfaNumber = 3;
			this->dictNumber = 2;
			this->dfa = dfa;
			this->dict = dict;

			int firstRuleNumber = this->dfaNumber++; //3
			assert(firstRuleNumber == 3); //0 silE
			                              //1 silB
			                              //2 -1 -1
			                              //�Ȃ̂�3����X�^�[�g

			*dfa << 0 << " " << 0 << " " << firstRuleNumber << " " << 0 << " " << 1 << std::endl;
			*dict << 0 << "\t" << "[</s>]" << "\t" << " " << "silE" << std::endl;

			*dfa << 1 << " " << 1 << " " << 2 << " " << 1 << " " << 1 << std::endl;
			*dict << 1 << "\t" << "[<s>]" << "\t" << " " << "silB" << std::endl;

			*dfa << 2 << " " << -1 << " " << -1 << " " << 1 << " " << 0 << std::endl;

			func(toprule,1,0 , firstRuleNumber);
		}

		//rule   �������郋�[��
		//number ���[���Ɋ���U����ׂ��ԍ� dfaNumber �ԍ��B
		//backNumber ���̃��[���̐e���[���̔ԍ�
		//termNumber ���̃��[�����ǂ��ɂ��J�ڂ��Ȃ��I�[���[�����������ɐi�ޔԍ�
		void func(const rule* currentrule,int number,int backNumber,int termNumber)
		{
			assert(currentrule);
			auto words = currentrule->getWords();
			auto nestrules = currentrule->getNestRules();
			auto termRule = currentrule->getTermRule();

			if (words->size() <= 0)
			{//�P��𔺂�Ȃ��m�[�h��������
				if (termRule == NULL)
				{//�l�X�g�����̃m�[�h�������玩�����X�L�b�v����B
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , number ,backNumber , termNumber);
					}
				}
				else
				{//term rule������̂ł���������������.
					func( termRule , number ,backNumber,termNumber);

					int nextNumber = this->dfaNumber ++ ;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
				//�X�L�b�v����̂ŃR�R�ł����.
				return ;
			}

			//�P�ꎫ�� dict���\�z����B
			int dictNumber = this->dictNumber++;
			{
				for(auto it = words->begin(); it != words->end() ; ++it )
				{
					*dict << dictNumber << "\t" << "[" << (*it)->word << "]" << "\t" << " " << (*it)->yomi << std::endl;
				}
			}
			
			//term rule������ΐ�Ɏ��s����B
			if (termRule == NULL)
			{//�I�[�m�[�h�Ȃ�
				if (nestrules->size() <= 0 && words->size() >= 1)
				{//�P�ꂾ���̃m�[�h�̏ꍇ�A�I�[�ɂȂ�B
					*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
				}
				else
				{//�P��ƃl�X�g�����ɂ���ꍇ�B
				 //�l�X�g�̏������s���B
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
			}
			else
			{
				int nestTermNumber = this->dfaNumber ++ ;

				if (nestrules->size() <= 0 && words->size() >= 1)
				{//�P�ꂾ���̃m�[�h�̏ꍇ�A�I�[�ɂȂ�B
					*dfa << nestTermNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
				}
				else
				{//�P��ƃl�X�g�����ɂ���ꍇ�B
				 //�l�X�g�̏������s���B
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , nestTermNumber);
					}
				}
				func( termRule , nestTermNumber ,number,termNumber);
			}


		}
	} maton;
	maton.fire( &this->Grammer ,dfa , dict  );

	return true;
}

//�����F�����[�����\�z���܂��B ���K�\���ɂ��Ή����Ă��܂��B
xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) 
{
	_USE_WINDOWS_ENCODING;

	//��ԍŏ��������K�\���̍\���ϊ���������.
    // .+ --> (:?.*)
    // (�܂�|���񂩂�)? --> (�܂�|���񂩂�|)   ���������K�\���Ƃ��Ă̓G���[�����A���̃G���W���̏ꍇ�e�F����.
    // �Ȃ̂�? --> �Ȃ�(��|)
	std::wstring optstr = L"";

	//�������ȒP�ɂ��邽�߂ɁAUTF-16�Ń��[�v���܂��B
	for( const wchar_t * p = _A2W( str.c_str() ) ; *p ; ++p )
	{
        if ( *p == L'.' && *(p+1) == L'+')
        { // .+ --> (:?.*)
            optstr += L"(?:.+)";
			++p;
        }
        else if (*p == L'(' && *(p+1) == L'?' && *(p+2) == L':' )
		{
            optstr += L"(?:";
			p+=2;
		}
        else if (*(p+1) == L'?')
        {
            if (*p == L')')
            {// (�܂�|���񂩂�)? --> (�܂�|���񂩂�|)
                    optstr += L"|)";
            }
            else 
            {// �Ȃ̂�? --> �Ȃ�(��|)
                    optstr += std::wstring(L"") + L"(?:" + *p + L"|)";
            }
			++p;
        }
        else if (*p == L'*' || *p == L'+' || *p == L'.' || *p == L'[' || *p == L']')
        {
//            throw exception(std::string("") + "���݂́A���^���� " + p + " �͗��p�ł��܂���B���p�\�ȃ��^���� () | .+ ?");
        }
        else
        {
            optstr += *p;
        }
    }

	this->LocalCaptureRuleNodeCount = 1;
	return AddRegexpImpl(id,optstr, stateHandle);
}

//�����F�����[����o�^���镔���̏ڍׂȎ��s�ł��B���K�\���̃l�X�g������̂ōċN���Ă܂��B
xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle)
{
	_USE_WINDOWS_ENCODING;
    std::wstring matchString;

	//���K�\�����p�[�X���Ȃ����.
	const wchar_t * p = str.c_str();
    const wchar_t * splitPos = p;
    rule*  currentRule = stateHandle;
	for(  ; *p ; ++p )
	{
		if (*p == L'(')
        {
            //�����ʂ܂Ŕ�΂�. )
            int nest = 1;
            const wchar_t* n = p + 1;
            for( ; *n  ; ++n )
            {
                if (*n == L'(')
                {
                    ++nest ;
                }
                else if (*n == L')')
                {
                    --nest ;
                    if (nest <= 0)
                    {
                        break;
                    }
                }
            }

            //�l�X�g����O�̕���
            matchString = std::wstring(splitPos, 0 ,p - splitPos);


			//�l�X�g���Ă��镔�����i�[���郋�[�������.
			rule*  nestRule;

			//�L���v�`���[�H
			std::wstring captureNodeName;
            if (*(p+1) == L'?' && *(p+2) == L':')
            {
                p += 2;
            }
			else
			{
				captureNodeName = num2wstr(this->LocalCaptureRuleNodeCount++) + L":" + num2wstr(this->GlobalRuleNodeCount) ;
			}

			//�����񂾂�������� OR�����ɂȂ��Ă��܂��̂ŁA�l�X�g���i�[����l�X�g�����Ȃ��ƃ_���B
			rule* firstNestRule = new rule(this->GlobalRuleNodeCount ++);
			firstNestRule->AddWord(_W2A(matchString.c_str()));

			//���ړ��Ă�()�l�X�g���i�[���镔�������
			if (captureNodeName.length() <= 0)
			{
				nestRule = new rule(this->GlobalRuleNodeCount ++);
			}
			else
			{
				nestRule = new rule( _W2A(captureNodeName.c_str()) );
			}
			firstNestRule->AddNestRule(nestRule);
			currentRule->AddNestRule(firstNestRule);

			//�������̌�ɂ��\�����A������ꍇ�A���̃c���[���쐬����.
            if (*n == L'\0'  || *(n+1) == L'\0' || *(n+1) == L'|')
            {//���������ō\�����Ƃ܂�ꍇ�͂����ŏI�[
				//nop
            }
            else
            {//����ɍ\���������Ă���ꍇ
            	rule * afterRule = new rule( this->GlobalRuleNodeCount++);
            	nestRule->SetCombineRule(afterRule);

				currentRule = afterRule;
            }

            //�l�X�g���Ă��郋�[�����ċA���Ď��s.
			matchString = std::wstring(p+1 , 0 , (int) (n - p - 1) );
			auto r = this->AddRegexpImpl(id,matchString, nestRule);
			if(!r)
			{
				return xreturn::errnoError(r);
			}

            p = n ;
            splitPos = n + 1;  //+1�͍Ō�� ) ���΂�. i�� for�� ++i �� i == splitPos �ƂȂ�B(�킩��ɂ���)
        }
        else if (*p == L'|') 
        {
        	matchString = std::wstring(splitPos,0 , (int) (p - splitPos));
			if (matchString.length() >= 1)
            {
            	currentRule->AddWord(_W2A(matchString.c_str()));
			}
			//�󕪊� (A|) �̂悤�ȏꍇ�A��m�[�h������.
			if (matchString.length() <= 0 || *(p+1) == L'\0' )
			{
				currentRule->AddWord("");
			}

			splitPos = p + 1;
            currentRule = stateHandle;
        }
        else if (*p == L'.' && *(p+1) == L'+') 
        {
			currentRule->AddWord("��");

			p += 1;
			splitPos = p + 1;
        }
    }

    //�Ō�̎c��
	matchString = std::wstring(splitPos , 0 , (int) (p - splitPos) );
    if ( matchString.length() >= 1 &&str.length() >= 1 && *(p-1) != L')')
    {
        currentRule->AddWord(_W2A(matchString.c_str()) );
    }
	return true;
}
