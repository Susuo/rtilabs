// SusiePlugin.cpp: SusiePlugin �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePlugin.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SusiePlugin::SusiePlugin()
{
}
SusiePlugin::~SusiePlugin()
{
}

//������������Ă��Ȃ��ꍇ�� RDoNotInitaliseException �ł��B
//����Ȋ֐��Ȃ��ꍇ�� RNotSupportException �ł��B
void SusiePlugin::Create(const string inDllName) throw()
{
	Lib.Create(inDllName.c_str() );
	ExportGetPluginInfo = (GETPLUGININFO) Lib.GetProcAddress("GetPluginInfo");
	ExportIsSupported = (ISUPPORTED) Lib.GetProcAddress("IsSupported");
	ExportGetPictureInfo = (GETPICTUREINFO) Lib.GetProcAddress("GetPictureInfo");
	ExportGetPicture = (GETPICTURE) Lib.GetProcAddress("GetPicture");
}


//�v���O�C���o�[�W�����̎擾
//�G���[ RSusieConferException
string SusiePlugin::GetPluginVersion() const throw()
{
	char buf[MAX_PATH+4];	//���߂Ɋm��
	int ret;
	try
	{
		ret = ExportGetPluginInfo(0 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	}
	buf[ret] = '\0';

	return buf;
}
//�v���O�C���R�s�[���C�g�̎擾
string SusiePlugin::GetPluginCopyRight() const throw()
{
	char buf[MAX_PATH+4];	//���߂Ɋm��
	int ret;
	try
	{
		ret = ExportGetPluginInfo(1 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	}
	buf[ret] = '\0';

	return buf;
}
//�v���O�C�����Ή����Ă���g���q ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�)
//@inNo	0�`
//ret �`��
//���s ""
string SusiePlugin::GetPluginSupportExt(int inNo) const throw()
{
	char buf[MAX_PATH+4];	//���߂Ɋm��
	int ret;
	try
	{
		ret = ExportGetPluginInfo(2 + inNo*2 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	}
	buf[ret] = '\0';

	return buf;
}

//�v���O�C�����Ή����Ă���t�@�C���`����
//@inNo	0�`
//ret �t�@�C���^�C�v
//���s ""
string SusiePlugin::GetPluginSupportFiletypeName(int inNo) const throw()
{
	char buf[MAX_PATH+4];	//���߂Ɋm��
	int ret;
	try
	{
		ret = ExportGetPluginInfo(3 + inNo*2 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	}
	buf[ret] = '\0';

	return buf;
}




//�T�|�[�g����Ă��邩�ǂ������ׂ�
bool SusiePlugin::IsSupported(char* ioFilename,char* io2KMemory) const
{
	int ret;
	try
	{
		ret = ExportIsSupported(ioFilename , (DWORD)io2KMemory);
	}
	catch(...){	return false;	}	//��O�ł͂Ȃ��̂Œ���

	return (ret != 0);
}


//�摜�̏��̎擾
//�G���[ RSusieConferException
void SusiePlugin::GetPictureInfo(char* ioFilename,	PictureInfo *outinfo) const throw()
{

	TRACE("%s\n" , GetPluginCopyRight().c_str() );
	int ret;

	try
	{
		ret = ExportGetPictureInfo(ioFilename,0 ,0	 , outinfo );
	}
	catch(...){	throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	}
	if ( ret == 2 ) ret = 0;	//SUSIE�̃{�P�̃o�O�΍�.
	SusieResult( ret ,"GetPictureInfo");
}


//�W�J
//�G���[ RSusieConferException
void SusiePlugin::GetPicture(char* ioFilename,
			HLOCAL *outBinfo, HLOCAL *outBm,
			SusieCallable* inCallable) const throw()
{
	int ret;

	//���[�h
	try
	{
		ret = ExportGetPicture(ioFilename,0,0 , 
			outBinfo , outBm ,	(FARPROC)SusieCallable::ProgressCallback , (DWORD)inCallable);
	}
	catch(...)
	{
		throw RSusieConferException(0,"�v���O�C������O���΂��܂���");	
	}
	if ( ret == 2 ) ret = 0;	//SUSIE�̃{�P�̃o�O�΍�.

	SusieResult( ret , "GetPicture");
}


//Susie�̖߂�l�̏���
//�G���[ RSusieConferException
void SusiePlugin::SusieResult(int inRetCode,const char* inFuncName) const throw()
{
	//����I��
	if (inRetCode == 0) return ;

	switch(inRetCode)
	{
	case -1:
		throw RSusieConferException(inRetCode,"%s: ���̋@�\�̓C���v�������g����Ă��Ȃ�", inFuncName);
		break;
	case 1:
		throw RSusieConferException(inRetCode,"%s: �R�[���o�b�N�֐�����0��Ԃ����̂œW�J�𒆎~����", inFuncName);
		break;
	case 2:
		throw RSusieConferException(inRetCode,"%s: ���m�̃t�H�[�}�b�g", inFuncName);
		break;
	case 3:
		throw RSusieConferException(inRetCode,"%s: �f�[�^�����Ă���", inFuncName);
		break;
	case 4:
		throw RSusieConferException(inRetCode,"%s: �������[���m�ۏo���Ȃ�", inFuncName);
		break;
	case 5:
		throw RSusieConferException(inRetCode,"%s: �������[�G���[�iLock�o���Ȃ��A���j", inFuncName);
		break;
	case 6:
		throw RSusieConferException(inRetCode,"%s: �t�@�C�����[�h�G���[", inFuncName);
		break;
	case 8:
		throw RSusieConferException(inRetCode,"%s: �����G���[", inFuncName);
		break;
	default:
		throw RSusieConferException(inRetCode,"%s: ��`�O�G���[", inFuncName);
		break;
	}
}

