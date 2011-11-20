// RfilesPreviewOne.h: RfilesPreviewOne �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_)
#define AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableOnlyOne.h"
#include "RfilePreviewOne.h"
#include "RfileFactory.h"

//�v���r���[�p�̈�̃t�@�C��������킷.
class RfilesPreviewOne : public Rfiles  
{
private:
public:
	RfilesPreviewOne(string inPath);
	//final
	~RfilesPreviewOne();

	//�ꗗ�擾
	virtual void ls();
	//�ꗗ�擾
	virtual void ls(string inFilter);
	//�폜
	virtual void rm(string inFilename);
	//���l�[��
	virtual void rename(string inSrcFilename,string inDestFilename);
	//��ނ�Ԃ�.
	virtual __RFILETYPE		getRfileType()	const;
	//�p�X�̎擾
	virtual string getPath() const ;
	//�p�X�̐ݒ�
	virtual void setPath(const string inPath)  ;

	//RTable���쐬���܂��B
	//ls ���g���O�ɌĂ�ł��������B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw();

	//�e�X�g.
	static void test();
private:
	string		Path;
};

#endif // !defined(AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_)
