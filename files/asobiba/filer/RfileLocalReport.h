// RfileLocalReport.h: RfileLocalReport �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILELOCALREPORT_H__9820FAF2_02B0_4485_ABCF_F74642007AC6__INCLUDED_)
#define AFX_RFILELOCALREPORT_H__9820FAF2_02B0_4485_ABCF_F74642007AC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileLocalReport;

#include "comm.h"


#include "RfileLocal.h"
#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"

class RfileLocalReport : public RfileLocal  
{
public:
	RfileLocalReport()
	{
		this->FileModify = NULL;
	}
	virtual ~RfileLocalReport()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//�o�^���܂��B
	virtual void Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib);

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
		case __FILECOLUMN_TYPE_ATTRIB:			//����
			SortKey = (const void*) &this->Attrib;
			break;
		default:
			//�T�|�[�g�ΏۊO.
			ASSERT(0);
			abort();
		}
	}
	//����ȃt�@�C���C���q�̎擾.
	static FileModifyData* NormalLoadModifty(string inExt , string inFullFilename );
	//����ȃt�@�C���C���q�̎擾.
	static FileModifyData* SpecialLoadModifty(string inExt , string inFullFilename ,
									FileModifyChacheLargeWin32::Modifty inDefaultIconIndex );
	//�f�B���N�g���̃t�@�C���C���q�̎擾.
	static FileModifyData* DirectoryLoadModifty(string inFullFilename );

	//��ނ�Ԃ�.
	virtual __RFILETYPE	getType() const	
	{
		return __RFILETYPE_LOCALFILE_BIGICON;
	}
	//�f�B���N�g���ł���?
	virtual bool IsDirectory() const 
	{
		return Btob(this->Attrib & FILE_ATTRIBUTE_DIRECTORY) ;
	}


private:
	string		Name;
	__int64		Size;
	__int64		Date;
	DWORD		Attrib;

	//�t�@�C���C���q(�A�C�R���A�t�@�C���^�C�v)
	//���L�|�C���^�ŃL���b�V�����Ă܂��B
	//�f�l�ɂ͂����߂ł��Ȃ��댯�ȃ��[�`���ł�(w
	FileModifyData*	FileModify;


};

#endif // !defined(AFX_RFILELOCALREPORT_H__9820FAF2_02B0_4485_ABCF_F74642007AC6__INCLUDED_)
