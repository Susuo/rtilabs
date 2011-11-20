// RfilesDebug.h: RfilesDebug �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_)
#define AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfilesDebug;

#include "comm.h"

#include "Rfiles.h"

class RfilesDebug  : public Rfiles
{
private:
public:
	RfilesDebug() {}
	virtual ~RfilesDebug(){}

	//�ꗗ�擾
	virtual void ls(RTable * rt){}
	//�ꗗ�擾
	virtual void ls(string inFilter,RTable * rt){}
	//�폜
	virtual void rm(string inFilename){}
	//���l�[��
	virtual void rename(string inSrcFilename,string inDestFilename){}
	//��ނ�Ԃ�.
	virtual __RFILETYPE		getRfileType()	const
	{
		return __RFILETYPE_DEBUG;
	}
	//RTable���쐬���܂��B
	//ls ���g���O�ɌĂ�ł��������B
	//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw() {}
	//�p�X�̎擾
	virtual string getPath() const 
	{
		return "";
	}
	//�p�X�̐ݒ�
	virtual void setPath(const string inPath)  
	{
		return ;
	}


};

#endif // !defined(AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_)
