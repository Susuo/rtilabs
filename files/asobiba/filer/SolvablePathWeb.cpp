// SolvablePathWeb.cpp: SolvablePathWeb �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathWeb.h"
#include "SolvablePathSpecialFolder.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



//���̃p�X���������Ă݂�
bool SolvablePathWeb::DoSolvable(string inPath) 
{
	//�擪�� http:// �ł��邱��
	if ( inPath.substr(0,7) != "http://" && inPath.substr(0,7) != "https://" )
	{
		//�h���C���ۂ������O�ł���Ε⊮����....

		if (inPath.size() <= 3) return false; //����ȒZ���̂͂Ȃ�. a.to ���Z���̂���?

		if (inPath[1] == ':') return false;	//�h���C�u���^�[?

		if (inPath.find("://") != -1) return false;	// :// ��������Ă��Ƃ͂Ȃ񂩕ʂ̃v���g�R��?

		if (inPath.substr(0,2) == "\\\\")	return false;	//���ꂾ�����狤�L.

		if (inPath.find(".") == -1) return false;	// . ���Ȃ��h���C���Ȃ�ĂȂ��ł��傤.

		//�����܂ł����炱�����h���C���Ƃ��ĔF��.

		inPath = string("http://") + inPath;
	}

	this->setPath(inPath);

	//OK
	return true;
}

//��f�B���N�g�����オ��
string SolvablePathWeb::UpDirectory() const
{
	const string path = getPath() ;

	int protcolSkip = path.find("://");	//http:// ���X�L�b�v.
	if (protcolSkip == -1 ) protcolSkip = 0;	//�o�O?
	else					protcolSkip += sizeof("://");

	const int lastpos = MultiString::getLastDirectoryPos( path.substr( protcolSkip ), '/');
	if (lastpos == 0)
	{	//�g�b�v�炵���̂ŁA�f�X�N�g�b�v�ɖ߂�.
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_DESKTOP);
	}

	return path.substr( 0 , protcolSkip + lastpos + 1) ;
}

void SolvablePathWeb::Test()
{
	SolvablePathWeb sp;

	//�p�X�������������邩.
	ASSERT( sp.DoSolvable("http://www.yahoo.co.jp") );
	ASSERT( sp.getPath() == "http://www.yahoo.co.jp" );

}


