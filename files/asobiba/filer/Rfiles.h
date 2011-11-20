// RFiles.h: RFiles �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_)
#define AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Rfiles;

#include "comm.h"


#include "RTable.h"
#include "ReferenceCount.h"
#include "Rfile.h"
#include "RNotSupportException.h"


//���z�f�B���N�g���̍ő�[�x.
const int MAX_DIMLIMITERES	=	128;


//RfilesWithCache �Ŏg�p����t�@���N�^ �̎�������
class CacheFileFnctional
{
public:
	//Rfile �̍쐬.
	virtual Rfiles* New(__RFILETYPE inRfileType,const string inSrcPath) const throw() = 0;

	//�\�[�X�p�X�̎擾
	virtual string getSrcPath(const string inPath) const = 0;
	//���΃p�X�̎擾
	virtual string getRelativePath(const string inPath) const = 0;

	//���� inTargetFile ��ǉ����āE�́E�j��!!�ł���?
	virtual bool IsAppend(const string inSrcPath, const Rfile* inTargetFile) const = 0;

	//RTable �̍쐬.
	virtual RTable* ProxyCreateTable( const Rfiles* inSrcFiles,__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() = 0;
};

class Rfiles  : public ReferenceCount
{
private:
	//RTable �t�@�C�����i�[����ꏊ.
	RTable*			Table;

public:
	Rfiles()
	{
		this->Table = NULL;
	}
protected:
	virtual ~Rfiles()
	{
		delete this->Table ;
	}
public:
	//�t�@�C�������g���q�𕪗�
	static	string Filename2Ext(const string inFilename)
	{
		const char* str = inFilename.c_str();
		for(int i = inFilename.length() - 1; str[i] != '.' && i >= 0 ; i--)
			;
		if (i < 0 )	return "";

		//�g���q��啶����
		string ext = (char*)(str + i + 1);
		for(register int l = 0 ; ext[l] != '\0' ;l++) ext[l] = toupper(ext[l]);

		return ext;	// +1 �� '.' �̕��ł��B
	}

	//�ꗗ�擾
	virtual void ls() = 0;
	//�ꗗ�擾
	virtual void ls(string inFilter) = 0;
	//�폜
	virtual void rm(string inFilename) = 0;
	//���l�[��
	virtual void rename(string inSrcFilename,string inDestFilename) = 0;
	//��ނ�Ԃ�.
	virtual __RFILETYPE		getRfileType()	const = 0;
	//RTable���쐬���܂��B
	//ls ���g���O�ɌĂ�ł��������B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw() = 0;
	//�p�X�̎擾
	virtual string getPath() const = 0;
	//�p�X�̐ݒ�
	virtual void setPath(const string inPath) = 0;

	//�������ݕs�\�� RTable�̎擾.
	const RTable* getTable() const
	{
		ASSERT(this->Table != NULL);
		return this->Table;
	}

protected:
	//�������݉\�� RTable���擾
	RTable* getWritebleTable() const
	{
		ASSERT(this->Table != NULL);
		return this->Table;
	}
	//RTable ��u��������.
	void setTable(RTable* inRt)
	{
		ASSERT(inRt != NULL);

		delete this->Table;
		this->Table = inRt;
	}
};


#endif // !defined(AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_)
