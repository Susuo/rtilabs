// RResorceImageLoad.cpp: RResorceImageLoad �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RResorceImageLoad.h"
#include "WindowsMain.h"
#include <ole2.h>
#include <olectl.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RResorceImageLoad::RResorceImageLoad()
{

}

RResorceImageLoad::~RResorceImageLoad()
{

}

void RResorceImageLoad::Load(RDIBBitmap * outImage, const string & inResourceName) throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
#ifndef  _WIN32_WCE
	HANDLE handle = LoadBitmap(winMain->getInstance() , inResourceName.c_str()  );
#else
	MY_USES_CONVERSION;
	HANDLE handle = LoadBitmap(winMain->getInstance() , _A2W(inResourceName.c_str())  );
#endif
	if (handle == NULL)
	{
		throw RWin32Exception("���\�[�X %s �̃C���[�W�����[�h�ł��܂���" , inResourceName.c_str() );
  	}
	HDC dc = CreateCompatibleDC(NULL);
	if (dc == NULL)
	{
		throw RWin32Exception("���\�[�X��`�悷��̂ɕK�v��DC ���擾�ł��܂���"  );
  	}

	SelectObject(dc , handle);

	outImage->Create( outImage->getWidth() , outImage->getHeight() , 16);

	BitBlt(outImage->getDC() , 0 , 0, outImage->getWidth() , outImage->getHeight() , dc , 0 , 0 , SRCCOPY);

	DeleteObject(handle);
	DeleteDC(dc);
}

void RResorceImageLoad::LoadFile(RDIBBitmap * outImage, const string & inFilename) throw(RException)
{
	BITMAP bmp_info;
	HDC memDC = NULL;
	HDC hdc   = NULL;
	HBITMAP hBit = NULL;
	try
	{
		//�f�o�C�X�R���e�L�X�g�擾
		hdc = GetDC(NULL);
		//������DC�擾
		memDC = CreateCompatibleDC(hdc);

		//DC �J��
		ReleaseDC(NULL,hdc);

		//�r�b�g�}�b�v�n���h������
#ifndef _WIN32_WCE
		hBit = (HBITMAP) LoadImage(NULL, inFilename.c_str() , IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
#else
		MY_USES_CONVERSION;
		hBit = SHLoadDIBitmap( _A2W(inFilename.c_str() ) );
#endif
		if (!hBit)
		{
			 throw RException(EXCEPTIONTRACE + "LoadImage �Ɏ��s:" + inFilename);
		}

		GetObject(hBit, sizeof(BITMAP), &bmp_info);
		SelectObject(memDC,hBit);   //�r�b�g�}�b�v�I��

		outImage->Create( bmp_info.bmWidth , bmp_info.bmHeight  , 16);

		//�摜�]��(�蔲���`)
		BitBlt( outImage->getDC() ,0,0,bmp_info.bmWidth , bmp_info.bmHeight , memDC,0,0,SRCCOPY);

		DeleteDC(memDC)  ;       //������DC�J��
		DeleteObject(hBit) ;    //�r�b�g�}�b�v�n���h���폜
	}
	catch(RException e)
	{
		if (hdc)	ReleaseDC(NULL,hdc);		   //DC�j��
		if (memDC)  DeleteDC(memDC);       //������DC�J��
		if (hBit)   DeleteObject(hBit);    //�r�b�g�}�b�v�n���h���폜
		throw;
	}
}

