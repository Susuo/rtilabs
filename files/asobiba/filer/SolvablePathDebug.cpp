// SolvablePathDebug.cpp: SolvablePathDebug �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathDebug.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//���̃p�X���������Ă݂�
//�����ł����ꍇ�͉����������ʂ̃p�X 
//inPath www.yahoo.co.jp ---> return http://www.yahoo.co.jp/ �ȂǂƂȂ�
//�����ł��Ȃ������ꍇ�� "" ��Ԃ�.
bool SolvablePathDebug::DoSolvable(string inPath) 
{
	if (inPath == "debug") return true;

	return false;
}

