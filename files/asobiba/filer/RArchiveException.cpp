// RArchiveException.cpp: RArchiveException �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveException.h"

/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_LZH		0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048

#define ERROR_END	ERROR_BROKEN_DATA

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
RArchiveException::RArchiveException(unsigned long inErrorCode,char* Format,...)
{
	setErrorCode(inErrorCode);
	setError(Format,(char*)(&Format+1));

	string adderror;
//UnLha32�� API.txt ���R�s�y(w
//���쌠�Ƃ����v���� �޸�޸�޸���������.
	switch(inErrorCode)
	{
//���x��    �F�ʏ�C�Y���t�@�C���ɂ��Ă̏������X�L�b�v���邾���ŁC��{�I��
//		    ���s�𒆎~���邱�Ƃ͂���܂���B "-jse" �X�C�b�`�ŃG���[�Ƃ���
//			���Ƃ��ł��܂��B
	case ERROR_DISK_SPACE:
		adderror = "�f�B�X�N�̋󂫂�����܂���B";
		FatalError = false;
		break;
	case ERROR_READ_ONLY:
		adderror = "�㏑�����悤�Ƃ��Ă���t�@�C�������[�h�I�����[�ł��B";
		FatalError = false;
		break;
	case ERROR_USER_SKIP:
		adderror = "���[�U�[�ɂ���ď������X�L�b�v����܂����B";
		FatalError = false;
		break;
	case ERROR_FILE_CRC:
		adderror = "�i�[�t�@�C���̃`�F�b�N�T���������Ă��܂���B";
		FatalError = false;
		break;
	case ERROR_FILE_OPEN:
		adderror = "�t�@�C�����J���܂���ł����B";
		FatalError = false;
		break;
	case ERROR_MORE_FRESH:
		adderror = "���V�����t�@�C�����W�J��ɑ��݂��Ă��܂��B";
		FatalError = false;
		break;
	case ERROR_NOT_EXIST:
		adderror = "�t�@�C���͓W�J��ɑ��݂��Ă��܂���B";
		FatalError = false;
		break;
	case ERROR_ALREADY_EXIST:
		adderror = "�t�@�C���͊��ɑ��݂��Ă��܂��B";
		FatalError = false;
		break;
	//���G���[  �F�v���I�ȃG���[�ő����͂��̎��_�Ŏ��s�𒆎~���܂��B
	case ERROR_MAKEDIRECTORY:
		adderror = "�f�B���N�g�����쐬�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_CANNOT_WRITE:
		adderror = "�������݃G���[�������܂����B";
		FatalError = true;
		break;
	case ERROR_HUFFMAN_CODE:
		adderror = "�i�[�t�@�C���̃n�t�}���R�[�h�����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_HEADER_CRC:
		adderror = "���ɂ̃w�b�_�̃`�F�b�N�T���������Ă��܂���B";
		FatalError = true;
		break;
	case ERROR_HEADER_BROKEN:
		adderror = "���ɂ̃w�b�_�����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_ARC_FILE_OPEN:
		adderror = "���ɂ��J�������o���܂���B";
		FatalError = true;
		break;
	case ERROR_NOT_ARC_FILE:
		adderror = "���ɂ̃t�@�C�������w�肳��Ă��܂���B";
		FatalError = true;
		break;
	case ERROR_CANNOT_READ:
		adderror = "�t�@�C���̓ǂݍ��ݎ��ɃG���[�������܂����B";
		FatalError = true;
		break;
	case ERROR_FILE_STYLE:
		adderror = "�w�肳�ꂽ�t�@�C���͗L���� LZH ���ɂł͂���܂���B";
		FatalError = true;
		break;
	case ERROR_COMMAND_NAME:
		adderror = "�R�}���h�w�肪�Ԉ���Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_MORE_HEAP_MEMORY:
		adderror = "��Ɨp�̂��߂̃q�[�v���������s�����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_ENOUGH_MEMORY:
		adderror = "�O���[�o�����������s�����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_ALREADY_RUNNING:
		adderror = "���� UNLHA32.DLL �����쒆�ł��B";
		FatalError = true;
		break;
	case ERROR_USER_CANCEL:
		adderror = "���[�U�[�ɂ���ď��������f����܂����B";
		FatalError = true;
		break;
	case ERROR_HARC_ISNOT_OPENED:
		adderror = "���ɂƃn���h�����֘A�t����O�� FindFirst �����Ă΂�܂����B";
		FatalError = true;
		break;
	case ERROR_NOT_SEARCH_MODE:
		adderror = "FindFirst���Ă΂��O�� FindNext ���Ă΂�܂����B";
		FatalError = true;
		break;
	case ERROR_NOT_SUPPORT:
		adderror = "�T�|�[�g����Ă��Ȃ� API ���g�p����܂����B";
		FatalError = true;
		break;
	case ERROR_TIME_STAMP:
		adderror = "���t�y�ю��Ԃ̎w��`�����Ԉ���Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_TMP_OPEN:
		adderror = "��ƃt�@�C�����쐬�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_LONG_FILE_NAME:
		adderror = "�f�B���N�g���̃p�X���������܂��B";
		FatalError = true;
		break;
	case ERROR_ARC_READ_ONLY:
		adderror = "�ǂݍ��ݐ�p�����̏��ɂɑ΂��鑀��͂ł��܂���B";
		FatalError = true;
		break;
	case ERROR_SAME_NAME_FILE:
		adderror = "���łɓ������O�̃t�@�C�������ɂɊi�[����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_NOT_FIND_ARC_FILE:
		adderror = "�w�肳�ꂽ�f�B���N�g���ɂ͏��ɂ�����܂���ł����B";
		FatalError = true;
		break;
	case ERROR_RESPONSE_READ:
		adderror = "���X�|���X�t�@�C���̓ǂݍ��ݎ��ɃG���[�������܂����B";
		FatalError = true;
		break;
	case ERROR_NOT_FILENAME:
		adderror = "�t�@�C�����̎w�肪�K�v�ł��B";
		FatalError = true;
		break;
	case ERROR_TMP_COPY:
		adderror = "��ƃt�@�C���̏��ɂւ̏����߂����ł��܂���ł����B";
		FatalError = true;
		break;
	case ERROR_EOF:
		adderror = "���ɂ̃f�[�^���������Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_ADD_TO_LARC:
		adderror = "���� SFX �ւ̏��ɑ���͍s���܂���B";
		FatalError = true;
		break;
	case ERROR_TMP_BACK_SPACE:
		adderror = "��ƃt�@�C���̏����߂��ɕK�v�ȃf�B�X�N�̋󂫂�����܂���B";
		FatalError = true;
		break;
	case ERROR_SHARING:
		adderror = "�t�@�C���ւ̃A�N�Z�X��������Ă��܂���B";
		FatalError = true;
		break;
	case ERROR_NOT_FIND_FILE:
		adderror = "�t�@�C����������܂���B";
		FatalError = true;
		break;
	case ERROR_LOG_FILE:
		adderror = "���O�t�@�C���̃G���[�ł��B";
		FatalError = true;
		break;
	case ERROR_NO_DEVICE:
		adderror = "�f�o�C�X��������܂���B";
		FatalError = true;
		break;
	case ERROR_GET_ATTRIBUTES:
		adderror = "�t�@�C���������擾�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_SET_ATTRIBUTES:
		adderror = "�t�@�C�������̐ݒ肪�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_GET_INFORMATION:
		adderror = "�t�@�C����񂪎擾�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_GET_POINT:
		adderror = "�t�@�C���|�C���^���擾�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_SET_POINT:
		adderror = "�t�@�C���|�C���^�̐ݒ肪�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_CONVERT_TIME:
		adderror = "�^�C���X�^���v�̕ϊ����ł��܂���B";
		FatalError = true;
		break;
	case ERROR_GET_TIME:
		adderror = "�^�C���X�^���v�̎擾���ł��܂���B";
		FatalError = true;
		break;
	case ERROR_SET_TIME:
		adderror = "�^�C���X�^���v�̐ݒ肪�ł��܂���B";
		FatalError = true;
		break;
	case ERROR_CLOSE_FILE:
		adderror = "�t�@�C������邱�Ƃ��ł��܂���ł����B";
		FatalError = true;
		break;
	case ERROR_HEAP_MEMORY:
		adderror = "�q�[�v�����������Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_HANDLE:
		adderror = "�����ȃn���h���ł��B";
		FatalError = true;
		break;
	case ERROR_TIME_STAMP_RANGE:
		adderror = "�����ȃ^�C���X�^���v�ł��B";
		FatalError = true;
		break;
	case ERROR_MAKE_ARCHIVE:
		adderror = "���ɂ̍쐬�Ɏ��s���܂����B";
		FatalError = true;
		break;
	case ERROR_NOT_CONFIRM_NAME:
		adderror = "�s���Ȗ��O�����ɂɋL�^����Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_UNEXPECTED_EOF:
		adderror = "�w�b�_�������I�Ɍ������Ă��܂��B";
		FatalError = true;
		break;
	case ERROR_INVALID_END_MARK:
		adderror = "�s���ȃG���h�}�[�N�ł��B";
		FatalError = true;
		break;
	case ERROR_NO_END_MARK:
		adderror = "�K�v�ȃG���h�}�[�N���t������Ă��܂���B";
		FatalError = true;
		break;
	case ERROR_HDR_INVALID_SIZE:
		adderror = "�s���ȃw�b�_�T�C�Y�ł��B";
		FatalError = true;
		break;
	case ERROR_UNKNOWN_LEVEL:
		adderror = "�����Ȃ��w�b�_�`���ł��B";
		FatalError = true;
		break;
	case ERROR_BROKEN_DATA:
		adderror = "�i�[�t�@�C�������Ă��܂��B";
		FatalError = true;
		break;
	default:
		adderror = "���m�̃G���[���������܂����B�т�����ł��B";
		FatalError = true;
		break;
	}

	setError( adderror + "\n" + getMessage() );
}

RArchiveException::~RArchiveException()
{
}

