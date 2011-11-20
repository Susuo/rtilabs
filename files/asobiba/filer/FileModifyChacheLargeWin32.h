// FileModifyChacheLargeWin32.h: FileModifyChacheLargeWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_)
#define AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModifyChacheWin32.h"
#include "RLock.h"

//�傫�ȃt�@�C���̃L���b�V��.
class FileModifyChacheLargeWin32  
{
public:
	enum Modifty
	{
		__SYSTEMICON_EXEFILE,
		__SYSTEMICON_NORMALFILE,
		__SYSTEMICON_NORMALFOLDER,
		__SYSTEMICON_SHAREMARK,
		__SYSTEMICON_SHORTCUTMARK,
		__SYSTEMICON_NETRESOURCE_DOMAIN,
		__SYSTEMICON_NETRESOURCE_WORKGROUP,
		__SYSTEMICON_NETRESOURCE_SERVER,
		__SYSTEMICON_NETRESOURCE_SHAREPOINT_DISK,
		__SYSTEMICON_MAX,				//�o�^����Ă��鐔.
	};
	static const char*	const FOLDER_STRING; //= "�t�H���_";
	static const char*	const EXEFILE_STRING; //= "���s�t�@�C��";
	static const char*	const FILE_STRING; //= "�t�@�C��";
private:
	FileModifyChacheLargeWin32();
public:
	virtual ~FileModifyChacheLargeWin32();
public:
	//singleton
	static FileModifyChacheLargeWin32* getInstance();

	//����.
	//������Ȃ��ꍇ�� NULL.
	FileModifyData* Search(string inExt)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Search(inExt);	
		if (p)
		{
			this->ChacheSystem->Use(p);
		}
		return p;
	}
	//�V�X�e�����擾
	FileModifyData* SearchSystem(int inPosition)
	{
		RAutoLock al(&Lock);
		FileModifyData* p =  this->ChacheSystem->SearchSystem(inPosition);	

		ASSERT(p);
		this->ChacheSystem->Use(p);

		return p;
	}

	//������ �L���b�V�����Ăق����f�[�^
	FileModifyData* Tearch(string inExt,HICON inIcon , string inFileType)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Tearch(inExt,inIcon,inFileType);
		this->ChacheSystem->Use(p);

		return p;
	}
	//������ �L���b�V���ł��Ȃ����j�[�N�ȃf�[�^
	FileModifyData* Tearch(HICON inIcon , string inFileType)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Tearch(inIcon,inFileType);	
		this->ChacheSystem->Use(p);

		return p;
	}

	//�s�v�ɂȂ����̂ō폜
	void Delete(FileModifyData* inFileModifyData)
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Delete(inFileModifyData);
	}
	//�R�s�[
	FileModifyData* Copy(FileModifyData* inFileModifyData)
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Use(inFileModifyData);
		return inFileModifyData;
	}

	//�C���[�W���X�g�̎擾.
	//����ɂ��̃N���X�O���珑�����݂����ꍇ�͌��ʂ͕ۏ؂��Ȃ��B
	const HIMAGELIST getImageList()
	{
		RAutoLock al(&Lock);
		return this->ChacheSystem->getImageList();
	}


	//�V�X�e���A�C�R���ƃ~�b�N�X���ă��j�[�N�ȃA�C�R���̍쐬.
	//���L�t�H���_�Ƃ��A�V���[�b�g�J�b�g�Ƃ��ɂ��܂����B
/*	�s��.
	FileModifyData* MakeMix(HICON inIcon,int inPosition)
	{
	}
*/

	//�f�o�b�O.
	void Dump()
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Dump();
	}

private:
	FileModifyChacheWin32*	ChacheSystem;
	RLock					Lock;
};

#endif // !defined(AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_)
