// SolvableFileText.cpp: SolvableFileText �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileText.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvableFileText::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}

void SolvableFileText::Test()
{
	SolvableFileText sf;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


