// ImageListChacheWin32.h: ImageListChacheWin32 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_)
#define AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDoNotInitaliseException.h"
#include "RImageList.h"

//�t�@�C�����C������f�[�^(�A�C�R���A�t�@�C���^�C�v)�̂��̂���.
class FileModifyData
{
private:
	FileModifyData(){}
protected:
	FileModifyData(int inIndex , string inFileType )
	{
		this->Index = inIndex;
		this->FileType = inFileType;
	}
public:
	int getIndex() const
		{		return this->Index;	}
	string getFileType() const
		{		return this->FileType;	}
	const string* getFileTypePointer() const		//�댯����
		{		return &this->FileType;	}	//
	//�f�o�b�O
	void Dump()
	{
		printf("index as %d  " , Index);
		printf("%s  " , FileType.c_str());
		puts("");
	}
private:
	int Index;
	string FileType;
};

//�t�@�C�����C������f�[�^(�A�C�R���A�t�@�C���^�C�v)�̎Q�Ɖ񐔕t����
//�Ǘ�����N���X.
class FileModifyAdministrant : public FileModifyData
{
public:
	FileModifyAdministrant(int inIndex , string inFileType ) 
		: FileModifyData(inIndex,inFileType)
	{
		this->UseCount = 0;
	}
	//����A�g���܂��B
	//�g����������A�g���I������� needless ���ĂԂ悤��
	void Use()
	{
		this->UseCount++;
	}
	//�K�v�Ȃ�
	void Needless()
	{
		this->UseCount--;
	}
	//�����Ă�������?
	bool IsDelete() const
	{
		return this->UseCount == 0;
	}
private:
	int UseCount;
};

//�t�@�C�����C������(�A�C�R���A�t�@�C���^�C�v)���L���b�V�����āA
//�����œ��삳���邷���[���[�`��(w
class FileModifyChacheWin32  
{
public:
	//���s�� RDoNotInitaliseException 
	FileModifyChacheWin32(int inWidth,int inHegith,int inSystemChacheSize );
	virtual ~FileModifyChacheWin32();

	//����.
	//������Ȃ��ꍇ�� NULL.
	FileModifyData* Search(string inExt);
	//�V�X�e�����擾
	FileModifyData* SearchSystem(int inPosition) const;

	//������ �L���b�V�����Ăق����f�[�^
	FileModifyData* Tearch(string inExt,HICON inIcon , string inFileType);
	//������ �L���b�V���ł��Ȃ����j�[�N�ȃf�[�^
	FileModifyData* Tearch(HICON inIcon , string inFileType);

	//������  �L���b�V���ł��Ȃ��V�X�e���n�ȃf�[�^
	FileModifyData* TearchSystem(int inPosition , HICON inIcon , string inFileType);

	//�g�p����
	void Use(FileModifyData* inFileModifyData);

	//�폜����
	void Delete(FileModifyData* inFileModifyData);

	//�C���[�W���X�g�̎擾.
	//����ɂ��̃N���X�O���珑�����݂����ꍇ�͌��ʂ͕ۏ؂��Ȃ��B
	const HIMAGELIST getImageList() const
	{
		return this->ImageList.getImageList();
	}
	//�f�o�b�O
	void Dump();
private:
	//�^�[�Q�b�g���������āA�폜����
	//������Ȃ��ꍇ�� false .... �܂�V�X�e���n�̃L���b�V���ł��B
	bool SearchDelete(FileModifyAdministrant* inTarget);
	//�V�X�e���n�̃L���b�V�����܂߂� ALL ���ځ[��B
	void Clear();

private:
	//�ʏ�̃L���b�V��
	typedef map<string , FileModifyAdministrant*> ICONCHACHE;
	ICONCHACHE			Chache;

	//�L���b�V���ł��Ȃ��f�[�^ ���j�[�N
	typedef deque<FileModifyAdministrant*> ICONONECHACHE;
	ICONONECHACHE		NoneChache;

	//�L���b�V���ł��Ȃ��f�[�^ �V�X�e��
	FileModifyAdministrant**				NoneChacheSystem;
	int										NoneChacheSystemSize;

	RImageList			ImageList;
};

#endif // !defined(AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_)
