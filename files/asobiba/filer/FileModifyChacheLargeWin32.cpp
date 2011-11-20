// FileModifyChacheLargeWin32.cpp: FileModifyChacheLargeWin32 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FileModifyChacheLargeWin32.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

const char*	const FileModifyChacheLargeWin32::FOLDER_STRING = "�t�H���_";
const char*	const FileModifyChacheLargeWin32::EXEFILE_STRING = "���s�t�@�C��";
const char*	const FileModifyChacheLargeWin32::FILE_STRING = "�t�@�C��";

FileModifyChacheLargeWin32::FileModifyChacheLargeWin32()
{

	this->ChacheSystem = new FileModifyChacheWin32(32,32,__SYSTEMICON_MAX);

	//�V�X�e���̃C���[�W���X�g�̎擾
	SHFILEINFO  shfi;
	HIMAGELIST systemImageList = (HIMAGELIST)SHGetFileInfo( "" , 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON );

	this->ChacheSystem->TearchSystem( __SYSTEMICON_EXEFILE ,ImageList_GetIcon(systemImageList , 1 , ILD_TRANSPARENT) , EXEFILE_STRING);
	this->ChacheSystem->TearchSystem( __SYSTEMICON_NORMALFILE,ImageList_GetIcon(systemImageList , 2 , ILD_TRANSPARENT) , FILE_STRING);
	this->ChacheSystem->TearchSystem( __SYSTEMICON_NORMALFOLDER,ImageList_GetIcon(systemImageList , 3 , ILD_TRANSPARENT) , FOLDER_STRING);
	this->ChacheSystem->TearchSystem( __SYSTEMICON_SHAREMARK,ImageList_GetIcon(systemImageList , 4 , ILD_TRANSPARENT) , "");
	this->ChacheSystem->TearchSystem( __SYSTEMICON_SHORTCUTMARK,ImageList_GetIcon(systemImageList , 4 , ILD_TRANSPARENT) , "");

	this->ChacheSystem->TearchSystem( __SYSTEMICON_NETRESOURCE_DOMAIN,ImageList_GetIcon(systemImageList , 7 , ILD_TRANSPARENT) , "�h���C��");
	this->ChacheSystem->TearchSystem( __SYSTEMICON_NETRESOURCE_WORKGROUP,ImageList_GetIcon(systemImageList , 18 , ILD_TRANSPARENT) , "���[�N�O���[�v");
	this->ChacheSystem->TearchSystem( __SYSTEMICON_NETRESOURCE_SERVER,ImageList_GetIcon(systemImageList , 15 , ILD_TRANSPARENT) , "�}�V����");
	this->ChacheSystem->TearchSystem( __SYSTEMICON_NETRESOURCE_SHAREPOINT_DISK,ImageList_GetIcon(systemImageList , 85 , ILD_TRANSPARENT) , "���L�f�B�X�N");
}

FileModifyChacheLargeWin32::~FileModifyChacheLargeWin32()
{
	delete this->ChacheSystem;
}

//singleton
FileModifyChacheLargeWin32* FileModifyChacheLargeWin32::getInstance()
{
	static FileModifyChacheLargeWin32 fmclw32;
	return &fmclw32;
}
