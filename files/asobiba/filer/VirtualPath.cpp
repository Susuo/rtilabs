// VirtualPath.cpp: VirtualPath �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "VirtualPath.h"

#define iskanji1st(c) (((c) >= 0x81 && (c) <= 0x9f) || ((c) >= 0xe0 && (c) <= 0xfc))
#define iskanji2nd(c) ((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

VirtualPath::VirtualPath(string inPath)
{
	const char* buf = inPath.c_str();
	int count = 0;
	for(int i = 0; buf[i] != '\0' ; i++)
	{
		unsigned char c = (unsigned char) buf[i];
		if (iskanji1st(c))
		{
			c = (unsigned char) buf[i+1];
			if (iskanji2nd(c))
			{
				// 2 byte skip
				i++;
			}
		}
		else
		{
			//���z�p�X�̋�؂� :: ���T�[�`.
			if (c == ':' && buf[i + 1] == ':' )
			{
				if ( count >= (MAX_NEXT_VIRTUAL_PATH-1) )			break;

				PathDilimiteres[count++] = i;

				// ::�� 2�o�C�g�Ȃ̂ŁA1�o�C�g�X�L�b�v���܂��B
				i++;
			}
		}
	}
	PathDilimiteres[count] = inPath.length();
	DilimiteresMax = count ;

	Path = inPath;

}

VirtualPath::~VirtualPath()
{

}

//�e�X�g
void VirtualPath::test()
{
	VirtualPath		vp("c:\\test3\\a.lzh::�͂��߂Ă̂��邷�΂�/�����肽��i�G�L�D`�jʧʧ.zip::�����[��݂��ガ�Ղꂢ.png");
	ASSERT( vp.getPathDilimiterCount() == 2 );
	ASSERT( vp.getNativePath() == "c:\\test3\\a.lzh" );
	ASSERT( vp.getLastPath() == "�����[��݂��ガ�Ղꂢ.png" );
	ASSERT( vp.getAdjacentPath() == "�͂��߂Ă̂��邷�΂�/�����肽��i�G�L�D`�jʧʧ.zip" );
	ASSERT( vp.getSubPath(0) == "c:\\test3\\a.lzh" );
	ASSERT( vp.getSubPath(1) == "�͂��߂Ă̂��邷�΂�/�����肽��i�G�L�D`�jʧʧ.zip" );
	ASSERT( vp.getSubPath(2) == "�����[��݂��ガ�Ղꂢ.png" );

}


