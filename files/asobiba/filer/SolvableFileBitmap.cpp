// SolvableFileBitmap.cpp: SolvableFileBitmap �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileBitmap.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvableFileBitmap::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}

void SolvableFileBitmap::Test()
{
	SolvableFileBitmap sf;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


