// RfileLocalBigIcon.cpp: RfileLocalBigIcon �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileLocalBigIcon.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�o�^���܂��B
void RfileLocalBigIcon::Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib)
{
	this->Name = inName;
	this->Size = inSize;
	this->Date = inDate;
	this->Attrib = inAttrib;

	//�t�@�C���C���q�̎擾
	if ( IsDirectory() ) 
	{
		this->FileModify = this->DirectoryLoadModifty( inRfiles->getPath() + inName);
	}
	else
	{
		string ext = Rfiles::Filename2Ext(inName) ;

		if (ext == "EXE")
		{//����t�@�C�� �L���b�V�����܂���.
			this->FileModify = this->SpecialLoadModifty(ext ,inRfiles->getPath() + inName,FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_EXEFILE);
		}
		else if (ext == "LNK" || ext == "ICO")
		{//����t�@�C�� �L���b�V�����܂���.
			this->FileModify = this->SpecialLoadModifty(ext ,inRfiles->getPath() + inName,FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE);
		}
		else
		{//��ʃt�@�C�� �L���b�V�����܂��B
			this->FileModify = this->NormalLoadModifty(ext ,inRfiles->getPath() + inName);
		}
	}
	ASSERT(this->FileModify);
}


//�l�̎擾.
const string RfileLocalBigIcon::getVars(__FILECOLUMN_TYPE inType) const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//���O
		return Name;
	case __FILECOLUMN_TYPE_EXT:				//�^�C�v(�g���q�Ɋ֘A�t�����Ă��閼�O)
		return FileModify->getFileType();
	case __FILECOLUMN_TYPE_SIZE:			//�T�C�Y
		return "";
	case __FILECOLUMN_TYPE_DATE:			//�X�V���t
		return "";
	case __FILECOLUMN_TYPE_ATTRIB:			//����
		return "";
	}
	return "";
}

//�C���[�W�̎擾.
const int RfileLocalBigIcon::getImageIndex() const
{
	return FileModify->getIndex();
}

//���ʂȃt�@�C���C���q�̎擾.
FileModifyData* RfileLocalBigIcon::NormalLoadModifty(string inExt , string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	FileModifyData* p = fmc->Search( inExt );
	if (!p)
	{
		//�Ȃ��̂Ŏ擾.
		SHFILEINFO  shfi = {0};
		SHGetFileInfo( inFullFilename.c_str() , FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);

		//�擾���s�A�V�X�e���̕��ʂ̃t�@�C���̃A�C�R����Ԃ��܂��B
		if (shfi.hIcon == NULL) 
		{
			p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE);
		}
		else
		{//�A�C�R���擾����
			//�t�@�C���^�C�v�擾�ł���񂵂�?
			if (shfi.szTypeName[0] != '\0')
				p = fmc->Tearch(inExt , shfi.hIcon, shfi.szTypeName );
			else
				p = fmc->Tearch(inExt , shfi.hIcon,  inExt + "�t�@�C��");
			DestroyIcon(shfi.hIcon);
		}
	}
	return p;
}

//����ȃt�@�C���C���q�̎擾.
FileModifyData* RfileLocalBigIcon::SpecialLoadModifty(string inExt , string inFullFilename ,
								FileModifyChacheLargeWin32::Modifty inDefaultIconIndex )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	//����t�@�C�� �L���b�V�����܂���.
	SHFILEINFO  shfi = {0};
	SHGetFileInfo( inFullFilename.c_str() , FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);
	//�擾���s�A�V�X�e���̕��ʂ̃t�@�C���̃A�C�R����Ԃ��܂��B
	if (shfi.hIcon == NULL) 
	{
		return fmc->SearchSystem(inDefaultIconIndex);
	}
	else
	{//�A�C�R���擾����
		FileModifyData* p ;
		//����ȃA�C�R���Ȃ̂Ŕ�L���b�V���Ƃ��ēo�^���܂��B
		//�t�@�C���^�C�v�擾�ł���񂵂�?
		if (shfi.szTypeName[0] != '\0')
			p = fmc->Tearch( shfi.hIcon, shfi.szTypeName );
		else
			p = fmc->Tearch( shfi.hIcon,  inExt + "�t�@�C��");
		DestroyIcon(shfi.hIcon);
		return p;
	}
}

//�f�B���N�g���̃t�@�C���C���q�̎擾.
FileModifyData* RfileLocalBigIcon::DirectoryLoadModifty(string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();

	SHFILEINFO  shfi = {0};
	SHGetFileInfo( inFullFilename.c_str(), FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME );

	//�擾���s�A�V�X�e���̕��ʂ̃f�B���N�g���̃A�C�R����Ԃ��܂��B
	if (shfi.hIcon == NULL) 
	{
		return fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
	}
	else
	{//�A�C�R���擾����
		FileModifyData* p ;
		if (shfi.dwAttributes & SFGAO_SHARE)
		{//���L�f�B���N�g��
			if ( FILE_EXIST((inFullFilename + "\\desktop.ini").c_str()) )
			{
				p = fmc->Tearch(shfi.hIcon, FileModifyChacheLargeWin32::FOLDER_STRING );
			}
			else
			{
				p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
			}
			//�{����mix �Ƃ����ɂ傲�ɂ傷��.
		}
		else
		{
			if ( FILE_EXIST((inFullFilename + "\\desktop.ini").c_str()) )
			{
				p = fmc->Tearch(shfi.hIcon, FileModifyChacheLargeWin32::FOLDER_STRING );
			}
			else
			{
				p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
			}
		}
		DestroyIcon(shfi.hIcon);
		return p;
	}
}

