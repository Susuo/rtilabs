// RGISJoin.h: RGISJoin �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_)
#define AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStdioFile.h"
#include "RZlib.h"

const int SEGMENT_MAX	=	26;


enum ENUMCVType
{
	CVTypeNone	= 0,
	CVTypeOnePoint	= 1,
	CVTypeMultiPoint	= 2,
	CVTypeString	= 3,
	CVTypeByteData	= 4,
};

struct JoinTempFormatScript
{
	string TempFileDirectoryName;		//��������t�@�C���������Ă���f�B���N�g���� DK �Ƃ�
	unsigned char BlockStartSimbol;		//�u���b�N���X�^�[�g����Ƃ��̃V���{��	���H�������� D �Ƃ�
	int	DataSize;						//CV�f�[�^�������Œ蒷�f�[�^�T�C�Y	�s�σf�[�^
	int	DataSizeSub;					//CV�f�[�^�������Œ蒷�f�[�^�T�C�Y	�ϓ��f�[�^
	bool WithNode;						//CV�̑O�Ƀm�[�h�����L�ڂ���.
	ENUMCVType CVType;					//0 -> CV�Ȃ�  1-> 1Point 2-> ����Point 3->������
};




enum ENUMHeadType
{
	HeadTypeFixed = 1,					//�Œ蒷 DataSize �̃f�[�^�̌J��Ԃ�
	HeadTypeFixedAndString = 2,			//�Œ蒷 DataSize + ������\0 �̌J��Ԃ�
};

struct JoinHeadFormatScript
{
	string TempFilename;				//��������t�@�C���������Ă���t�@�C���� DK �Ƃ�
	unsigned char BlockStartSimbol;		//�u���b�N���X�^�[�g����Ƃ��̃V���{��	�ڎ��������� I �Ƃ�
	int DataSize;						//�Œ�f�[�^�T�C�Y 
	ENUMHeadType	HeadType;
};

struct OptimizeTemp
{
	char Share[16];
	vector<DWORD>	Pos;
};


class RGISJoin  
{
public:
	RGISJoin();
	virtual ~RGISJoin();

	//��������
	void Join(const string & inDir , const string &inMapArcFilename ,
		const JoinTempFormatScript* inScript, int inSize,
		const JoinHeadFormatScript* inHead , int inHeadSize);
	//�������̃`�F�b�N
	void Check(const string & inMasterFilename) throw (RException);
	//�n�}�̏ڂ������𓾂�.
	void Info(const string & inMasterFilename) throw (RException);

private:
	void GetAllFilenameList(RStringVector * inList ,const string & inDir ,const JoinTempFormatScript* inScript, int inSize);
	//master�Ɏ��g���ۂ��ƃR�s�[
	void FileAppend( RStdioFile * ioMaster , RStdioFile * ioFile , unsigned long inSize);
	//�����𐔂��܂�
	unsigned long GetDataCount(RStdioFile * ioFile , const JoinTempFormatScript* inScript , unsigned long inSize) ;
	//�����𐔂��܂�
	unsigned long RGISJoin::GetHeadDataCount(RStdioFile * ioFile , const JoinHeadFormatScript* inScript , unsigned long inSize) ;

	//�w�b�_�[���������݂܂��B
	void RGISJoin::WriteHeader(RStdioFile * ioMaster , const string & inDir ,
						const JoinHeadFormatScript* inHead , int inHeadSize);
	//master�Ɉ��k�����������g���R�s�[
	void RGISJoin::ZFileAppend( RStdioFile * ioMaster , RStdioFile * ioZFile);
	//�œK��
	DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) ;
	DWORD RGISJoin::DataSkip(RStdioFile * ioFile , const JoinTempFormatScript* inScript , char * outMainData = NULL);
	//������t�H�[�}z�b�g�ŋL�^����Ă���t�@�C����
	//�����𐔂��܂�
	unsigned long RGISJoin::StringCount(RStdioFile * ioFile, unsigned long inSize);
	//������t�H�[�}z�b�g�ŋL�^����Ă���t�@�C����
	//�����𐔂��܂�
	unsigned long RGISJoin::FixedAndStringCount(RStdioFile * ioFile , unsigned long inSize , int inFixedSize);

private:
	RZlib	Zlib;
};

#endif // !defined(AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_)
