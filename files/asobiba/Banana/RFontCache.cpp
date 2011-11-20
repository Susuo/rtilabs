// RFontCache.cpp: RFontCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFontCache.h"
#include "RGraphics.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RFontCache::RFontCache()
{
	Cache = NULL;
}

RFontCache::~RFontCache()
{
	if ( Cache ) delete [] Cache;
}

void RFontCache::Create(int inCacheSize , int inLineMax ,int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset)
{
	NewFont.Create(inFontSize , FontName , Bold , Italic , Underline , Strike , Charset);

	//���̃t�H���g�ŕ`�悵���ꍇ�A�ǂꂭ�炢�̑傫���ɂȂ邩�����߂Ă݂�B
	{
		HDC dc = GetDC(NULL);
		SelectObject( dc , this->NewFont.getFont() );
		string testString = "���C�ۂ�ł��B���C�͏����ł������܂��B";

		SIZE size;

	#ifndef _WIN32_WCE
		GetTextExtentPoint32(dc  , testString.c_str() , testString.size() , &size );
	#else
		MY_USES_CONVERSION;
		unsigned short * wstr = _A2W(testString.c_str());
		int wsize = lstrlen(wstr);
		GetTextExtentPoint32(dc  , wstr , wsize , &size );
	#endif

		ReleaseDC(NULL,dc);

		this->Width = size.cx / testString.size();
		this->Height = size.cy;
	}

	//���m�N���r�b�g�}�b�v�̍쐬.
	Canvas.Create( this->Width * inLineMax,this->Height * inCacheSize, 16);
	SelectObject( Canvas.getDC() , this->NewFont.getFont() );
	SetTextColor(Canvas.getDC(),RGB(0,0,0) );	//��
	SetBkColor(Canvas.getDC(),RGB(255,255,255));	//��
	SetBkMode(Canvas.getDC(),OPAQUE);

	this->CacheSize = inCacheSize;
	this->Cache = new FontCacheData[inCacheSize + 1];
	memset(this->Cache , 0 , sizeof(FontCacheData) * inCacheSize);

	TRACE("FONT CACHE �T�C�Y�� %d �m�ۂ��܂���\n" , inCacheSize);
	TRACE("FONT CACHE ���z��ʂ� %d,%d �m�ۂ��܂���\n" , this->Width , this->Height);

}

void RFontCache::TextOut(RDIBBitmap* ioTargetImg ,int inX,int inY, const string & inName)
{
	//�n�b�V���l�̌v�Z.
	unsigned long hash = makeHache(inName);

	int avg = 0;
	int noEntry = -1;
	for(int i = 0 ; i < this->CacheSize ; i ++)
	{
		FontCacheData * p = &this->Cache[i];
		if ( p->Hash == hash )
		{
			p->Hit ++;

			//�L���b�V���Ƀq�b�g
//			::BitBlt(ioTargetImg->getDC() , inX , inY , p->Size.cx , p->Size.cy , Canvas.getDC() , 0 , this->Height * i , SRCAND);
			RGraphics::AndCopy( &Canvas , 0 , this->Height * i ,  p->Size.cx , p->Size.cy , ioTargetImg , inX , inY );
			return ;
		}
		else if(p->Hit == 0)
		{
			//�L���b�V���̋󂫂��݂���.
			noEntry = i;
		}
		avg += p->Hit;
	}


	//���āA�L���b�V���Ƀq�b�g���Ȃ������킯����.
	TRACE("������ %s �̓L���b�V���ɂ���܂���\n" , inName.c_str() );

	if (noEntry == -1)
	{
		//�L���b�V�������t�Ȃ킯����
		TRACE("FONT CACHE �͂��Ȃ������ς��ł��B\n");

		avg = avg / this->CacheSize / 2;

		//�q�b�g�������ψȉ��̂�����ׂĉ������.
		for(int i = 0 ; i < this->CacheSize ; i ++)
		{
			FontCacheData * p = &this->Cache[i];
			if (p->Hit <= avg)
			{
				TRACE("���ψȉ��� %d (%d/%d)�𗎑悳���܂���\n" , i , p->Hit , avg);

				p->Hash = 0;
				p->Hit = 0;
				noEntry = i;	//��ɏ㏑�����A�Ō�ɏ������񂾂������͗��p����.

				//��������
				::BitBlt(Canvas.getDC() , 0 , this->Height * i , Canvas.getWidth() , this->Height , NULL , 0 , 0 , WHITENESS);
			}
		}
		//�����N�����藎�Ƃ��Ȃ��ꍇ�́A���ׂļުɻ���
		if (noEntry == -1)
		{
			memset(this->Cache , 0 , sizeof(FontCacheData) * this->CacheSize);
			::BitBlt(Canvas.getDC() , 0 , 0 , Canvas.getWidth() , Canvas.getHeight() , NULL , 0 , 0 , WHITENESS);
			noEntry = 0;
		}
	}

	//�ŁA�V�K�쐬����킯����.
	TRACE("�V�K�� %d �Ɋ��蓖�Ă܂���\n" , noEntry);
	{
		FontCacheData * p = &this->Cache[noEntry];
		RECT rc ;
		rc.top = this->Height * noEntry;
		rc.left = 0;
		rc.bottom = this->Height;
		rc.right = Canvas.getWidth();

	#ifndef _WIN32_WCE
		GetTextExtentPoint32( Canvas.getDC() , inName.c_str() , inName.size() , &p->Size );
		DrawText( Canvas.getDC()  , inName.c_str() , inName.size() , &rc , DT_NOCLIP);
	#else
		MY_USES_CONVERSION;
		unsigned short * wstr = _A2W(inName.c_str());
		int wsize = lstrlen(wstr);
		GetTextExtentPoint32( Canvas.getDC() , wstr , wsize , &p->Size );
		DrawText( Canvas.getDC()  , wstr , wsize , &rc , DT_NOCLIP);
	#endif

		p->Hash = hash;
		p->Hit = 1;

		//�ŁA���ʂ�]���ƁA�A���[�Ȃ��������B
//		::BitBlt(ioTargetImg->getDC() , inX , inY , p->Size.cx , p->Size.cy , Canvas.getDC() , 0 , rc.top , SRCAND);
		RGraphics::AndCopy( &Canvas ,  0 , rc.top,  p->Size.cx , p->Size.cy , ioTargetImg , inX , inY );
	}

}

//�����񂪈�v���邩�ǂ����̓n�b�V���Ŕ��f���܂��B
// ?/40�� �ŏd�����܂�(w �܂��A���������Ȃ�.
unsigned long RFontCache::makeHache(const string &inName) const
{
	unsigned long ret = 0;

	for( const char * p = inName.c_str() ; *p ; p++)
		ret += (unsigned char) *p;

	return ret;
}
