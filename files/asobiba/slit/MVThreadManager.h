// MVThreadManager.h: MVThreadManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_)
#define AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "MVReaderThread.h"
#include "MVFileList.h"



class MVThreadManager  
{
	struct MVThreadManagerImagCache
	{
		bool			Run;		//�d�����˗����Ă���?
		MVReaderThread	Thread;		//��ǂ݃X���b�h

		MVPageData*		Image;
		int				Page;		//�S�����Ă���y�[�W
	};

public:
	MVThreadManager();
	virtual ~MVThreadManager();

	/**
	 * Update:				�V�����{���J��
	 *
	 * @param inFilename	�t�@�C����
	 * @return void
	 */
	void Update(const string & inFilename) throw(RException);

	/**
	 * GotoPage:			�w�肳�ꂽ�y�[�W�Ɉړ�
	 *
	 * @param inPage				�y�[�W�ԍ�
	 * @return const MVPageData*	�y�[�W�̃f�[�^.
	 *
	 */
	const MVPageData* GotoPage(int inPage) throw(RException);

	/**
	 * ChangeOption:		�ݒ�̕ύX
	 *
	 * @param inImageConvertParam	�ϊ��Ɏg�p����p�����[�^
	 * @return void 
	 */
	void ChangeOption(const MVImagePreConvert* inImageConvertParam );


	int getPageCount() const
	{
		return FileList.getCount();
	}
private:

	//���ׂẴX���b�h���~������.
	void MVThreadManager::AllThreadDestory();
	//�X���b�h�̊J�n����.
	void MVThreadManager::ThreadRun(int inPage , MVThreadManagerImagCache* ioThread);
	//�f�[�^�̓ǂݍ��݂��I����Ă��Ȃ��ꍇ���f������.
	void MVThreadManager::ThreadAbort(MVThreadManagerImagCache* ioThread);
	//�f�[�^�̓ǂݍ��݂����͏I����Ă��Ȃ��ꍇ�͏I���܂ő҂�.
	void MVThreadManager::ThreadJoin(MVThreadManagerImagCache* ioThread);

private:
	MVThreadManagerImagCache	Before;
	MVThreadManagerImagCache	Now;
	MVThreadManagerImagCache	After;


	MVFileList			FileList;

	MVImagePreConvert	ImageConvertParam;

};

#endif // !defined(AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_)
