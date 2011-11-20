// SolvablePathSMBLogon.cpp: SolvablePathSMBLogon �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSMBLogon.h"
#include "SolvablePathSpecialFolder.h"
#include "RConv.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//���̃p�X���������Ă݂�
bool SolvablePathSMBLogon::DoSolvable(string inPath) 
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
		//������ smb:// �̏ꍇ�́A�h���C�����O�C���Ƃ��Ĉ���. �Ǝ��d�l(w
		if (inPath.substr(0,6) != "smb://")
		{
			return false;	//�ڂ�.
		}
		else
		{
			//�Ōオ \\ �łȂ��ꍇ�͍Ō�� \\ ��⊮���܂��B
			if ( MultiString::IsPathToComplement(inPath) )
			{
				inPath += "\\";
			}
			//�p�X�̐[���� 3�ȉ��ł��邱�Ƃ��K�{   substr �� �擪�� smb:// ����������.
			//smb://\\Microsoft Windows Network\\DOMAIN\\ .

			int dilimiter[MAX_DIMLIMITERES] ;
			int maxDilimiter = MultiString::setPathDelimiteres(inPath.c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
			if ( maxDilimiter >= 4)
			{
				// \\�}�V���� �Ƃ��ĕ]�����Ă݂܂�.
				return DoSolvable( string("\\") + inPath.substr(dilimiter[2]) );
			}
		}
	}
	else
	{
		//�Ōオ \\ �łȂ��ꍇ�͍Ō�� \\ ��⊮���܂��B
		if ( MultiString::IsPathToComplement(inPath) )
		{
			inPath += "\\";
		}

		//�p�X�̐[���� 1�ȉ��ł��邱�Ƃ��K�{   substr �� �擪�� \\ ����������.
		if ( MultiString::setPathDepth(inPath.substr(2) ) >= 2)
		{
			return false;	//�ڂ�
		}
	}
	this->setPath(inPath);

	//OK
	return true;
}

//��f�B���N�g�����オ��
string SolvablePathSMBLogon::UpDirectory() const
{
	const string path = getPath() ;

	//�擪�� \\ ��������...
	if (path.substr(0,2) == "\\\\")
	{	//���L�̏����擾���܂��B

		//�Ō�� \\ �����.
		string remoteName = path.substr( 0 , path.size() - 1);

		NETRESOURCE srcNr;
		srcNr.lpProvider = "Microsoft Windows Network";
		srcNr.dwType = RESOURCETYPE_DISK;
		srcNr.lpRemoteName = (char*) remoteName.c_str();

		DWORD bufSize = MAX_PATH * 10;	//�K��
		NETRESOURCE *destNr = (NETRESOURCE*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, bufSize);

		if ( WNetGetResourceParent( &srcNr , destNr , &bufSize) != NO_ERROR)
		{
			GlobalFree(destNr);
			//�擾�Ɏ��s.. �ǂ�����? �}�C�l�b�g���[�N�ɂł��߂���?
			return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_NETWORK);
		}

		string ret = string("smb://\\") + destNr->lpProvider + string("\\") + destNr->lpRemoteName + string("\\");

		GlobalFree(destNr);

		return ret;
	}

	//������ smb:// �̏ꍇ�́A�h���C�����O�C���Ƃ��Ĉ���. �Ǝ��d�l(w
	//��ŁAsmb:// �ȏ�ɂ�����ꍇ�� �}�C�l�b�g���[�N�ɔ�΂�.
	if (path.substr(0,6) == "smb://" && MultiString::setPathDepth(path) == 1)
	{
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_NETWORK);
	}

	const int lastpos = MultiString::getLastDirectoryPos( path );
	return path.substr( 0 , lastpos + 1) ;
}

void SolvablePathSMBLogon::Test()
{
	SolvablePathSMBLogon sp;

	//�p�X�������ł��邩?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


