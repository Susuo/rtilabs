// RImage.h: RImage �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_)
#define AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RImage;

#include "comm.h"

class RImage
{
public:
	RImage();
	virtual ~RImage();

	//�ǂݍ���
	bool Load(const string inFilename);
	//���
	void Release();
	//HDC
	const HDC			getHDC() const				{	return	MemoryDC; }
	int					getContentsWidth() const	{	return	ContentsWidth;}
	int					getContentsHeight() const	{	return	ContentsHeight;}
	//�^�C�g�����擾����
	string getTitle() const ;

private:
	//�r�b�g�}�b�v���`����.
	//�G���[ RDoNotInitaliseException 
	void AttachHBitmap(const string inAlt ,const BITMAPINFO* inBitmapInfo,const void* inBitmapData) throw();
	//��̃r�b�g�}�b�v���`����.
	//HDC �̓������f�o�C�X�Ȃ̂ŁA������ Bitblt ���Ă��������B
	//�G���[ RDoNotInitaliseException 
	HDC AttachCanvas(const string inAlt ,int inWidth,int inHegiht) throw();
private:
	HBITMAP				Bitmap;
	HDC					MemoryDC;
	string				Alt;

	DWORD				ContentsWidth;
	DWORD				ContentsHeight;
};

#endif // !defined(AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_)
