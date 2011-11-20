// RfileArcBigIcon.h: RfileArcBigIcon �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_)
#define AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileArcBigIcon;

#include "comm.h"


#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"
#include "RfileArc.h"

//���k�t�@�C���ő傫���A�C�R��.
class RfileArcBigIcon : public RfileArc  
{
public:
	RfileArcBigIcon()
	{
		this->FileModify = NULL;
	}
	RfileArcBigIcon(const RfileArcBigIcon& inSrc)
	{
		Path = inSrc.Path;
		Name = inSrc.Name;
		Size = inSrc.Size;
		Date = inSrc.Date;
		Ratio = inSrc.Ratio;
		Mode = inSrc.Mode;

		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		FileModify = fmc->Copy( inSrc.FileModify );
	}
	virtual ~RfileArcBigIcon()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//�o�^���܂��B
	virtual void Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute);

	//�l�̎擾.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const;
	//�C���[�W�̎擾.
	virtual const int getImageIndex() const;

	//�\�[�g�L�[��o�^���܂��B Attch�̌�ɌĂ�ł�������.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey)
	{
		switch(inKey)
		{
		case __FILECOLUMN_TYPE_NAME:			//���O
			SortKey = (const void*) &this->Name;
			break;
		case __FILECOLUMN_TYPE_EXT:				//�^�C�v
			SortKey = (const void*) this->FileModify->getFileTypePointer();
			break;
		case __FILECOLUMN_TYPE_SIZE:			//�T�C�Y
			SortKey = (const void*) &this->Size;
			break;
		case __FILECOLUMN_TYPE_DATE:			//�X�V���t
			SortKey = (const void*) &this->Date;
			break;
		case __FILECOLUMN_TYPE_RATIO:			//���k���[�g(���k��)
			SortKey = (const void*) &this->Ratio;
			break;
		case __FILECOLUMN_TYPE_ARCMODE:			//���k���[�h
			SortKey = (const void*) &this->Mode;
			break;
		default:
			//�T�|�[�g�ΏۊO.
			ASSERT(0);
			abort();
		}
	}

	//��ނ�Ԃ�.
	virtual __RFILETYPE	getType() const	
	{
		return __RFILETYPE_ARC_BIGICON;
	}
	//�f�B���N�g���ł���?
	virtual bool IsDirectory() const 
	{
		//�f�B���N�g���Ƃ����T�O���ˁ[
		return false;
	}

	//����ȃt�@�C���C���q�̎擾.
	static FileModifyData* NormalLoadModifty(string inExt , string inFullFilename );
	//�f�B���N�g���̃t�@�C���C���q�̎擾.
	static FileModifyData* DirectoryLoadModifty();

private:
	string		Path;	//�A�[�J�C�o���̃p�X
	string		Name;
	DWORD		Size;
	DWORD		Date;
	WORD		Ratio;
	string		Mode;

	//�t�@�C���C���q(�A�C�R���A�t�@�C���^�C�v)
	//���L�|�C���^�ŃL���b�V�����Ă܂��B
	//�f�l�ɂ͂����߂ł��Ȃ��댯�ȃ��[�`���ł�(w
	FileModifyData*	FileModify;


};

#endif // !defined(AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_)
