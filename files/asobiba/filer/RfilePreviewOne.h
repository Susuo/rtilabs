// RfilePreviewOne.h: RfilePreviewOne �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_)
#define AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RfileLocal.h"

//�v���r���[�p�̈�̃t�@�C����\���B
class RfilePreviewOne : public Rfile
{
public:
	RfilePreviewOne()
	{
		//�\�[�g�Ȃ�Ă��Ȃ����B
		SortKey = NULL;
	}
	virtual ~RfilePreviewOne()
	{
	}

	//�o�^���܂��B
	void Attach(const char* inName)
	{
		this->Name = inName;
	}
	//�\�[�g�L�[��o�^���܂��B Attch�̌�ɌĂ�ł�������.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey)
	{
	}

	//�l�̎擾.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const
	{
		if (inType == __FILECOLUMN_TYPE_NAME) return Name;
		return "";
	}
	//�C���[�W�̎擾.
	virtual const int getImageIndex() const
	{
		ASSERT(0);
		return 0;
	}

	//��ނ�Ԃ�.
	virtual __RFILETYPE	getType() const	
	{
		return __RFILETYPE_PREVIEWONE;
	}
	//�f�B���N�g���ł���?
	virtual bool IsDirectory() const 
	{
		return false ;
	}
private:
	string	Name;
};

#endif // !defined(AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_)
