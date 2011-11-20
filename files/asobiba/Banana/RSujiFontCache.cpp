// RSujiFontCache.cpp: RSujiFontCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RSujiFontCache.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSujiFontCache::RSujiFontCache()
{
}

RSujiFontCache::~RSujiFontCache()
{
}

void RSujiFontCache::Create(int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset)
{
	NewFont.Create(inFontSize , FontName , Bold , Italic , Underline , Strike , Charset);


	//���̃t�H���g�ŕ`�悵���ꍇ�A�ǂꂭ�炢�̑傫���ɂȂ邩�����߂Ă݂�B
	{
		HDC dc = GetDC(NULL);
		SelectObject( dc , this->NewFont.getFont() );
		string testString = "0123456789";

		SIZE size;
	#ifndef _WIN32_WCE
		GetTextExtentPoint32(dc  , testString.c_str() , testString.size() , &size );
	#else
		MY_USES_CONVERSION;
		GetTextExtentPoint32(dc  , _A2W(testString.c_str()) , testString.size() , &size );
	#endif

		ReleaseDC(NULL,dc);

		this->Width = size.cx / testString.size();
		this->Height = size.cy;
	}

	//���m�N���r�b�g�}�b�v�̍쐬.
	Canvas.Create( this->Width * 10,this->Height , 16);
	SelectObject( Canvas.getDC() , this->NewFont.getFont() );
	SetTextColor(Canvas.getDC(),RGB(0,0,0) );	//��
	SetBkColor(Canvas.getDC(),RGB(255,255,255));	//��
	SetBkMode(Canvas.getDC(),OPAQUE);

	//���ۂɐ�������������.
	RECT rc = {0};
	for(int i = 0; i < 10 ; i++)
	{
		SIZE size;

	#ifndef _WIN32_WCE
		char buf[2];
		itoa(i , buf, 10);
		GetTextExtentPoint32( Canvas.getDC() , buf , 1 , &size );
		DrawText( Canvas.getDC()  , buf , 1 , &rc , DT_NOCLIP);
	#else
		TCHAR buf[2];
		_itow(i , buf, 10);
		GetTextExtentPoint32( Canvas.getDC() , buf , 1 , &size );
		DrawText( Canvas.getDC()  , buf , 1 , &rc , DT_NOCLIP);
	#endif
		this->XPoint[i] = rc.left;
		this->YWidth[i] = size.cx;
		rc.left += size.cx;
	}

}

void RSujiFontCache::TextOut(HDC ioTargetDC ,int inX,int inY, unsigned int inNum) 
{

	unsigned int target = inNum;

	//�����𐔂���.
	int scal;
	for(scal = 10  ; target >= scal ; scal *= 10)
		;

	int drawX = inX;

	//��̌�����`��.
	for( scal/=10 ; scal ; scal/=10 )
	{
		int p = target / scal;
		target = target % scal;

		ASSERT(p >=0 && p <= 9);

		::BitBlt(ioTargetDC , drawX  , inY , this->YWidth[p] ,this->Height , Canvas.getDC() , this->XPoint[p] , 0 , SRCAND);
		drawX += this->YWidth[p];
	}
}

