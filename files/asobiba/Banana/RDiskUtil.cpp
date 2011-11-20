// RDiskUtil.cpp: RDiskUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RDiskUtil.h"
#include "RConv.h"
#include <imagehlp.h>

#pragma comment(lib,"imagehlp.lib")


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RDiskUtil::RDiskUtil()
{

}

RDiskUtil::~RDiskUtil()
{

}

//�R�s�[(�f�B���N�g���̏ꍇ�ȉ��S��)
void RDiskUtil::Copy(const string & inFilename,const string & inTo, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);
	char * to = RDiskUtil::getDoubleNULLFilename(inTo);


	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_COPY;						//����̎��
	op.pFrom = filename;				//�Ώۂ̃t�@�C��
	op.pTo   = to;                        
	op.fFlags= appendFlg;         
	op.fAnyOperationsAborted = true; //�L�����Z���ł��邩�H
	int ret = SHFileOperation(&op);

	delete [] filename;
	delete [] to;

	if ( ret != 0)
		throw RWin32Exception(ret , "�R�s�[�Ɏ��s���܂��� target:" + inFilename + " To:" + inTo);
}
//�ړ�(�f�B���N�g���̏ꍇ�ȉ��S��)
void RDiskUtil::Move(const string & inFilename,const string & inTo, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);
	char * to = RDiskUtil::getDoubleNULLFilename(inTo);

	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_MOVE;							//����̎��
	op.pFrom = filename;						//�Ώۂ̃t�@�C��
	op.pTo   = to;                        
	op.fFlags= appendFlg;         
	op.fAnyOperationsAborted = true;			//�L�����Z���ł��邩�H
	int ret = SHFileOperation(&op);

	delete [] filename;
	delete [] to;

	if ( ret != 0)
		throw RWin32Exception(ret , "�ړ��Ɏ��s���܂��� target:" + inFilename + " To:" + inTo);
}
//�폜(�f�B���N�g���̏ꍇ�ȉ��S��)
void RDiskUtil::Delete(const string & inFilename, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);

	SHFILEOPSTRUCT op = {0};
	op.hwnd  = NULL;
	op.wFunc = FO_DELETE;				  //����̎��
	op.pFrom = filename;				  //�Ώۂ̃t�@�C��
	op.pTo   = NULL;                        
	op.fFlags= appendFlg;				  //Undo�������邩�H
	op.fAnyOperationsAborted = true;	  //�L�����Z���ł��邩�H
	int ret = SHFileOperation(&op);
	delete [] filename;

	if ( ret != 0)
		throw RWin32Exception("�폜�Ɏ��s���܂��� target:" + inFilename);

}
//�S�~����
void RDiskUtil::Gomi(const string & inFilename, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);

	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_DELETE;						//����̎��
	op.pFrom = filename;						//�Ώۂ̃t�@�C��
	op.pTo   = NULL;                        
	op.fFlags= FOF_ALLOWUNDO | appendFlg;       //Undo�������邩�H
	op.fAnyOperationsAborted = true;			//�L�����Z���ł��邩�H
	int ret = SHFileOperation(&op);

	delete [] filename;

	if ( ret != 0)
		throw RWin32Exception(ret , "�S�~���Ɉړ��Ɏ��s���܂��� target:" + inFilename );
}

//�t�@�C������ \0\0 �ŏI����Ă���K�v�����邽�ߕt������
char* RDiskUtil::getDoubleNULLFilename(const string & inStr)
{
	const string newStr = RConv::Replace(inStr , "/" , "\\");
	//�t�@�C������ \0\0 �ŏI����Ă���K�v�����邽�ߕt������.
	char * filename = new char[ newStr.size() + 2 ];
	strcpy(filename , newStr.c_str());
	filename[ newStr.size() + 1 ] = '\0';
	return filename;
}

//�f�B���N�g���̍쐬
void RDiskUtil::MakeDirectory(const string & inDirectoryName)
{
	const string newStr = RConv::Replace(inDirectoryName , "/" , "\\");

	if (newStr.size() > 2 && newStr[1] == ':')
	{
		MakeSureDirectoryPathExists( newStr.c_str() );
	}
	else
	{
		::CreateDirectory(newStr.c_str() , NULL);
	}
}


//�t�@�C�������݂��邩?
bool RDiskUtil::Exist(const string & inFileName)
{
	const string newStr = RConv::Replace(inFileName , "/" , "\\");
	return (0xFFFFFFFF != GetFileAttributes(newStr.c_str() ));
}
