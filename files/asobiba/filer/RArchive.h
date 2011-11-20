// RArchive.h: RArchive �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_)
#define AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotCloseException.h"
#include "RArchiveException.h"

enum __ARCHIVETYPE
{
	__ARCHIVETYPE_LZH,
};

typedef	HGLOBAL	HARC;

#define FNAME_MAX32		512

typedef struct 
{
	DWORD 			dwOriginalSize;		/* �t�@�C���̃T�C�Y */
 	DWORD 			dwCompressedSize;	/* ���k��̃T�C�Y */
	DWORD			dwCRC;				/* �i�[�t�@�C���̃`�F�b�N�T�� */
	UINT			uFlag;				/* �������� */
										/* Status flag */
	UINT			uOSType;			/* ���ɍ쐬�Ɏg��ꂽ OS */
	WORD			wRatio;				/* ���k�� */
	WORD			wDate;				/* �i�[�t�@�C���̓��t(DOS �`��) */
	WORD 			wTime;				/* �i�[�t�@�C���̎���(�V) */
	char			szFileName[FNAME_MAX32 + 1];	/* ���ɖ� */
	char			dummy1[3];
	char			szAttribute[8];		/* �i�[�t�@�C���̑���(���ɌŗL) */
	char			szMode[8];			/* �i�[�t�@�C���̊i�[���[�h(�V) */
}	INDIVIDUALINFO;


#define M_INIT_FILE_USE			0x00000001L	/* ���W�X�g���̐ݒ���g�p */
#define M_REGARDLESS_INIT_FILE	0x00000002L	/* �V ���g�p���Ȃ� */
#define M_NO_BACKGROUND_MODE	0x00000004L	/* �o�b�N�O���E���h���֎~ */
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	/* ���i�ȃt�@�C�����T�[�` */
#define M_CHECK_FILENAME_ONLY	0x00000200L	/* �V���s��Ȃ� */
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	/* �h���C�u������i�[ */
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	/* �V ���i�[���Ȃ� */
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* �G���[���b�Z�[�W��\�� */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* �V��\�����Ȃ� */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L /* �j���w�b�_�̓ǂݔ�΂� */

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	/* �������ꂽ�t�@�C������ */
#define EXTRACT_NAMED_FILE		0x80000000L	/* �w�肵���t�@�C������ */


class RArchive  
{
protected:
	RArchive()
	{
	}
public:
	virtual ~RArchive()
	{
	}
public:
	//!�������ł��Ȃ������� RDoNotInitaliseException
	//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
	virtual void Create() throw() = 0;

	//���݂̃o�[�W������Ԃ��܂��B
	//@return			���݂̃o�[�W����	  1 -> Version 0.01
	//										100 -> Version 1.00
	virtual WORD getVersion() = 0;

	//���݁A���쒆���ۂ��𓾂܂��B
	virtual bool IsRunning() = 0;

	//�����̎��s
	//@inHWND		�֘A�t����WindowHandle;
	//@inCmdLine	�R�}���h�̕�����B 
	//@outOutput	���ʂ�Ԃ����߂̃o�b�t�@�BLHA.EXE ���R��
	//				�\�[���ɕ\������̂Ɠ��l�̂��̂��C���̃o�b�t�@�Ɋi�[��
	//				��܂��B
	//@inSize		�o�b�t�@�̃T�C�Y�B�Ԃ����ʂ̃T�C�Y���w��
	//				���ꂽ�T�C�Y���������ꍇ�͎w��T�C�Y�܂ŏo�͂����B
	//				���o�[�W�����ł� 64KB �܂ł̐���������B
	//!RArchiveException
	virtual void Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize)  throw() = 0;

	//�t�@�C�����������A�[�J�C���ł��邩�ǂ����`�F�b�N
	//@inFilename	�t�@�C����
	//@inMode		�`�F�b�N���[�h
	//@return		������ true
	virtual bool CheckArchive(const char* inFilename,const int inMode) = 0;

	//�w�肳�ꂽ���Ƀt�@�C���Ɋi�[����Ă���t�@�C�����𓾂܂��B
	//@inFilename	�t�@�C����
	//@return		�t�@�C����
	virtual	int	GetFileCount(const char* inCmdLine) = 0;

	//�A�[�J�C�����J��
	//@inHWND		�֘A�t����Window
	//@inFilename	�Ώۃt�@�C����
	//@inMode		�J���Ƃ��Ɏg���I�v�V����
	//@return		�A�[�J�C���n���h��
	//!RIOCannotOpenException	�J���܂���.
	virtual HARC OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode) = 0;

	//OpenArchive�ŊJ�����A�[�J�C�����N���[�Y����.
	//@inHandle		�A�[�J�C���n���h��
	//!RIOCannotOpenException	�J���܂���.
	virtual void CloseArchive(HARC inHandle) = 0;

	//�t�@�C���������J�n
	//ioHandle		OpenArchive�ŊJ�����n���h��
	//@inWildName	��������t�@�C�����@���C���h�J�[�h���i�E�́E�j���I
	//@outSubInfo	����
	//@return		����			true
	//				�����I��		false
	//���s	RArchiveException
	virtual bool FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo) = 0;

	//�t�@�C������ 2�߈ȍ~.
	//@ioHandle		OpenArchive�ŊJ�����n���h��
	//@outSubInfo	����
	//@return		����			true
	//				�����I��		false
	//���s	RArchiveException
	virtual bool FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo) = 0;
};

#endif // !defined(AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_)
