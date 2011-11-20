// MVThreadManager.cpp: MVThreadManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVThreadManager.h"
#include "RWaitCursor.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MVThreadManager::MVThreadManager()
{
	Now.Run = false;
	Now.Image = NULL;
	Now.Page = -1;

	After.Run = false;
	After.Image = NULL;
	After.Page = -1;

	Before.Run = false;
	Before.Image = NULL;
	Before.Page = -1;

}

MVThreadManager::~MVThreadManager()
{
	AllThreadDestory();
}

/**
 * Update:				�V�����{���J��
 *
 * @param inFilename	�t�@�C����
 * @return void 
 */
void MVThreadManager::Update(const string & inFilename) throw(RException)
{
	RWaitCursor wait;

	//���ׂẴX���b�h���~������.
	AllThreadDestory();

	FileList.Update(inFilename);

}

//�f�[�^�̓ǂݍ��݂����͏I����Ă��Ȃ��ꍇ�͏I���܂ő҂�.
void MVThreadManager::ThreadJoin(MVThreadManagerImagCache* ioThread)
{
	try
	{
		if (ioThread->Run)
		{
			ioThread->Thread.Join();
			ioThread->Image = ioThread->Thread.TakeoverPageData();

			ioThread->Run = false;
		}
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		delete ioThread->Image;
		ioThread->Image = NULL;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

//�f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
void MVThreadManager::ThreadAbort(MVThreadManagerImagCache* ioThread)
{
	try
	{
		if (ioThread->Run)
		{
			ioThread->Thread.HoldUp();
			ioThread->Thread.Join();
			ioThread->Image = ioThread->Thread.TakeoverPageData();

			ioThread->Run = false;
		}
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		delete ioThread->Image;
		ioThread->Image = NULL;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

//�X���b�h�̊J�n����.
void MVThreadManager::ThreadRun(int inPage , MVThreadManagerImagCache* ioThread)
{
	//���ꂩ��d�����˗�����̂ɖZ�����Ăǂ������?
	ASSERT(ioThread->Run == false);

	ASSERT(ioThread->Image == NULL);

	//�͈͊O�̃y�[�W�̓ǂݍ��݂̓_��.
	if (inPage < 0 || inPage >= this->FileList.getCount() )
	{
		ioThread->Page = -1;
		return ;
	}

	try
	{
		//�X���b�h�ɕK�v�ȏ����L�^���Ă��炢�܂�.
		ZipReadInfo zipInfo;
		FileList.getInfo(inPage , &zipInfo );
		
		//�d�����˗�����.
		ioThread->Run = true;
		ioThread->Page = inPage;

		ioThread->Thread.Setting(	zipInfo.buffer , zipInfo.compressedSize , 
										zipInfo.uncompressedSize , zipInfo.compressionMethod , 
										&this->ImageConvertParam );

		//�X���b�h����
		ioThread->Thread.Start();

		TRACE("�X���b�h %p �� %d�y�[�W�̎d�����˗����܂���\n" , ioThread , inPage);
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

/**
 * GotoPage:			�w�肳�ꂽ�y�[�W�Ɉړ�
 *
 * @param inPage				�y�[�W�ԍ�
 * @return const MVPageData*	�y�[�W�̃f�[�^.
 *
 */
const MVPageData* MVThreadManager::GotoPage(int inPage) throw(RException)
{
	//���̉摜��ǂݍ��ݒ��͂��肦�Ȃ�.
	ASSERT(Now.Run == false);

	//�����|�C���^���w���Ă͂����Ȃ�. double free ���Ă��܂�.
	ASSERT((Now.Image == NULL) || (Now.Image != After.Image) );
	ASSERT((Now.Image == NULL) || (Now.Image != Before.Image) );
	//�ł��A NULL �Ȃ�A�Ԃ����Ă��������Ȃ���.
	ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
	//�����y�[�W���w���͂����Ȃ�.
	ASSERT( (Now.Page == -1) || (Now.Page != After.Page) );
	ASSERT( (Now.Page == -1) || (Now.Page != Before.Page) );
	ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );

	//�� �̃f�[�^
	if (Now.Page == inPage)
	{
		//���������ɁA���̃f�[�^�𑗐M.

		//�����|�C���^���w���Ă͂����Ȃ�. double free ���Ă��܂�.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//�ł��A NULL �Ȃ�A�Ԃ����Ă��������Ȃ���.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//�����y�[�W���w���͂����Ȃ�.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//�v�����ꂽ�y�[�W�� �� �̃y�[�W
	else if (After.Page == inPage)
	{
		//���̃f�[�^�̓ǂݍ��݂����͏I����Ă��Ȃ��ꍇ�͏I���܂ő҂�.
		ThreadJoin(&After);
		//�O�̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
		ThreadAbort(&Before);
		ASSERT(After.Run == false);
		ASSERT(Before.Run == false);

		//	�O �̃f�[�^��j��.
		delete Before.Image;
		Before.Image = NULL;

		//�� �̃f�[�^�� �O �̃f�[�^�Ƃ��ċL�^.
		Before.Image = Now.Image;
		Before.Page = Now.Page;
		ASSERT(Now.Image == Before.Image);
		ASSERT(Now.Page == Before.Page);
		ASSERT(Before.Run == false);

		//�� �̃f�[�^�� �� �̃f�[�^�Ƃ��ċL�^
		Now.Image = After.Image;
		Now.Page = After.Page;
		ASSERT(Now.Image == After.Image);
		ASSERT(Now.Page == After.Page);
		ASSERT(Now.Run == false);

		//�� �̃f�[�^�̐�ǂ݂��J�n����.
		After.Image = NULL;
		ThreadRun(inPage + 1 , &After);

		//�����|�C���^���w���Ă͂����Ȃ�. double free ���Ă��܂�.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//�ł��A NULL �Ȃ�A�Ԃ����Ă��������Ȃ���.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//�����y�[�W���w���͂����Ȃ�.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//�v�����ꂽ�y�[�W�� �O �̃y�[�W
	else if (Before.Page == inPage)
	{
		//�O�̃f�[�^�̓ǂݍ��݂����͏I����Ă��Ȃ��ꍇ�͏I���܂ő҂�.
		ThreadJoin(&Before);
		//���̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
		ThreadAbort(&After);

		//�� �̃f�[�^��j��.
		delete After.Image;
		After.Image = NULL;

		//�� �̃f�[�^�� �� �̃f�[�^�Ƃ��ċL�^.
		After.Image = Now.Image;
		After.Page = Now.Page;
		ASSERT(Now.Image == After.Image);
		ASSERT(Now.Page == After.Page);
		ASSERT(After.Run == false);

		//�O �̃f�[�^�� �� �̃f�[�^�Ƃ��ċL�^
		Now.Image = Before.Image;
		Now.Page = Before.Page;
		ASSERT(Now.Image == Before.Image);
		ASSERT(Now.Page == Before.Page);
		ASSERT(Now.Run == false);

		//�O �̃f�[�^�̐�ǂ݂��J�n����.
		Before.Image = NULL;
		ThreadRun(inPage - 1 , &Before);

		//�����|�C���^���w���Ă͂����Ȃ�. double free ���Ă��܂�.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//�ł��A NULL �Ȃ�A�Ԃ����Ă��������Ȃ���.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//�����y�[�W���w���͂����Ȃ�.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//�O �ɂ� �� �ɂ������Ă��Ȃ��摜
	else 
	{
		//���̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
		ThreadAbort(&After);
		//�O�̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
		ThreadAbort(&Before);
		ASSERT(After.Run == false);
		ASSERT(Before.Run == false);

		//���̃f�[�^��j��.
		delete Now.Image;
		Now.Image = NULL;
		//���̃f�[�^�����[�h
		ThreadRun(inPage  , &Now );

		//�O�̃f�[�^��j���A���̃f�[�^���j��. 
		delete Before.Image;
		Before.Image = NULL;
		delete After.Image;
		After.Image = NULL;

		//�O�Ǝ��̃f�[�^�̐�ǂ݂��J�n����.
		After.Image = NULL;
		ThreadRun(inPage + 1 , &After  );
		Before.Image = NULL;
		ThreadRun(inPage - 1 , &Before );

		//���̃f�[�^�̓ǂݍ��݂��I���܂ő҂�.
		ThreadJoin(&Now);

		//�����|�C���^���w���Ă͂����Ȃ�. double free ���Ă��܂�.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//�ł��A NULL �Ȃ�A�Ԃ����Ă��������Ȃ���.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//�����y�[�W���w���͂����Ȃ�.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}

	//���̉摜��ǂݍ��ݒ��͂��肦�Ȃ�.
	ASSERT(Now.Run == false);
	ASSERT(Now.Image != NULL);
	ASSERT(Now.Thread.IsAlive() == false);


	return Now.Image;
}


//���ׂẴX���b�h���~������.
void MVThreadManager::AllThreadDestory()
{
	//���̉摜��ǂݍ��ݒ��͂��肦�Ȃ�.
	ASSERT(Now.Run == false);

	//���̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
	ThreadAbort(&After);
	//�O�̃f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
	ThreadAbort(&Before);
	ASSERT(After.Run == false);
	ASSERT(Before.Run == false);

	delete Now.Image;
	Now.Image = NULL;
	Now.Page = -1;

	delete After.Image;
	After.Image = NULL;
	After.Page = -1;

	delete Before.Image;
	Before.Image = NULL;
	Before.Page = -1;
}


/**
 * ChangeOption:		�ݒ�̕ύX
 *
 * @param inImageConvertParam	�ϊ��Ɏg�p����p�����[�^
 * @return void 
 */
void MVThreadManager::ChangeOption(const MVImagePreConvert* inImageConvertParam )
{
	bool threadReload = true ;
	if ( this->ImageConvertParam.getScrollType() != inImageConvertParam->getScrollType() )
	{
		//�ǂݒ����K�v.
	}
	else if (	this->ImageConvertParam.getJustFit() == false && 
				this->ImageConvertParam.getJustFit() == inImageConvertParam->getJustFit() )
	{
		//���Ƃ��ƃW���X�g�t�B�b�g���Ȃ��ŁA��Ԃ��ς��Ȃ�
		threadReload = false;
	}
	else if (this->ImageConvertParam.getJustFit() == true && 
			 this->ImageConvertParam.getJustFit() == inImageConvertParam->getJustFit()  )
	{
		switch( this->ImageConvertParam.getScrollType() )
		{
		case MVConfig::YOKO:
			if ( this->ImageConvertParam.getScreenHeight() == inImageConvertParam->getScreenHeight() )
			{
				//���Ƃ��ƃW���X�g�t�B�b�g���� �����ǁA�������ς��Ȃ�
				threadReload = false;
			}
			break;
		case MVConfig::TATE:
			if ( this->ImageConvertParam.getScreenWidth() == inImageConvertParam->getScreenWidth() )
			{
				//���Ƃ��ƃW���X�g�t�B�b�g���� �����ǁA�������ς��Ȃ�
				threadReload = false;
			}
			break;
		}
	}

	//�X���b�h��ǂݒ����K�v������Γǂݒ���..
	if (threadReload)
	{
		//���ׂẴX���b�h���~������.
		AllThreadDestory();
	}

	this->ImageConvertParam = *inImageConvertParam;
}


