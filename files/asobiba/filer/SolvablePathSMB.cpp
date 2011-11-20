// SolvablePathSMB.cpp: SolvablePathSMB �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSMB.h"
#include "RConv.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvablePathSMB::DoSolvable(string inPath) 
{
	//�擪�� file:/// �������ꍇ�A���̕������J�b�g.
	if ( inPath.substr(0,8) == "file:///" )
	{
		inPath = inPath.substr(8);
		// / �� \\ �ɂ���.
		replace( inPath.begin() , inPath.end() , '/','\\');
		//UrlEncode ����菜��.
		inPath = RConv::UrlEncode(inPath);
	}

	//�擪�� \\ �łȂ���΃_��
	if (inPath.substr(0,2) != "\\\\")
	{
		return false;
	}

	//�Ōオ \\ �łȂ��ꍇ�͍Ō�� \\ ��⊮���܂��B
	if ( MultiString::IsPathToComplement(inPath) )
	{
		inPath += "\\";
	}

	//�p�X�̐[���� 2�ȏ゠�邱�Ƃ��K�{   substr �� �擪�� \\ ����������.
	if ( MultiString::setPathDepth(inPath.substr(2) ) < 2)
	{
		return false;
	}

	this->setPath(inPath);

	//OK
	return true;
}

//��f�B���N�g�����オ��
string SolvablePathSMB::UpDirectory() const
{
	const string path = getPath() ;
	const int lastpos = MultiString::getLastDirectoryPos( path );
	return path.substr( 0 , lastpos + 1) ;
}

void SolvablePathSMB::Test()
{
	SolvablePathSMB sp;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


