// MVConfig.h: MVConfig �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVConfig_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
#define AFX_MVConfig_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


class MVConfig  
{
private:
	MVConfig();
public:
	enum SCROLL_TYPE
	{
		YOKO	=	0,	//���X�N���[��
		TATE	=	1,	//���ăX�N���[��
	};

	
	virtual ~MVConfig();


	static MVConfig* getInstance();

	void Load();
	void Save();

	void MVConfig::SavePage(const string & inFilename , int inPage);
	void MVConfig::setFlg(bool isLastMemory , bool isJustFit, int inScrollCount , SCROLL_TYPE inScrollType);

	const string getLastFilename() const
	{
		return this->LastFilename;
	}
	const int getLastPage() const
	{
		return this->LastPage;
	}
	const bool getJustFit() const
	{
		return this->JustFit;
	}
	const bool getLastMemory() const
	{
		return this->LastMemory;
	}

	const int getScrollCount()
	{
		return this->ScrollCount;
	}

	SCROLL_TYPE getScrollType() const
	{
		return this->ScrollType;
	}
private:
	//�J�����g�f�B���N�g���̎擾.
	string CommonCurrentDirectory() const;
private:
	string LastFilename;
	int LastPage;
	bool JustFit;
	bool LastMemory;
	int ScrollCount;
	SCROLL_TYPE	ScrollType;


	string CurrentDirectory;
};

#endif // !defined(AFX_MVConfig_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
