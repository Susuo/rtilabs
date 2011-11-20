// SolvablePathLocal.cpp: SolvablePathLocal �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathLocal.h"
#include <ctype.h>
#include "RConv.h"
#include "MultiString.h"
#include "SolvablePathSpecialFolder.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvablePathLocal::DoSolvable(string inPath) 
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

	//�h���C�u���^�[�͍Œ�1�����K�v.
	if (inPath.size() < 1)	return false;

	//�h���C�u���^�[�� a-z A-Z
	if ( ! isalpha (inPath[0]) ) return false;

	//c: �� : �̕�������?
	if ( inPath[1] != ':' ) 
	{
		if (inPath.size() != 1) return false;	//�ڂ�.

		// c �P�̂������� c: �ƕ⊮
		inPath += ":";
	}
	//�Ōオ \\ �łȂ��ꍇ�͍Ō�� \\ ��⊮���܂��B
	if ( MultiString::IsPathToComplement(inPath) )
	{
		inPath += "\\";
	}
	this->setPath(inPath);

	//OK
	return true;
}

//��f�B���N�g�����オ��
string SolvablePathLocal::UpDirectory() const
{
	//�p�X�� c:\ ���Œ�� �Ō�͂��Ȃ炸 \ ���͂���Ƃ�������
	//����𗘗p���܂��B

	string path = getPath();
	if (path.length() <= 3)
	{//�h���C�u�ꗗ��
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_DRIVES);
	}
	const int lastpos = MultiString::getLastDirectoryPos(path);
	string ret = path.substr( 0 , lastpos + 1) ;
	return ret;
}

void SolvablePathLocal::Test()
{
	SolvablePathLocal sp;

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:\\") );
	ASSERT( sp.getPath() == "c:\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:") );
	ASSERT( sp.getPath() == "c:\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c") );
	ASSERT( sp.getPath() == "c:\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:\\GAME\\�����̂Ƃ���") );
	ASSERT( sp.getPath() == "c:\\GAME\\�����̂Ƃ���\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:\\GAME\\�����̂Ƃ���\\") );
	ASSERT( sp.getPath() == "c:\\GAME\\�����̂Ƃ���\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:\\GAME\\�����̂Ƃ���\\kanoso") );
	ASSERT( sp.getPath() == "c:\\GAME\\�����̂Ƃ���\\kanoso\\" );

	//�p�X�������ł��邩?
	ASSERT( sp.DoSolvable("c:\\�\\\�\\\���\\\��������") );
	ASSERT( sp.getPath() == "c:\\�\\\�\\\���\\\��������\\" );

	ASSERT( sp.DoSolvable("file:///c:/") );
	ASSERT( sp.getPath() == "c:\\" );

	ASSERT( sp.DoSolvable("file:///C:/Documents%20and%20Settings/Administrator/�f�X�N�g�b�v/") );
	ASSERT( sp.getPath() == "C:\\Documents and Settings\\Administrator\\�f�X�N�g�b�v\\" );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("http://www.yahoo.co.jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("ftp://www.yahoo.co.jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("://www.yahoo.co.jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("//www.yahoo.co.jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("www.yahoo.co.jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable(".jp") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("�f�X�N�g�b�v") );

	//�p�X�������Ɏ��s���邩?
	ASSERT( ! sp.DoSolvable("�l�b�g���[�N�R���s���[�^") );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\Windows") );
	ASSERT( sp.getPath() == "c:\\Windows\\" );
	ASSERT( sp.UpDirectory() == "c:\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\GAME\\�����̂Ƃ���\\kanoso") );
	ASSERT( sp.getPath() == "c:\\GAME\\�����̂Ƃ���\\kanoso\\" );
	ASSERT( sp.UpDirectory() == "c:\\GAME\\�����̂Ƃ���\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\GAME\\�����̂Ƃ���") );
	ASSERT( sp.getPath() == "c:\\GAME\\�����̂Ƃ���\\" );
	ASSERT( sp.UpDirectory() == "c:\\GAME\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\�\\\�\\\���\\\��������") );
	ASSERT( sp.getPath() == "c:\\�\\\�\\\���\\\��������\\" );
	ASSERT( sp.UpDirectory() == "c:\\�\\\�\\\���\\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\�\\\�\\\���\") );
	ASSERT( sp.getPath() == "c:\\�\\\�\\\���\\\" );
	ASSERT( sp.UpDirectory() == "c:\\�\\\�\\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\�\\\�\\\") );
	ASSERT( sp.getPath() == "c:\\�\\\�\\\" );
	ASSERT( sp.UpDirectory() == "c:\\�\\\" );

	//��ɏオ��
	ASSERT( sp.DoSolvable("c:\\�\") );
	ASSERT( sp.getPath() == "c:\\�\\\" );
	ASSERT( sp.UpDirectory() == "c:\\" );
}


