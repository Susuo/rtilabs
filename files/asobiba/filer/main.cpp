#include "comm.h"
#include "RTableMuchDuplication.h"
#include "Owners.h"
#include "RfilesLocal.h"
#include "RfilesPreviewOne.h"
#include "SolvablePathLocal.h"
#include "RSyncFileCopy.h"
#include "RTableEasy.h"
#include "AddressHistory.h"
#include "RConv.h"
#include "RfilesLzh.h"
#include "RfilesWithCache.h"
#include "VirtualPath.h"
#include "WindowsMain.h"
#include "RExpr.h"




int main()
{


	puts("Welcome to the SuperFiler!! yah!");

	int ret = -1;
//	try
	{
/*
		//���z�p�X.
		VirtualPath::test();
		//�d���̑����\�[�g
		RTableMuchDuplication<string>::test();
		//�P���ȃ\�[�g
		RTableEasy::test();
		//�t�@�C���̏��L�҃��X�g
		Owners::test();
		//���[�J������̃t�@�C���̎擾
		RfilesLocal::test();
		//�P��t�@�C��
		RfilesPreviewOne::test();
		//LZH����Q�b�g.
		RfilesLzh::test();
		//�L���b�V���e�X�g
		RfilesWithCache::test();
		//�q�X�g���[�֌W
		AddressHistory::test();
		//�p�X����.
		SolvablePathLocal::Test();
		//�R�s�[�e�X�g.
		RSyncFileCopy::Test();
		//������ϊ����[�`��
		RConv::test();
		//��������
		RExprToken::test();
		//�v�Z.
		RExpr::test();
*/
		//Windows�̎��s��
		ret = WindowsMain::getMainSystem()->Run();
	}
	/*
	catch(RException e)
	{
		TRACE("�����s���̃G���[���������܂���\n�u%s�v\n", e.getMessage() );	
		return -100;
	}
	catch(...)
	{
		TRACE("�V�X�e���܂��� C++ �̗�O���������܂���");
		return -200;
	}
	*/
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return ret;
}

