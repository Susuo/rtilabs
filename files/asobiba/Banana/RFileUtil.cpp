// RFileUtil.cpp: RFileUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RFileUtil.h"
#include "RStdioFile.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RFileUtil::RFileUtil()
{

}

RFileUtil::~RFileUtil()
{

}

//�s���𐔂���.
int RFileUtil::LineCount(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	int i;
	for( i = 0  ; !file.IsEof() ; i++)
	{
		string r = file.ReadLine();
	}
	return i;
}


void RFileUtil::test()
{
	//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
	//�e�X�g���[�`���͓������܂���
	/*
	int r;
	ASSERT((r = RFileUtil::LineCount("test/01102SK.sal")) == 266);
	ASSERT((r = RFileUtil::LineCount("test/test.cm")) == 3);
	*/
}

