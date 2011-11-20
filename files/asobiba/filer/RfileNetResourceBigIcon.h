// RfileNetResourceBigIcon.h: RfileNetResourceBigIcon �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_)
#define AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileNetResourceBigIcon;

#include "comm.h"

#include "RfileNetResource.h"
#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"


class RfileNetResourceBigIcon : public RfileNetResource  
{
public:
	RfileNetResourceBigIcon()
	{
		this->FileModify = NULL;
	}
	virtual ~RfileNetResourceBigIcon()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//�o�^���܂��B
	virtual void Attach(const char* inName,const char* inComment,DWORD inDisplayType );

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
		case __FILECOLUMN_TYPE_COMMENT:			//�R�����g
			SortKey = (const void*) &this->Comment;
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
		return __RFILETYPE_SMBLOGON_BIGICON;
	}
	//�f�B���N�g���ł���?
	virtual bool IsDirectory() const 
	{
		//�f�B���N�g���Ƃ����T�O���ˁ[
		return false;
	}


private:
	string		Name;
	string		Comment;

	//�t�@�C���C���q(�A�C�R���A�t�@�C���^�C�v)
	//���L�|�C���^�ŃL���b�V�����Ă܂��B
	//�f�l�ɂ͂����߂ł��Ȃ��댯�ȃ��[�`���ł�(w
	FileModifyData*	FileModify;


};

#endif // !defined(AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_)
