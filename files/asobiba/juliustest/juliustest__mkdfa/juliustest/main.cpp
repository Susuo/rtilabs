/*
julius ��MS-SAPI���x�����炢�Ɋy�Ɏg�����߂ɁA��X�͂����ƃn�b�N�����Ȃ���΂Ȃ�Ȃ��B


julius�u�}�i�̖{���̗͂��v���m��!!�v
*/
#include <Windows.h>
#include "julius-plus/mkdfa.h"



int main()
{
//	_CrtSetBreakAlloc(447);
	{
//julius�t���� mkdfa �𒊏o���Ă݂��B
//�������A�I���W�i�������������������[���[�N������̂Ƃ� static�ϐ����g���܂����Ă���̂Ŋ֐�������Ӗ��͂Ȃ��Ǝv���邪�B
//mkdf.pl���g��Ȃ��ōςނ̂ŁAwindows�n�ɂ͊����������ˁB
		mkdfa mkdfa;
		mkdfa.conv("fruit");
	}

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
return 0;
}