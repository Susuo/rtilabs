// XLFileUtil.cpp: XLFileUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLFileUtil.h"
#include "XLStringUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLFileUtil::XLFileUtil()
{

}

XLFileUtil::~XLFileUtil()
{

}


//�t�@�C�������݂��邩?
bool XLFileUtil::Exist(const string & inFileName)
{
	const string newStr = XLStringUtil::Replace(inFileName , "/" , "\\");
	return (0xFFFFFFFF != GetFileAttributes(newStr.c_str() ));
}

//�t�@�C�������ׂ� string �ɓǂݍ���.
string XLFileUtil::FileToString(const string & inFileName)
{

	FILE * fp = fopen(inFileName.c_str() , "rb");
	//���݂��Ȃ��ꍇ�͋�
	if (fp == NULL) return "";

	//�P�c�Ɏ����Ă�����.
	fseek(fp , 0 ,SEEK_END);

	//����ŃT�C�Y���킩��.
	unsigned long size = ftell(fp);

	//�擪�ɖ߂�.
	fseek(fp , 0 ,SEEK_SET);

	//�̈���m�ۂ��ēǂݍ���
	string ret;
	ret.resize(size + 1);
	fread( &ret[0] , 1 , size  , fp);
	ret[size] = '0';	//�I�[.

	fclose(fp);

	return ret;
}

//inStr �� �t�@�C���ɏ�������
void XLFileUtil::StringToFile(const string & inStr , const string & inFileName)
{
	FILE * fp = fopen(inFileName.c_str() , "wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return ;
	}

	fwrite( inStr.c_str() , 1, inStr.size()  , fp);

	fclose(fp);
}

//inBuffer �� �t�@�C���ɏ�������
void XLFileUtil::BufferToFile(const XLBuffer & inBuffer , const string & inFileName)
{
	FILE * fp = fopen(inFileName.c_str() , "wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return ;
	}

	fwrite( &inBuffer[0] , 1, inBuffer.size()  , fp);

	fclose(fp);
}