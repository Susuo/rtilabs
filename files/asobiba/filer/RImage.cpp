// RImage.cpp: RImage �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImage.h"
#include "SusiePluginManager.h"
#include "NativeSupportPicture.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RImage::RImage()
{
	this->Bitmap = NULL;
	this->MemoryDC = NULL;

	ContentsWidth = 0;
	ContentsHeight = 0;

}

RImage::~RImage()
{
	this->Release();
}


//�ǂݍ���
bool RImage::Load(const string inFilename)
{
	const string ext = Rfiles::Filename2Ext( inFilename );

	//OS���Ή����Ă���Ȃ�AOS�Ń��[�h.
	//����̓Q�C�c�܂�.
	try
	{
		NativeSupportPicture nativeSupportImage;
		nativeSupportImage.Load( inFilename );

		LONG width,height;
		nativeSupportImage.getImageSize(&width,&height);

		this->AttachCanvas("",width,height);
		nativeSupportImage.getImage( this->AttachCanvas("",width,height) , width,height);

		return true;
	}
	catch(RNotSupportException e)
	{
		TRACE("�l�C�e�B���ł̓T�|�[�g����Ă܂���B�B�B");

	}

	//����ASUSIE �ɂł���点�邩�B
	SusiePluginLoadedPicture	LoadedPicture;
	SusieCallable callBack;
	try
	{
		SusiePluginManager* manager = SusiePluginManager::getInstance(  );
		const SusiePluginWapper* plugin = manager->getSupportPlugin(inFilename , ext);
		plugin->GetPictureInfo( inFilename , &LoadedPicture);
		plugin->GetPicture( inFilename , &LoadedPicture , &callBack);

		this->AttachHBitmap("",  LoadedPicture.getBitmapInfo() , LoadedPicture.getBitmap() );

		return true;
	}
	//�T�|�[�g����Ă��Ȃ����[�ł��B
	catch(RNotSupportException e)
	{
		printf("%s\n",e.getMessage() );
	}
	//SUSIE���G���[�͂��܂����B
	catch(RSusieConferException e)
	{
		printf("%s\n",e.getMessage() );
	}
	return false;
}



//�r�b�g�}�b�v���`����.
//�G���[ RDoNotInitaliseException 
void RImage::AttachHBitmap(const string inAlt ,const BITMAPINFO* inBitmapInfo,const void* inBitmapData) throw()
{
	this->Release();
	this->Alt = inAlt;

	int imageSize;
	if (inBitmapInfo->bmiHeader.biSizeImage)	imageSize = inBitmapInfo->bmiHeader.biSizeImage;
	else										imageSize = inBitmapInfo->bmiHeader.biWidth * inBitmapInfo->bmiHeader.biHeight * (inBitmapInfo->bmiHeader.biBitCount/8);

	this->ContentsWidth =  inBitmapInfo->bmiHeader.biWidth;
	this->ContentsHeight = inBitmapInfo->bmiHeader.biHeight;
	
	HDC hdc = GetDC(NULL);
	if (! hdc)
	{
		throw RDoNotInitaliseException("GetDC�Ɏ��s���܂���");
	}
	void* bitmapData;
	this->Bitmap = CreateDIBSection( hdc, inBitmapInfo , DIB_RGB_COLORS, (void **)&bitmapData, NULL, 0 );
	memcpy(bitmapData , inBitmapData ,	imageSize);
	this->MemoryDC = CreateCompatibleDC( hdc );
	if( ! this->MemoryDC )
	{
		DeleteObject( this->Bitmap  );
		this->Bitmap = NULL;
		this->MemoryDC = NULL;

		throw RDoNotInitaliseException("������DC�����܂���ł���");
	}
	//�֘A�t��
	SelectObject(  this->MemoryDC ,  this->Bitmap  );
	//�ӂ[�� DC �̍폜.
	ReleaseDC(NULL,hdc);

}

//�r�b�g�}�b�v���`����.
//�G���[ RDoNotInitaliseException 
HDC RImage::AttachCanvas(const string inAlt ,int inWidth,int inHegiht) throw()
{
	this->Release();
	this->Alt = inAlt;

	this->ContentsWidth =  inWidth;
	this->ContentsHeight = inHegiht;
	
	HDC hdc = GetDC(NULL);
	if (! hdc)
	{
		throw RDoNotInitaliseException("GetDC�Ɏ��s���܂���");
	}
	this->Bitmap = ::CreateCompatibleBitmap(hdc , inWidth , inHegiht);

	this->MemoryDC = CreateCompatibleDC( hdc );
	if( ! this->MemoryDC )
	{
		DeleteObject( this->Bitmap  );
		this->Bitmap = NULL;
		this->MemoryDC = NULL;

		throw RDoNotInitaliseException("������DC�����܂���ł���");
	}
	//�֘A�t��
	SelectObject(  this->MemoryDC ,  this->Bitmap  );
	//�ӂ[�� DC �̍폜.
	ReleaseDC(NULL,hdc);

	return this->MemoryDC;
}

//���
void RImage::Release()
{
	if (this->Bitmap)
	{
		DeleteObject( this->Bitmap );
		this->Bitmap = NULL;
	}
	if (this->MemoryDC)
	{
		DeleteDC( this->MemoryDC );
		this->MemoryDC = NULL;
	}
}


//�^�C�g�����擾����
string RImage::getTitle() const 
{
	return Alt;
}

