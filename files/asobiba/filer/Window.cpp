// Window.cpp: Window �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Window.h"
#include "RTaskManager.h"
#include "SolvablePathManager.h"
#include "SolvableFileManager.h"
#include "DisplayFactory.h"
#include "MultiString.h"
#include "RTaskLS.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Window::Window(FilerSub* inFilerSub,string inPath)
{
	Files = NULL;
	ParentFSub = inFilerSub;

	//Blank�y�[�W.
	Disp = DisplayFactory::New(__DISPLAYTYPE_BLANK, this ,__RFILETYPE_NONE_DUMMY);
	if ( ! this->Preview(inPath , true) )
	{
		this->PreviewOrLs(inPath , true);
	}
}

Window::~Window()
{
	if (this->Files) 
	{
		ASSERT( this->Files->Dec() );
	}

	if (Disp) Disp->Dec();
}


//�A�N�e�B���ɂȂ�܂���.
void Window::OnActive()
{
	if (Disp)		
	{
		//�A�N�e�B��
		Disp->OnActive();
	}
}

//��A�N�e�B���ɂȂ�܂���.
void Window::OnNoActive() const
{
	if (Disp)		Disp->OnNoActive();
}

//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
void Window::GetWindowRect(RECT * outRC) const
{
	Disp->GetWindowRect(outRC);
}


//�A�b�v�f�[�g���܂��B
void Window::UpdateWindow(Rfiles* inFiles , Display* inDisp)
{
	//�����A�f�B�X�v���C���X�V�����ꍇ...
	if (inDisp)
	{
		//���ݕێ����Ă���f�B�X�v���C�ƈႤ���Ƃ��m�F����
		if (inDisp != this->Disp)
		{
			//���݃f�B�X�v���C�j��.
			if (Disp) Disp->Dec();
			//�ύX.
			this->Disp = inDisp;
		}
	}

	//�����A�X�V����t�@�C���V�X�e�����L���Ȃ�΁A�X�V����.
	if (inFiles)
	{
		//���ݕێ����Ă���t�@�C���V�X�e���ƈႤ���Ƃ��m�F����
		if (inFiles != this->Files)
		{
			//���ݕێ����Ă���t�@�C���V�X�e���ւ̎Q�ƃJ�E���g�����炷�B
			if (this->Files) Files->Dec();
			//�ύX.
			this->Files = inFiles;
		}
	}

	if (this->Files && this->Disp)
	{
		//�f�B�X�v���C�̃f�[�^�̍X�V.
		const RTable* rt = this->Files->getTable();
		this->Disp->setRfileItems( rt->getAllSize() , rt->getAllList() );

		//���݃A�N�e�B����������A�O�̂��� Display���X�V���Ă����B
		if ( ParentFSub->IsActiveWindow(this) )			ParentFSub->setActiveWindow(this);
	}
}

//�f�B���N�g������オ��.
void Window::UpDirectory()
{
	if (Files)
	{
		//�p�X�������ł���I�u�W�F�N�g�̍쐬.
		SolvablePathManager* spm = SolvablePathManager::getInstance();
		SolvablePath* tws = spm->DoSolvable( getNowPath() );
		if (!tws)	return ;

		this->PreviewOrLs( tws->UpDirectory() );
	}
}


//���ɂ����?
bool Window::getIsLeft() const
{
	return ParentFSub->getIsLeft();
}

//�߂�!
void Window::HistoryBack()
{
	try
	{	//Scan ���g���Ɨ����̎c���Ă��܂��̂ŁA
		PreviewOrLs( History.Back()  );
	}
	catch(RNotFoundException e)
	{
		//�߂�܂���ł���
		printf("error : %s\n" , e.getMessage() );
	}
}

//�i��!
void Window::HistoryNext()
{
	try
	{	//Scan ���g���Ɨ����̎c���Ă��܂��̂ŁA
		PreviewOrLs( History.Next() );
	}
	catch(RNotFoundException e)
	{
		//�i�߂܂���ł���
		printf("error : %s\n" , e.getMessage() );
	}
}



//�f�B���N�g���X�L����.
bool Window::ls(string inPath,bool isNOTRecodeHistory) 
{
	Rfiles* files = NULL;
	Display* disp = Disp;
	try
	{
		//�p�X�������ł���I�u�W�F�N�g�̍쐬.
		SolvablePathManager* spm = SolvablePathManager::getInstance();
		SolvablePath* tws = spm->DoSolvable(inPath);
		if (tws == NULL) 
			throw RNotSupportException("�p�X %s �͉����ł��܂���ł���" ,inPath.c_str() );
		//���ݕ\�����Ă���p�^�[���ƈႤ�ꍇ�͂قڃf�B�X�v���C�̍�蒼�����K�v�ł��B
		if ( Disp->getNowNavigateType() != tws->getType())
		{
			if ( !tws->IsDisplaySupported( Disp->getType() ))
			{//�T�|�[�g����Ă��Ȃ��̂ō�蒼���ł��B
				//�f�B�X�v���C�쐬.
				disp = DisplayFactory::New( tws->WhatIsDisplayeRecommendedMost() , this , tws->getType() );
				//�J�����̓o�^.
				disp->setFixedCloumn( tws->LetKnowColumnesInDisplayRecommendedMost() );
			}
		}

		//���ۂɃt�@�C���V�X�e���𑀍삷�� Rfiles ���쐬.
		files = tws->RfilesFactory( disp->getType() );
		try
		{
			//�\�[�g�@�\�̍쐬.
			files->MakeTable(__FILECOLUMN_TYPE_EXT , true );
		}
		catch(RNotSupportException e)
		{	//���s������A���O�Ń\�[�g.
			files->MakeTable(__FILECOLUMN_TYPE_NAME , true );
		}

		//�^�X�N�}�l�[�W���[�ɓo�^.
		RTaskManager * rtm = RTaskManager::getInstance();
		//	void ls(Rfiles* inFiles ,Display* inDisp,  Window* inCallback);
		rtm->putTask( new RTaskLS(files ,Mask , disp , this) );
	}
	catch(RException e)
	{
		//�V�K�ɂ�������Ȃ����.
		if (Disp != disp )
		{
			if (disp)	disp->Dec();
		}
		//�V�����t�@�C���ꗗ�̏���
		if (files)	files->Dec();

		//���̃p�X�͉����ł��܂���B
		printf("error : %s\n" , e.getMessage() );
		return false;
	}
	//�q�X�g���[�ɒǉ�.
	if (!isNOTRecodeHistory) History.Append(inPath);

	return true;
}

//�v���r���[
bool Window::Preview(string inPath,bool isNOTRecodeHistory)
{
	Display* disp = Disp;
	Rfiles* files = NULL;
	try
	{
		//�p�X�������ł���I�u�W�F�N�g�̍쐬.
		SolvableFileManager* spm = SolvableFileManager::getInstance();
		SolvableFile* tws = spm->DoSolvable(inPath);
		if (tws == NULL) 
			throw RNotSupportException("�p�X %s �͉����ł��܂���ł���" ,inPath.c_str() );
		//���ݕ\�����Ă���p�^�[���ƈႤ�ꍇ�͂قڃf�B�X�v���C�̍�蒼�����K�v�ł��B
		if ( Disp->getNowNavigateType() != tws->getType())
		{
			if ( !tws->IsDisplaySupported( Disp->getType() ))
			{//�T�|�[�g����Ă��Ȃ��̂ō�蒼���ł��B
				//�f�B�X�v���C�쐬.
				disp = DisplayFactory::New( tws->WhatIsDisplayeRecommendedMost() , this , tws->getType() );
				//�J�����̓o�^.
				disp->setFixedCloumn( tws->LetKnowColumnesInDisplayRecommendedMost() );
			}
		}
		//���ۂɃt�@�C���V�X�e���𑀍삷�� Rfiles ���쐬.
		files = tws->RfilesFactory( disp->getType() );
		try
		{
			//�\�[�g�@�\�̍쐬.
			files->MakeTable(__FILECOLUMN_TYPE_EXT , true );
		}
		catch(RNotSupportException e)
		{	//���s������A���O�Ń\�[�g.
			files->MakeTable(__FILECOLUMN_TYPE_NAME , true );
		}

		//�^�X�N�}�l�[�W���[�ɓo�^.
		RTaskManager * rtm = RTaskManager::getInstance();
		//	void ls(Rfiles* inFiles ,Display* inDisp,  Window* inCallback);
		rtm->putTask( new RTaskLS(files ,Mask , disp , this) );


	}
	catch(RException e)
	{
		//�V�K�ɂ�������Ȃ����.
		if (Disp != disp ) 	
		{
			if (disp) disp->Dec();
		}
		//�V�����t�@�C���ꗗ�̏���
		if (files) files->Dec();

		//���̃p�X�͉����ł��܂���B
		printf("error : %s\n" , e.getMessage() );
		return false;
	}
	//�q�X�g���[�ɒǉ�.
	if (!isNOTRecodeHistory) History.Append(inPath);
	return true;
}


//���݂̃f�B�X�v���C��ł̃p�X���ύX�ɂȂ�܂����B
//���Ƃ��΃u���E�U�Ń����N���N���b�N���ꂽ�Ƃ��A
//���݂̃f�B�X�v���C���g���ăp�X�ƃq�X�g���[�����L�^���܂��B
void Window::OnQuickChange(string inPath,bool isNOTRecodeHistory )
{
	ASSERT(this->Files);

	this->Files->setPath(inPath);

	//�e�� OnQuickChange �����������Ƃ�ʒm���A
	//�A�h���X�o�[���X�V����.
	ParentFSub->OnQuickChange();

	//�q�X�g���[�ɒǉ�.
	if (!isNOTRecodeHistory) History.Append(inPath);
}


//�^�C�g���̎擾.
string Window::getTitle() const
{
	string title;
	if (Disp)	title = Disp->getTitle();
	
	//�擾�ł��Ȃ��΂����́A�Ō�̃f�B���N�g���ɂ���.
	if (title.empty() )
	{
		title = getNowPath();
		int pos = MultiString::getLastDirectoryPos( title.c_str() );

		if (title[pos] == '\\') pos ++;
		title = title.substr( pos , title.size() - pos);
		if ( !MultiString::IsPathToComplement(title.c_str() ) )
		{	//�Ō�� \\ ���폜���܂��B
			if ( title.size() <= 1) return "";	// \\\\ �ƘA���������΂��̂ŉ��.
			return title.substr(0 , title.size() - 1);
		}
	}
	return title;
}

//�^�C�g�����ύX���ꂽ�ꍇ
void Window::OnTitleChange()
{
	//�e�� OnTitleChange �����������Ƃ�ʒm����
	ParentFSub->OnTitleChange();
}


//���������L���Ă���^�X�N�����ׂĂƂ߂܂��B
void Window::StopTasks() const
{
	//�^�X�N�}�l�[�W���[�ɓo�^.
	RTaskManager * rtm = RTaskManager::getInstance();
	rtm->stopTasks(this);
}

