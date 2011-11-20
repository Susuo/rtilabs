// RfileArcBigIcon.cpp: RfileArcBigIcon �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileArcBigIcon.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



//�o�^���܂��B
void RfileArcBigIcon::Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute)
{
	this->Path = inPath;
	this->Name = inName;
	this->Size = inSize;
	this->Date = inDate;
	this->Ratio = inRatio;
	this->Mode = inMode;

	if ( strcmp( inAttribute , "DIR" ) != 0)
	{//�t�@�C����������...
		string ext = Rfiles::Filename2Ext(inName) ;
		this->FileModify = this->NormalLoadModifty(ext ,inName);
	}
	else
	{//�f�B���N�g����������...
		this->FileModify = this->DirectoryLoadModifty();
	}

	ASSERT(this->FileModify);
}


//�l�̎擾.
const string RfileArcBigIcon::getVars(__FILECOLUMN_TYPE inType)  const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//���O
		return Name;
	case __FILECOLUMN_TYPE_VIRTUALPATH:		//���z�p�X(���k�t�@�C���Ȃǂ́@���k�t�@�C���̐擪�� / �Ƃ���p�X)
		return Path;
	case __FILECOLUMN_TYPE_EXT:				//�^�C�v(�g���q�Ɋ֘A�t�����Ă��閼�O)
		return FileModify->getFileType();
	case __FILECOLUMN_TYPE_SIZE:			//�T�C�Y
		return "";
	case __FILECOLUMN_TYPE_DATE:			//�X�V���t
		return "";
	case __FILECOLUMN_TYPE_ARCMODE:			//���k���[�h
		return "";
	case __FILECOLUMN_TYPE_RATIO:			//���k���[�g(���k��)
		return "";
	}
	return "";
}

//�C���[�W�̎擾.
const int RfileArcBigIcon::getImageIndex()  const
{
	return FileModify->getIndex();
}


//���ʂȃt�@�C���C���q�̎擾.
FileModifyData* RfileArcBigIcon::NormalLoadModifty(string inExt , string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	FileModifyData* p = fmc->Search( inExt );
	if (!p)
	{
		//�Ȃ��̂Ŏ擾.
		SHFILEINFO  shfi = {0};
		SHGetFileInfo( ('.' + inExt).c_str() , FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ICON);

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

//�f�B���N�g���̃t�@�C���C���q�̎擾.
FileModifyData* RfileArcBigIcon::DirectoryLoadModifty()
{
	//�Ƃ肠�����A�ӂ[�̃f�B���N�g���A�C�R����Ԃ��܂��B
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	return fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
}
