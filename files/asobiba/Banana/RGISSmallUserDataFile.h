// RGISSmallUserDataFile.h: RGISSmallUserDataFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_)
#define AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RZlib.h"

class RGISSmallUserDataFile  
{
public:
	RGISSmallUserDataFile();
	virtual ~RGISSmallUserDataFile();

	void Create(const string & inPath) ;

	//�w�肳�ꂽ�u���b�N�̃��[�U��`�f�[�^��ǂݍ��݂܂�
	//�߂�l�́A������ new ���ꂽ�������ł��B
	//���̃N���X�͂��̃��������Ǘ����Ȃ��̂ŁA�K�؂ȏꏊ�ŊJ������K�v������܂��B
	//�\��ʂ�ł���΁A RGISSmallParent �� UserData �Ƃ��������o�ŊǗ�����A���̕�炷���͋L���ꂽ�����ɏ��ł��܂�.
	char* Load(unsigned long inBlock ,const RZlib * inZlib) const;

	//�ǉ�.
	void RGISSmallUserDataFile::Append(unsigned long inBlock ,const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException);
	//�폜
	void RGISSmallUserDataFile::Delete(unsigned long inBlock ,const int * inDeleteNumber , int inSize ,const RZlib * inZlib) const throw(RException);

	//�ύX.
	void RGISSmallUserDataFile::Modify(unsigned long inBlock ,const int * inDeleteNumber , int inDeleteCount ,
								   const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException);
private:
	//�w�肳�ꂽ�u���b�N�̃��[�U��`�f�[�^��ǂݍ��݂܂�
	//�߂�l�́A������ new ���ꂽ�������ł��B
	//���̃N���X�͂��̃��������Ǘ����Ȃ��̂ŁA�K�؂ȏꏊ�ŊJ������K�v������܂��B
	char* RGISSmallUserDataFile::FileLoad(unsigned long inBlock ,const RZlib * inZlib,int inOverplusAlooc) const;
	void RGISSmallUserDataFile::FileSave(unsigned long inBlock ,const char * inData , unsigned long inSize,const RZlib * inZlib) const throw(RException);
	//���̃f�[�^�Ɉړ�.
	char* RGISSmallUserDataFile::NextData(const char* inData) const;

	//���ȃ`�F�b�N �w�b�_���Ȃ��f�[�^�̂�.
	bool RGISSmallUserDataFile::MySelfTest(const char* inData , int inSize , int inCount) const;
	//���ȃ`�F�b�N.
	bool RGISSmallUserDataFile::MySelfTest(const char* inData) const;
	//�t�@�C���e�X�g.
	bool RGISSmallUserDataFile::FileTest(unsigned long inBlock ,const RZlib * inZlib) const;


private:

	//���[�U�f�[�^������p�X.
	string Path;
};

#endif // !defined(AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_)
