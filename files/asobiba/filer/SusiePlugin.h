// SusiePlugin.h: SusiePlugin �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_)
#define AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RSusieConferException.h"
#include "SusieCallable.h"
#include "RNotSupportException.h"
#include "RAutoLibraryWithException.h"

struct PictureInfo
{
	long left,top;    //�摜��W�J����ʒu
	long width;       //�摜�̕�(pixel)
	long height;      //�摜�̍���(pixel)
	WORD x_density;   //��f�̐����������x
	WORD y_density;   //��f�̐����������x
	short colorDepth; //�P��f�������bit��
	HLOCAL hInfo;     //�摜���̃e�L�X�g���
};


class SusiePlugin  
{
private:
	//DLL���ȒP�Ɏg�����߂̃N���X.
	RAutoLibraryWithException  Lib;

	//int WINAPI GetPluginInfo(int infono,char* iobuf,int inbuflen)
	typedef int (WINAPI *GETPLUGININFO)    (int infono,char* iobuf,int inbuflen);
	GETPLUGININFO ExportGetPluginInfo;

	//int WINAPI IsSupported(LPSTR filename,DWORD dw)
	typedef int (WINAPI *ISUPPORTED)    (char* ioFilename,DWORD inDw);
	ISUPPORTED ExportIsSupported;

	//int WINAPI GetPictureInfo(char* inBuffer,long inLen,unsigned int inFlag,struct PictureInfo *outInfo);
	typedef int (WINAPI *GETPICTUREINFO)    (char* inBuffer,long inLen,unsigned int inFlag,struct PictureInfo *outInfo);
	GETPICTUREINFO ExportGetPictureInfo;

	//int WINAPI GetPicture(char* ioBuffer,long inLen,unsigned int inFlag,HANDLE *outHBInfo,HANDLE *outHBm,FARPROC inPrgressCallback,long inData)
	typedef int (WINAPI *GETPICTURE)    (char* ioBuffer,long inLen,unsigned int inFlag,HANDLE *outHBInfo,HANDLE *outHBm,FARPROC inPrgressCallback,long inData);
	GETPICTURE ExportGetPicture;

	//Susie�̖߂�l�̏���
	//�G���[�� RSusieConferException
	void SusieResult(int inRetCode,const char* inFuncName)  const throw();

public:
	SusiePlugin() ;
	virtual ~SusiePlugin();

	//�K���Ă�!
	//������������Ă��Ȃ��ꍇ�� RDoNotInitaliseException �ł��B
	//����Ȋ֐��Ȃ��ꍇ�� RNotSupportException �ł��B
	void Create(const string inDllName);

	//�v���O�C���o�[�W�����̎擾
	//�G���[ RSusieConferException
	string GetPluginVersion() const throw();
	//�v���O�C���R�s�[���C�g�̎擾
	//�G���[ RSusieConferException
	string GetPluginCopyRight() const throw();
	//�v���O�C�����Ή����Ă���g���q ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�)
	//@inNo	0�`
	//ret �`��
	//���s ""
	//�G���[ RSusieConferException
	string GetPluginSupportExt(int inNo) const throw();
	//�v���O�C�����Ή����Ă���t�@�C���`����
	//@inNo	0�`
	//ret �t�@�C���^�C�v
	//���s ""
	//
	//�G���[ RSusieConferException
	string GetPluginSupportFiletypeName(int inNo) const throw();


	//�T�|�[�g����Ă��邩�ǂ������ׂ�
	bool IsSupported(char* ioFilename,char* io2KMemory) const ;

	//�摜�̏��̎擾
	//�G���[ RSusieConferException
	void GetPictureInfo(char* ioFilename,PictureInfo *outinfo) const throw();

	//�W�J
	//�G���[ RSusieConferException
	void GetPicture(char* ioFilename,
			HLOCAL *outBinfo, HLOCAL *outBm,
			SusieCallable* inCallable) const throw();
};

#endif // !defined(AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_)
