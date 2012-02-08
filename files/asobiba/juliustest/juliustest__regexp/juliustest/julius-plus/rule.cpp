/*
julius ��MS-SAPI���x�����炢�Ɋy�Ɏg�����߂ɁA��X�͂����ƃn�b�N�����Ȃ���΂Ȃ�Ȃ��B
�\�[�X�R�[�h�͂����R�ɁB


julius�u�}�i�̖{���̗͂��v���m��!!�v
*/
#include "rule.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

rule::~rule()
{
	{
		for(auto it = this->words.begin() ; it != this->words.end() ; ++it)
		{
			delete *it;
		}
		this->words.clear();
	}
	{
		for(auto it = this->nestrules.begin() ; it != this->nestrules.end() ; ++it)
		{
			delete *it;
		}
		this->nestrules.clear();
	}
	delete this->combineRule;
	this->combineRule = NULL;
}

std::string rule::getRuleName() const
{
	if (this->id == INT_MAX)
	{
		assert(!this->name.empty());
		return this->name;
	}
	std::stringstream s;
	s << "_RULE_" << this->id;
	return s.str();
}

/*
bool rule::Commit(const std::string & parentRuleName ,std::ostringstream* grammar , std::ostringstream* voca  )
{
	bool nestruleonly = true;
	bool wordonly = true;
	std::string rulename = getRuleName();
	std::string wordrule = rulename;

	//���̃��[���ɂ��Ē��ׂ܂��B
	auto it = rules.begin();
	for(;it!=rules.end();++it)
	{
		if( (*it)->isWordNode() )
		{
			wordonly = false;
		}
		else
		{
			nestruleonly = false;
		}
	}
	assert(!(wordonly == true && nestruleonly == true)); //���� only �Ȃ̂͂ǂ��l���Ă��I�J�V�C�B

	//�܂��l�X�g���郋�[�����珈�����܂��B
	if (!wordonly)
	{
		for(it = rules.begin();it!=rules.end();++it)
		{
			if( ! (*it)->isWordNode() )
			{
				*grammar << rulename << " : " << (*it)->getNestRule()->getRuleName() << rulename << std::endl;
			}
		}
		//�P�������ꍇ�A�P����i�[�����p���[����ǉ�����B
		wordrule = rulename + "__W";
	}

	//���ɒP�����������.
	if (!nestruleonly)
	{
		//���ɒP����������܂��B
		*grammar << rulename << " : " << wordrule << rulename << std::endl;
		*voca << "% " << wordrule  << std::endl;
		for(it = rules.begin();it!=rules.end();++it)
		{
			if( (*it)->isWordNode() )
			{
				*voca << (*it)->getWord()  << " " << (*it)->getWordYomi() <<  std::endl;
			}
		}
	}

	//�Ō�ɁA�l�X�g���Ă��郋�[�v�������Ăяo���ďI��.
	if (!wordonly)
	{
		for(it = rules.begin();it!=rules.end();++it)
		{
			if( ! (*it)->isWordNode() )
			{
				(*it)->getNestRule()->Commit(rulename,grammar,voca);
			}
		}
	}

	return true;
}
*/

//�ǂ݂��쐬����B
std::string rule::convertYomi(const std::string& word) const
{
	std::string w = word;

	//���̐�n��
	//�Ђ炪�Ȉ���ȕϊ�(�𒼂ȕ��@��)
	boost::algorithm::replace_all(w , "�A","��");
	boost::algorithm::replace_all(w , "�C","��");
	boost::algorithm::replace_all(w , "�E","��");
	boost::algorithm::replace_all(w , "�G","��");
	boost::algorithm::replace_all(w , "�I","��");
	boost::algorithm::replace_all(w , "�J","��");
	boost::algorithm::replace_all(w , "�L","��");
	boost::algorithm::replace_all(w , "�N","��");
	boost::algorithm::replace_all(w , "�P","��");
	boost::algorithm::replace_all(w , "�R","��");
	boost::algorithm::replace_all(w , "�T","��");
	boost::algorithm::replace_all(w , "�V","��");
	boost::algorithm::replace_all(w , "�X","��");
	boost::algorithm::replace_all(w , "�Z","��");
	boost::algorithm::replace_all(w , "�\","��");
	boost::algorithm::replace_all(w , "�^","��");
	boost::algorithm::replace_all(w , "�`","��");
	boost::algorithm::replace_all(w , "�c","��");
	boost::algorithm::replace_all(w , "�e","��");
	boost::algorithm::replace_all(w , "�g","��");
	boost::algorithm::replace_all(w , "�i","��");
	boost::algorithm::replace_all(w , "�j","��");
	boost::algorithm::replace_all(w , "�k","��");
	boost::algorithm::replace_all(w , "�l","��");
	boost::algorithm::replace_all(w , "�m","��");
	boost::algorithm::replace_all(w , "�n","��");
	boost::algorithm::replace_all(w , "�q","��");
	boost::algorithm::replace_all(w , "�t","��");
	boost::algorithm::replace_all(w , "�w","��");
	boost::algorithm::replace_all(w , "�z","��");
	boost::algorithm::replace_all(w , "�}","��");
	boost::algorithm::replace_all(w , "�~","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","���J");
	boost::algorithm::replace_all(w , "�@","��");
	boost::algorithm::replace_all(w , "�B","��");
	boost::algorithm::replace_all(w , "�D","��");
	boost::algorithm::replace_all(w , "�F","��");
	boost::algorithm::replace_all(w , "�H","��");
	boost::algorithm::replace_all(w , "�K","��");
	boost::algorithm::replace_all(w , "�M","��");
	boost::algorithm::replace_all(w , "�O","��");
	boost::algorithm::replace_all(w , "�Q","��");
	boost::algorithm::replace_all(w , "�S","��");
	boost::algorithm::replace_all(w , "�U","��");
	boost::algorithm::replace_all(w , "�W","��");
	boost::algorithm::replace_all(w , "�Y","��");
	boost::algorithm::replace_all(w , "�[","��");
	boost::algorithm::replace_all(w , "�]","��");
	boost::algorithm::replace_all(w , "�_","��");
	boost::algorithm::replace_all(w , "�W","��");
	boost::algorithm::replace_all(w , "�Y","��");
	boost::algorithm::replace_all(w , "�[","��");
	boost::algorithm::replace_all(w , "�]","��");
	boost::algorithm::replace_all(w , "�o","��");
	boost::algorithm::replace_all(w , "�r","��");
	boost::algorithm::replace_all(w , "�u","��");
	boost::algorithm::replace_all(w , "�x","��");
	boost::algorithm::replace_all(w , "�{","��");
	boost::algorithm::replace_all(w , "�p","�p");
	boost::algorithm::replace_all(w , "�s","�s");
	boost::algorithm::replace_all(w , "�v","�v");
	boost::algorithm::replace_all(w , "�y","�|");
	boost::algorithm::replace_all(w , "�|","�|");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");
	boost::algorithm::replace_all(w , "��","��");

	//���ꃋ�[��
	boost::algorithm::replace_all(w , "���J","����");
	boost::algorithm::replace_all(w , "����ɂ���","����ɂ���");
	boost::algorithm::replace_all(w , "����΂��","����΂��");
	boost::algorithm::replace_all(w , "�킽����","�킽����");
	boost::algorithm::replace_all(w , "�ڂ���","�ڂ���");
	boost::algorithm::replace_all(w , "�����","�����");
	boost::algorithm::replace_all(w , "������","������");
	boost::algorithm::replace_all(w , "���̂����","���̂����");

	//yomi2voca���
	//# 3�����ȏォ��Ȃ�ϊ��K���iv a�j
	boost::algorithm::replace_all(w , "���J��"," b a");
	boost::algorithm::replace_all(w , "���J��"," b i");
	boost::algorithm::replace_all(w , "���J��"," b e");
	boost::algorithm::replace_all(w , "���J��"," b o");
	boost::algorithm::replace_all(w , "���J��"," by u");

	//# 2��������Ȃ�ϊ��K��
    boost::algorithm::replace_all(w , "���J"," b u");
	boost::algorithm::replace_all(w , "����"," a a");
	boost::algorithm::replace_all(w , "����"," i i");
    boost::algorithm::replace_all(w , "����"," i e");
	boost::algorithm::replace_all(w , "����"," y a");
	boost::algorithm::replace_all(w , "����"," u:");
	boost::algorithm::replace_all(w , "����"," e e");
	boost::algorithm::replace_all(w , "����"," o:");
	boost::algorithm::replace_all(w , "����"," k a:");
	boost::algorithm::replace_all(w , "����"," k i:");
	boost::algorithm::replace_all(w , "����"," k u:");
	boost::algorithm::replace_all(w , "����"," ky a");
	boost::algorithm::replace_all(w , "����"," ky u");
	boost::algorithm::replace_all(w , "����"," ky o");
	boost::algorithm::replace_all(w , "����"," k e:");
	boost::algorithm::replace_all(w , "����"," k o:");
	boost::algorithm::replace_all(w , "����"," g a:");
	boost::algorithm::replace_all(w , "����"," g i:");
	boost::algorithm::replace_all(w , "����"," g u:");
	boost::algorithm::replace_all(w , "����"," gy a");
	boost::algorithm::replace_all(w , "����"," gy u");
	boost::algorithm::replace_all(w , "����"," gy o");
	boost::algorithm::replace_all(w , "����"," g e:");
	boost::algorithm::replace_all(w , "����"," g o:");
	boost::algorithm::replace_all(w , "����"," s a:");
	boost::algorithm::replace_all(w , "����"," sh i:");
	boost::algorithm::replace_all(w , "����"," s u:");
	boost::algorithm::replace_all(w , "����"," sh a");
	boost::algorithm::replace_all(w , "����"," sh u");
	boost::algorithm::replace_all(w , "����"," sh o");
	boost::algorithm::replace_all(w , "����"," s e:");
	boost::algorithm::replace_all(w , "����"," s o:");
	boost::algorithm::replace_all(w , "����"," z a:");
	boost::algorithm::replace_all(w , "����"," j i:");
	boost::algorithm::replace_all(w , "����"," z u:");
	boost::algorithm::replace_all(w , "����"," zy a");
	boost::algorithm::replace_all(w , "����"," zy u");
	boost::algorithm::replace_all(w , "����"," zy o");
	boost::algorithm::replace_all(w , "����"," z e:");
	boost::algorithm::replace_all(w , "����"," z o:");
	boost::algorithm::replace_all(w , "����"," t a:");
	boost::algorithm::replace_all(w , "����"," ch i:");
	boost::algorithm::replace_all(w , "��"," ts a");
	boost::algorithm::replace_all(w , "��"," ts i");
	boost::algorithm::replace_all(w , "��"," ts u:");
	boost::algorithm::replace_all(w , "��"," ch a");
	boost::algorithm::replace_all(w , "��"," ch u");
	boost::algorithm::replace_all(w , "��"," ch o");
	boost::algorithm::replace_all(w , "��"," ts e");
	boost::algorithm::replace_all(w , "��"," ts o");
	boost::algorithm::replace_all(w , "�Ă�"," t e:");
	boost::algorithm::replace_all(w , "�Ƃ�"," t o:");
	boost::algorithm::replace_all(w , "����"," d a:");
	boost::algorithm::replace_all(w , "����"," j i:");
	boost::algorithm::replace_all(w , "�Â�"," d u:");
	boost::algorithm::replace_all(w , "�Â�"," zy a");
	boost::algorithm::replace_all(w , "�Â�"," zy u");
	boost::algorithm::replace_all(w , "�Â�"," zy o");
	boost::algorithm::replace_all(w , "�ł�"," d e:");
	boost::algorithm::replace_all(w , "�ǂ�"," d o:");
	boost::algorithm::replace_all(w , "�Ȃ�"," n a:");
	boost::algorithm::replace_all(w , "�ɂ�"," n i:");
	boost::algorithm::replace_all(w , "�ʂ�"," n u:");
	boost::algorithm::replace_all(w , "�ʂ�"," ny a");
	boost::algorithm::replace_all(w , "�ʂ�"," ny u");
	boost::algorithm::replace_all(w , "�ʂ�"," ny o");
	boost::algorithm::replace_all(w , "�˂�"," n e:");
	boost::algorithm::replace_all(w , "�̂�"," n o:");
	boost::algorithm::replace_all(w , "�͂�"," h a:");
	boost::algorithm::replace_all(w , "�Ђ�"," h i:");
	boost::algorithm::replace_all(w , "�ӂ�"," f u:");
	boost::algorithm::replace_all(w , "�ӂ�"," hy a");
	boost::algorithm::replace_all(w , "�ӂ�"," hy u");
	boost::algorithm::replace_all(w , "�ӂ�"," hy o");
	boost::algorithm::replace_all(w , "�ւ�"," h e:");
	boost::algorithm::replace_all(w , "�ق�"," h o:");
	boost::algorithm::replace_all(w , "�΂�"," b a:");
	boost::algorithm::replace_all(w , "�т�"," b i:");
	boost::algorithm::replace_all(w , "�Ԃ�"," b u:");
	boost::algorithm::replace_all(w , "�ӂ�"," hy a");
	boost::algorithm::replace_all(w , "�Ԃ�"," by u");
	boost::algorithm::replace_all(w , "�ӂ�"," hy o");
	boost::algorithm::replace_all(w , "�ׂ�"," b e:");
	boost::algorithm::replace_all(w , "�ڂ�"," b o:");
	boost::algorithm::replace_all(w , "�ς�"," p a:");
	boost::algorithm::replace_all(w , "�҂�"," p i:");
	boost::algorithm::replace_all(w , "�Ղ�"," p u:");
	boost::algorithm::replace_all(w , "�Ղ�"," py a");
	boost::algorithm::replace_all(w , "�Ղ�"," py u");
	boost::algorithm::replace_all(w , "�Ղ�"," py o");
	boost::algorithm::replace_all(w , "�؂�"," p e:");
	boost::algorithm::replace_all(w , "�ۂ�"," p o:");
	boost::algorithm::replace_all(w , "�܂�"," m a:");
	boost::algorithm::replace_all(w , "�݂�"," m i:");
	boost::algorithm::replace_all(w , "�ނ�"," m u:");
	boost::algorithm::replace_all(w , "�ނ�"," my a");
	boost::algorithm::replace_all(w , "�ނ�"," my u");
	boost::algorithm::replace_all(w , "�ނ�"," my o");
	boost::algorithm::replace_all(w , "�߂�"," m e:");
	boost::algorithm::replace_all(w , "����"," m o:");
	boost::algorithm::replace_all(w , "�₟"," y a:");
	boost::algorithm::replace_all(w , "�䂣"," y u:");
	boost::algorithm::replace_all(w , "���"," y a:");
	boost::algorithm::replace_all(w , "���"," y u:");
	boost::algorithm::replace_all(w , "���"," y o:");
	boost::algorithm::replace_all(w , "�悧"," y o:");
	boost::algorithm::replace_all(w , "�炟"," r a:");
	boost::algorithm::replace_all(w , "�股"," r i:");
	boost::algorithm::replace_all(w , "�那"," r u:");
	boost::algorithm::replace_all(w , "���"," ry a");
	boost::algorithm::replace_all(w , "���"," ry u");
	boost::algorithm::replace_all(w , "���"," ry o");
	boost::algorithm::replace_all(w , "�ꂥ"," r e:");
	boost::algorithm::replace_all(w , "�낧"," r o:");
	boost::algorithm::replace_all(w , "�킟"," w a:");
	boost::algorithm::replace_all(w , "����"," o:");

	boost::algorithm::replace_all(w , "���J"," b u");
	boost::algorithm::replace_all(w , "�ł�"," d i");
	boost::algorithm::replace_all(w , "�ł�"," d e:");
	boost::algorithm::replace_all(w , "�ł�"," dy a");
	boost::algorithm::replace_all(w , "�ł�"," dy u");
	boost::algorithm::replace_all(w , "�ł�"," dy o");
	boost::algorithm::replace_all(w , "�Ă�"," t i");
	boost::algorithm::replace_all(w , "�Ă�"," t e:");
	boost::algorithm::replace_all(w , "�Ă�"," ty a");
	boost::algorithm::replace_all(w , "�Ă�"," ty u");
	boost::algorithm::replace_all(w , "�Ă�"," ty o");
	boost::algorithm::replace_all(w , "����"," s i");
	boost::algorithm::replace_all(w , "����"," z u a");
	boost::algorithm::replace_all(w , "����"," z i");
	boost::algorithm::replace_all(w , "����"," z u");
	boost::algorithm::replace_all(w , "����"," zy a");
	boost::algorithm::replace_all(w , "����"," zy u");
	boost::algorithm::replace_all(w , "����"," zy o");
	boost::algorithm::replace_all(w , "����"," z e");
	boost::algorithm::replace_all(w , "����"," z o");
	boost::algorithm::replace_all(w , "����"," ky a");
	boost::algorithm::replace_all(w , "����"," ky u");
	boost::algorithm::replace_all(w , "����"," ky o");
	boost::algorithm::replace_all(w , "����"," sh a");
	boost::algorithm::replace_all(w , "����"," sh u");
	boost::algorithm::replace_all(w , "����"," sh e");
	boost::algorithm::replace_all(w , "����"," sh o");
	boost::algorithm::replace_all(w , "����"," ch a");
	boost::algorithm::replace_all(w , "����"," ch u");
	boost::algorithm::replace_all(w , "����"," ch e");
	boost::algorithm::replace_all(w , "����"," ch o");
	boost::algorithm::replace_all(w , "�Ƃ�"," t u");
	boost::algorithm::replace_all(w , "�Ƃ�"," ty a");
	boost::algorithm::replace_all(w , "�Ƃ�"," ty u");
	boost::algorithm::replace_all(w , "�Ƃ�"," ty o");
	boost::algorithm::replace_all(w , "�ǂ�"," d o a");
	boost::algorithm::replace_all(w , "�ǂ�"," d u");
	boost::algorithm::replace_all(w , "�ǂ�"," dy a");
	boost::algorithm::replace_all(w , "�ǂ�"," dy u");
	boost::algorithm::replace_all(w , "�ǂ�"," dy o");
	boost::algorithm::replace_all(w , "�ǂ�"," d o:");
	boost::algorithm::replace_all(w , "�ɂ�"," ny a");
	boost::algorithm::replace_all(w , "�ɂ�"," ny u");
	boost::algorithm::replace_all(w , "�ɂ�"," ny o");
	boost::algorithm::replace_all(w , "�Ђ�"," hy a");
	boost::algorithm::replace_all(w , "�Ђ�"," hy u");
	boost::algorithm::replace_all(w , "�Ђ�"," hy o");
	boost::algorithm::replace_all(w , "�݂�"," my a");
	boost::algorithm::replace_all(w , "�݂�"," my u");
	boost::algorithm::replace_all(w , "�݂�"," my o");
	boost::algorithm::replace_all(w , "���"," ry a");
	boost::algorithm::replace_all(w , "���"," ry u");
	boost::algorithm::replace_all(w , "���"," ry o");
	boost::algorithm::replace_all(w , "����"," gy a");
	boost::algorithm::replace_all(w , "����"," gy u");
	boost::algorithm::replace_all(w , "����"," gy o");
	boost::algorithm::replace_all(w , "����"," j e");
	boost::algorithm::replace_all(w , "����"," j a");
	boost::algorithm::replace_all(w , "����"," j u");
	boost::algorithm::replace_all(w , "����"," j o");
	boost::algorithm::replace_all(w , "����"," j e");
	boost::algorithm::replace_all(w , "����"," j a");
	boost::algorithm::replace_all(w , "����"," j u");
	boost::algorithm::replace_all(w , "����"," j o");
	boost::algorithm::replace_all(w , "�т�"," by a");
	boost::algorithm::replace_all(w , "�т�"," by u");
	boost::algorithm::replace_all(w , "�т�"," by o");
	boost::algorithm::replace_all(w , "�҂�"," py a");
	boost::algorithm::replace_all(w , "�҂�"," py u");
	boost::algorithm::replace_all(w , "�҂�"," py o");
	boost::algorithm::replace_all(w , "����"," u a");
	boost::algorithm::replace_all(w , "����"," w i");
	boost::algorithm::replace_all(w , "����"," w e");
	boost::algorithm::replace_all(w , "����"," w o");
	boost::algorithm::replace_all(w , "�ӂ�"," f a");
	boost::algorithm::replace_all(w , "�ӂ�"," f i");
	boost::algorithm::replace_all(w , "�ӂ�"," f u");
	boost::algorithm::replace_all(w , "�ӂ�"," hy a");
	boost::algorithm::replace_all(w , "�ӂ�"," hy u");
	boost::algorithm::replace_all(w , "�ӂ�"," hy o");
	boost::algorithm::replace_all(w , "�ӂ�"," f e");
	boost::algorithm::replace_all(w , "�ӂ�"," f o");

	//# 1������Ȃ�ϊ��K��
	boost::algorithm::replace_all(w , "��"," a");
	boost::algorithm::replace_all(w , "��"," i");
	boost::algorithm::replace_all(w , "��"," u");
	boost::algorithm::replace_all(w , "��"," e");
	boost::algorithm::replace_all(w , "��"," o");
	boost::algorithm::replace_all(w , "��"," k a");
	boost::algorithm::replace_all(w , "��"," k i");
	boost::algorithm::replace_all(w , "��"," k u");
	boost::algorithm::replace_all(w , "��"," k e");
	boost::algorithm::replace_all(w , "��"," k o");
	boost::algorithm::replace_all(w , "��"," s a");
	boost::algorithm::replace_all(w , "��"," sh i");
	boost::algorithm::replace_all(w , "��"," s u");
	boost::algorithm::replace_all(w , "��"," s e");
	boost::algorithm::replace_all(w , "��"," s o");
	boost::algorithm::replace_all(w , "��"," t a");
	boost::algorithm::replace_all(w , "��"," ch i");
	boost::algorithm::replace_all(w , "��"," ts u");
	boost::algorithm::replace_all(w , "��"," t e");
	boost::algorithm::replace_all(w , "��"," t o");
	boost::algorithm::replace_all(w , "��"," n a");
	boost::algorithm::replace_all(w , "��"," n i");
	boost::algorithm::replace_all(w , "��"," n u");
	boost::algorithm::replace_all(w , "��"," n e");
	boost::algorithm::replace_all(w , "��"," n o");
	boost::algorithm::replace_all(w , "��"," h a");
	boost::algorithm::replace_all(w , "��"," h i");
	boost::algorithm::replace_all(w , "��"," f u");
	boost::algorithm::replace_all(w , "��"," h e");
	boost::algorithm::replace_all(w , "��"," h o");
	boost::algorithm::replace_all(w , "��"," m a");
	boost::algorithm::replace_all(w , "��"," m i");
	boost::algorithm::replace_all(w , "��"," m u");
	boost::algorithm::replace_all(w , "��"," m e");
	boost::algorithm::replace_all(w , "��"," m o");
	boost::algorithm::replace_all(w , "��"," r a");
	boost::algorithm::replace_all(w , "��"," r i");
	boost::algorithm::replace_all(w , "��"," r u");
	boost::algorithm::replace_all(w , "��"," r e");
	boost::algorithm::replace_all(w , "��"," r o");
	boost::algorithm::replace_all(w , "��"," g a");
	boost::algorithm::replace_all(w , "��"," g i");
	boost::algorithm::replace_all(w , "��"," g u");
	boost::algorithm::replace_all(w , "��"," g e");
	boost::algorithm::replace_all(w , "��"," g o");
	boost::algorithm::replace_all(w , "��"," z a");
	boost::algorithm::replace_all(w , "��"," j i");
	boost::algorithm::replace_all(w , "��"," z u");
	boost::algorithm::replace_all(w , "��"," z e");
	boost::algorithm::replace_all(w , "��"," z o");
	boost::algorithm::replace_all(w , "��"," d a");
	boost::algorithm::replace_all(w , "��"," j i");
	boost::algorithm::replace_all(w , "��"," z u");
	boost::algorithm::replace_all(w , "��"," d e");
	boost::algorithm::replace_all(w , "��"," d o");
	boost::algorithm::replace_all(w , "��"," b a");
	boost::algorithm::replace_all(w , "��"," b i");
	boost::algorithm::replace_all(w , "��"," b u");
	boost::algorithm::replace_all(w , "��"," b e");
	boost::algorithm::replace_all(w , "��"," b o");
	boost::algorithm::replace_all(w , "��"," p a");
	boost::algorithm::replace_all(w , "��"," p i");
	boost::algorithm::replace_all(w , "��"," p u");
	boost::algorithm::replace_all(w , "��"," p e");
	boost::algorithm::replace_all(w , "��"," p o");
	boost::algorithm::replace_all(w , "��"," y a");
	boost::algorithm::replace_all(w , "��"," y u");
	boost::algorithm::replace_all(w , "��"," y o");
	boost::algorithm::replace_all(w , "��"," w a");
	boost::algorithm::replace_all(w , "��"," i");
	boost::algorithm::replace_all(w , "��"," e");
	boost::algorithm::replace_all(w , "��"," N");
	boost::algorithm::replace_all(w , "��"," q");
	boost::algorithm::replace_all(w , "�[",":");

	//# �����܂łɏ�������ĂȂ� ���������� �͂��̂܂ܑ啶������
	boost::algorithm::replace_all(w , "��"," a");
	boost::algorithm::replace_all(w , "��"," i");
	boost::algorithm::replace_all(w , "��"," u");
	boost::algorithm::replace_all(w , "��"," e");
	boost::algorithm::replace_all(w , "��"," o");
	boost::algorithm::replace_all(w , "��"," w a");
	boost::algorithm::replace_all(w , "��"," o");

	//# ���̑����ʂȃ��[��
    boost::algorithm::replace_all(w , "��"," o");

	//# �ϊ��̌��ʒ����L�����������Ƃ��܂�ɂ���̂ň�ɂ܂Ƃ߂�
	boost::algorithm::replace_all(w , ": : :",":");
	boost::algorithm::replace_all(w , ": :",":");

	//���イ -> ky u u -> ky u: �݂����� �A������ u u �� u:�ɂ���
	//http://julius.sourceforge.jp/sapi/Docs/develop.html
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
//		boost::algorithm::replace_all(w ,"����" , "���[");
	boost::algorithm::replace_all(w , " a a"," a:");
	boost::algorithm::replace_all(w , " i i"," i:");
	boost::algorithm::replace_all(w , " u u"," u:");
	boost::algorithm::replace_all(w , " e e"," e:");
	boost::algorithm::replace_all(w , " e i"," e:");
	boost::algorithm::replace_all(w , " o o"," o:");
	boost::algorithm::replace_all(w , " o u"," o:");

	//�O��̃X�y�[�X�����
	boost::algorithm::trim(w);

	return w;
};

