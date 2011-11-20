// SusiePluginWapper.cpp: SusiePluginWapper �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePluginWapper.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//�Ƃ肠�����������ƁB
SusiePluginLoadedPicture::SusiePluginLoadedPicture()
{
	memset(&Info , 0 , sizeof(PictureInfo) );
	Binfo = NULL;
	Bm = NULL;
	BitmapInfo	= NULL;
	Bitmap	= NULL;
}
//����ƁB
SusiePluginLoadedPicture::~SusiePluginLoadedPicture()
{
	//���b�N����Ă���Ȃ�Unlock.
	if (this->BitmapInfo)
	{
		ASSERT(this->Binfo != NULL);

		::LocalUnlock(this->Binfo);
		this->BitmapInfo = NULL;
	}
	//���������
	::LocalFree(this->Binfo);
	this->Binfo = NULL;

	//���b�N����Ă���Ȃ�Unlock.
	if (this->Bitmap)
	{
		ASSERT(this->Bm != NULL);

		::LocalUnlock(this->Bm);
		this->Bitmap = NULL;
	}
	//���������
	::LocalFree(this->Bm);
	this->Bm = NULL;
}

//Bitmap���̎擾.
const BITMAPINFO* SusiePluginLoadedPicture::getBitmapInfo()
{
	ASSERT(this->Binfo != NULL);
	//���łɃ��b�N���Ă���ꍇ�͂��̂܂܁B
	if (this->BitmapInfo != NULL)	return this->BitmapInfo;

	this->BitmapInfo = (BITMAPINFO*) ::LocalLock(this->Binfo);
	return this->BitmapInfo;
}

//Bitmap���̂̎擾,
const void* SusiePluginLoadedPicture::getBitmap()
{
	ASSERT(this->Bm != NULL);
	//���łɃ��b�N���Ă���ꍇ�͂��̂܂܁B
	if (this->Bitmap != NULL)	return this->Bitmap;

	this->Bitmap = (void*) ::LocalLock(this->Bm);
	return this->Bitmap;
}





SusiePluginWapper::SusiePluginWapper()
{

}

SusiePluginWapper::~SusiePluginWapper()
{

}

//�T�|�[�g����Ă��邩�ǂ������ׂ�
bool SusiePluginWapper::IsSupported(const string inFilename,char* io2KMemory) const 
{
	bool ret;
	char * filename = strdup( inFilename.c_str() );
	try
	{
		ret = Plugin.IsSupported( filename ,io2KMemory );
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);

	return ret;
}

//�摜�̏��̎擾
//�G���[ RSusieConferException
void SusiePluginWapper::GetPictureInfo(const string inFilename,SusiePluginLoadedPicture *outPicture) const throw()
{
	char * filename = strdup( inFilename.c_str() );
	try
	{
		Plugin.GetPictureInfo( filename ,&(outPicture->Info) );
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);
}

void SusiePluginWapper::GetPicture(const string inFilename,
			SusiePluginLoadedPicture *outPicture,	SusieCallable* inCallable) const throw()
{
	char * filename = strdup( inFilename.c_str() );
	try
	{
		Plugin.GetPicture( filename ,&(outPicture->Binfo),&(outPicture->Bm) ,inCallable);
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);
}
