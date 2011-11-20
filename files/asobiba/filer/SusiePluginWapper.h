// SusiePluginWapper.h: SusiePluginWapper �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_)
#define AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "SusiePlugin.h"

//Susie�����[�h�����C���[�W�ł��B
class SusiePluginLoadedPicture
{
public:
	//�Ƃ肠�����������ƁB
	SusiePluginLoadedPicture();
	//����ƁB
	virtual ~SusiePluginLoadedPicture();

	//Bitmap���̎擾.
	const BITMAPINFO* getBitmapInfo();
	//Bitmap���̂̎擾,
	const void* getBitmap();

	void setPictureInfo(PictureInfo inPictureInfo)
	{
		this->Info = inPictureInfo;
	}
	void setPicture(HLOCAL inBinfo,HLOCAL inBm)
	{
		ASSERT(inBinfo != NULL);
		ASSERT(inBm != NULL);

		ASSERT(this->Binfo == NULL);
		ASSERT(this->Bm == NULL);

		this->Binfo = inBinfo;
		this->Bm = inBm;
	}


private:
	BITMAPINFO*	BitmapInfo;
	void*		Bitmap;

	friend class SusiePluginWapper;
		PictureInfo	Info;			//�摜���.
		HLOCAL		Binfo;			//Susie �������Ƀ��[�h���܂��B
		HLOCAL		Bm;				//Susie �������Ƀ��[�h���܂��B
};


//SusiePlugin�͑f�̂܂܂ł͂ƂĂ��A�z�Ń������֌W�̑��삪���ɂ����̂�
//��������b�v����N���X�ł��B
class SusiePluginWapper  
{
private:
	SusiePlugin	Plugin;
public:
	SusiePluginWapper();
	virtual ~SusiePluginWapper();

	//�K���Ă�!
	//������������Ă��Ȃ��ꍇ�� RDoNotInitaliseException �ł��B
	//����Ȋ֐��Ȃ��ꍇ�� RNotSupportException �ł��B
	void Create(const string inDllName)
		{		Plugin.Create(inDllName);	}

	//�v���O�C���o�[�W�����̎擾
	//�G���[ RSusieConferException
	string GetPluginVersion() const throw()
		{		return Plugin.GetPluginVersion();	}
	//�v���O�C���R�s�[���C�g�̎擾
	//�G���[ RSusieConferException
	string GetPluginCopyRight() const throw()
		{		return Plugin.GetPluginCopyRight();	}
	//�v���O�C�����Ή����Ă���g���q ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�)
	//@inNo	0�`
	//ret �`��
	//���s ""
	//�G���[ RSusieConferException
	string GetPluginSupportExt(int inNo) const throw()
		{		return Plugin.GetPluginSupportExt(inNo);	}
	//�v���O�C�����Ή����Ă���t�@�C���`����
	//@inNo	0�`
	//ret �t�@�C���^�C�v
	//���s ""
	//
	//�G���[ RSusieConferException
	string GetPluginSupportFiletypeName(int inNo) const throw()
		{		return Plugin.GetPluginSupportFiletypeName(inNo);	}

	//�T�|�[�g����Ă��邩�ǂ������ׂ�
	bool IsSupported(const string inFilename,char* io2KMemory) const ;

	//�摜�̏��̎擾
	//�G���[ RSusieConferException
	void GetPictureInfo(const string inFilename,SusiePluginLoadedPicture *outPicture) const throw();

	//�W�J
	//�G���[ RSusieConferException
	void GetPicture(const string inFilename,
			SusiePluginLoadedPicture *outPicture,SusieCallable* inCallable) const throw();

};

#endif // !defined(AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_)
