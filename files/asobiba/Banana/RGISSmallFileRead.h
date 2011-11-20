// RGISSmallFileRead.h: RGISSmallFileRead �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_)
#define AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RStdioFile.h"
#include "RGISSmallParentImp.h"
#include "RGISSmallHeaderImp.h"
#include "RGISSmallUserDataFile.h"
#include "RZlib.h"

class RGISSmallFileRead  
{
public:
	RGISSmallFileRead();
	virtual ~RGISSmallFileRead();

	void Create(const string & inFilename, const string & inUserDataPath) throw(RException);
	void Delete();
	bool Load(unsigned long inBlock, RGISSmallParentImp * ioParent);
	bool LoadHeader(RGISSmallHeaderImp * ioHedaer);

	//���[�U�[�f�[�^�̕ύX.
	bool UserDataModify(unsigned long inBlock, RGISSmallParentImp * ioParent,
									 const int * inDeleteNumber , int inDeleteCount ,
									 const char * inData , int inSize,int inCount) throw(RException);


	//WinCE �ȂǂŁA�T�X�y���h���ĕ��A����ƁA
	//�t�@�C���n���h���������ɂȂ錻�ۂ�����B
	//�����������邽�߂ɁA�����n���h���������ɂȂ��Ă�����A
	//�w�肵���t�@�C�����J���Ȃ������ƂőΏ�����.
	void KeepHanlde() throw(RException);

private:
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
private:
	//�t�@�C����
	string		Filename;
	//�}�b�v�t�@�C��.
	RStdioFile	File;
	//�}�b�v�t�@�C���T�C�Y.
	unsigned long FileSize;
	//�ڎ�.
	IndexArea*  Index;
	//�ڎ��̒���
	unsigned long 	IndexCount;
	//�w�b�_�[�̈ʒu
	unsigned long	HeaderPos;
	//�w�b�_�[�̒���
	unsigned long	HeaderSize;

	//���[�U��`�f�[�^���Ǘ�
	RGISSmallUserDataFile UserDataFile;

	RZlib	Zlib;
};

#endif // !defined(AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_)
