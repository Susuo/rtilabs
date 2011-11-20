// SolvableFile.cpp: SolvableFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFile.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//��f�B���N�g�����オ��
string SolvableFile::UpDirectory() const 
{
	string path = getPath();

	const int lastpos = MultiString::getLastDirectoryPos(path);
	string ret = path.substr( 0 , lastpos + 1) ;
	return ret;
}
