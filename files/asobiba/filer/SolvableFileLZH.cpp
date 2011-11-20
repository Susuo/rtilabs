// SolvableFileLZH.cpp: SolvableFileLZH �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileLZH.h"
#include "VirtualPath.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
//���̃p�X���������Ă݂�
bool SolvableFileLZH::DoSolvable(string inPath) 
{
	//�l�X�g���Ă��邩������Ȃ��̂Œ��ׂ�.
	VirtualPath		vp(inPath);
	if (vp.getPathDilimiterCount() == 0) 
	{	//�ŏ��Ȃ̂� / ��t��
		this->setPath(inPath + "::\\" );
	}
	else
	{
		//�Ōオ \\ �łȂ��ꍇ�͍Ō�� \\ ��⊮���܂��B
		if ( MultiString::IsPathToComplement(inPath) )
		{
			inPath += "\\";
		}
		this->setPath(inPath);
	}

	//OK
	return true;
}


void SolvableFileLZH::Test()
{
	SolvableFileLZH sf;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


