/*
julius ��MS-SAPI���x�����炢�Ɋy�Ɏg�����߂ɁA��X�͂����ƃn�b�N�����Ȃ���΂Ȃ�Ȃ��B


julius�u�}�i�̖{���̗͂��v���m��!!�v
*/
#include <Windows.h>
#include "julius-plus/rule.h"
#include "julius-plus/JuliusPlus.h"



int main()
{
//	_CrtSetBreakAlloc(447);
	{
//julius�t���� mkdfa �𒊏o���Ă݂��B
//�������A�I���W�i�������������������[���[�N������̂Ƃ� static�ϐ����g���܂����Ă���̂Ŋ֐�������Ӗ��͂Ȃ��Ǝv���邪�B
//mkdf.pl���g��Ȃ��ōςނ̂ŁAwindows�n�ɂ͊����������ˁB
		JuliusPlus julius;
		julius.AddRegexp(0,"�e�X�g(������|�Ȃ̂�)����",&julius.Grammer);

		std::ofstream dfa("regexp_test.dfa");
		std::ofstream dict("regexp_test.dict");
		julius.RuleCommit(&dfa,&dict);
	}

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
return 0;
}