// NativeSupportPicture.cpp: NativeSupportPicture �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "NativeSupportPicture.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

NativeSupportPicture::NativeSupportPicture()
{
	this->OlePicture = NULL;
	this->Bm = NULL;
}
//���.
NativeSupportPicture::~NativeSupportPicture()
{
	if ( this->OlePicture )
	{
		this->OlePicture->Release();
		this->OlePicture = NULL;
	}

	if (this->Bm)
	{
		GlobalFree(this->Bm);
		this->Bm = NULL;
	}
}

//���[�h
//���s�� RNotSupportException
void NativeSupportPicture::Load(const string inFilename) throw()
{
	ASSERT(this->Bm == NULL);
	ASSERT(this->OlePicture == NULL);

	HANDLE file = ::CreateFile(inFilename.c_str() , GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) throw RNotSupportException("�t�@�C�� %s ���݂���܂���");

	// �t�@�C���T�C�Y�̎擾
	DWORD fileSize = ::GetFileSize(file, NULL);
	// �ǂݍ��݂悤�o�b�t�@�̊m��
	this->Bm = ::GlobalAlloc(GPTR, fileSize);
	DWORD readByte;
	::ReadFile(file, this->Bm , fileSize, &readByte, NULL);
	CloseHandle(file);

	IStream *iStream;
	if(!SUCCEEDED(::CreateStreamOnHGlobal( this->Bm ,FALSE,&iStream))) 
	{
		throw RNotSupportException("CreateStreamOnHGlobal �Ɏ��s���܂���");
	}
	if(!SUCCEEDED(::OleLoadPicture(iStream,fileSize, TRUE, IID_IPicture, (LPVOID*)&this->OlePicture)))
	{
		iStream->Release();
		throw RNotSupportException("OleLoadPicture �Ɏ��s���܂���");
	}
	iStream->Release();

}

//�C���[�W�̃T�C�Y�̎擾
void NativeSupportPicture::getImageSize(LONG *outWidth,LONG *outHeight) const
{
	LONG width,height;

	this->OlePicture->get_Width(&width);
	this->OlePicture->get_Height(&height);

	HDC hdc = ::GetDC(NULL);
	width  = ::MulDiv(width,::GetDeviceCaps(hdc,LOGPIXELSX),2540);
	height = ::MulDiv(height,::GetDeviceCaps(hdc,LOGPIXELSY),2540);
	::ReleaseDC(NULL,hdc);


	*outWidth	= width ;
	*outHeight	= height ;
}

//�C���[�W���f�o�C�X�R���e�L�X�g�ɕ`��
//���s�� RNotSupportException
void NativeSupportPicture::getImage(HDC ioMemDC , LONG inWidth,LONG inHeight)  const throw()
{
	LONG orignalWidth,orignalHeight;
	this->OlePicture->get_Width(&orignalWidth);
	this->OlePicture->get_Height(&orignalHeight);

	if ( this->OlePicture->Render(ioMemDC, 0, 0, inWidth,inHeight , 0, orignalHeight, orignalWidth, -orignalHeight, NULL) != S_OK)
	{
		throw RNotSupportException("�f�o�C�X�R���e�L�X�g�ɏ������߂܂���ł���");
	}
}