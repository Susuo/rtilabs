// SolvableFilePDF.cpp: SolvableFilePDF �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFilePDF.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvableFilePDF::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}


void SolvableFilePDF::Test()
{
	SolvableFilePDF sf;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


