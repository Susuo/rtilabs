// RGISCaption.cpp: RGISCaption �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISCaption.h"
#include "RDrawObject.h"
#include "RGISKeiidoData.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISCaption::RGISCaption()
{
	this->MathAngle = RMathAngleFixed::getMathAngleFixed();
}

RGISCaption::~RGISCaption()
{

}

void RGISCaption::Create(RDIBBitmap * ioCanvas)
{
	this->Canvas = ioCanvas;

	//�������o���ꍇ�̐ݒ�.
	NewFont.Create(10 , "�l�r �S�V�b�N" , false , false , false , false  , SHIFTJIS_CHARSET);
	SelectObject( Canvas->getDC() , this->NewFont.getFont() );
	SetTextColor(Canvas->getDC(), RGB(255,255,255) );	//��
	SetBkColor(Canvas->getDC(),RGB(0,0,0) );	//��
	SetBkMode(Canvas->getDC(),OPAQUE);
}

void RGISCaption::Black(COLORREF16 color)
{
	RGraphics::Clear( this->Canvas , color);
}

void RGISCaption::Houi(RAngle deg)
{
	//�� �̂��邵������.	16x16
	RotateLine(Canvas , 8 ,  1 ,  8 ,15 , RGB24TOBGB16( RGB(255,255,255) ) , 1 , deg , 8 , 8);
	RotateLine(Canvas , 8 ,  1 ,  4 , 4 , RGB24TOBGB16( RGB(255,255,255) ) , 1 , deg , 8 , 8);
	RotateLine(Canvas , 8 ,  1 , 12 , 4 , RGB24TOBGB16( RGB(255,255,255) ) , 1 , deg , 8 , 8);
}

void RGISCaption::Zahyou(const LPOINT * inXY)
{
	RGISKeiidoData b,l;
	l.setInt(inXY->x);
	b.setInt(0xffffffff - inXY->y);

	string bstr = b.toString() + "\n" + l.toString();

	RECT rc;
	rc.left  = 16;
	rc.top   = 0;
	rc.right = 16 + 64;
	rc.bottom= 16;

#ifndef _WIN32_WCE
	DrawText( Canvas->getDC()  , bstr.c_str() , bstr.size() , &rc , DT_NOCLIP);
#else
	MY_USES_CONVERSION;
	unsigned short * wstr = _A2W(bstr.c_str());
	int wsize = lstrlen(wstr);
	DrawText( Canvas->getDC()  ,  wstr , wsize , &rc , DT_NOCLIP);

#endif
}

//��ʂ̉E���ɏk�ڂ�\������
void RGISCaption::ScaleLine(int inScale)
{
	int size = (3 * Canvas->getWidth() ) / 20;
	const int yohakuW = 10;
	const int yohakuH = 10;
	const int lineTateboH = 5;


	//|---------------------|  ����Ȃ̂�����

	int lineStart = Canvas->getWidth() - size - yohakuW;
	int lineEnd = Canvas->getWidth() - yohakuW ;

	//����
	RGraphics::Line( Canvas , lineStart , yohakuH , lineEnd , yohakuH , COLORREF16(RGB(128,255,255)) , 1);
	//�c��
	RGraphics::Line( Canvas , lineStart , yohakuH - lineTateboH, lineStart , yohakuH + lineTateboH, COLORREF16(RGB(128,255,255)) , 1);
	RGraphics::Line( Canvas , lineEnd   , yohakuH - lineTateboH, lineEnd   , yohakuH + lineTateboH, COLORREF16(RGB(128,255,255)) , 1);

	//������ �^���ɏ���.
	RECT rc;
	rc.left  = lineStart + 3;
	rc.top   = 0;
	rc.right = lineEnd - 3;
	rc.bottom= yohakuH + lineTateboH;

	//�P�ʂ̓��[�g��
	unsigned long m = (size << inScale) / 10;

	string bstr = num2str(m) + "m";

#ifndef _WIN32_WCE
	DrawText( Canvas->getDC()  , bstr.c_str() , bstr.size() , &rc , DT_NOCLIP | DT_CENTER);
#else
	MY_USES_CONVERSION;
	unsigned short * wstr = _A2W(bstr.c_str());
	int wsize = lstrlen(wstr);
	DrawText( Canvas->getDC()  ,  wstr , wsize , &rc , DT_NOCLIP | DT_CENTER);
#endif
}



void RGISCaption::RotateLine(RDIBBitmap * ioCanvas , int sx,int sy ,int ex, int ey , COLORREF16 color ,int size ,RAngle deg , int xjiku , int yjiku) const
{
	sx -= xjiku;
	sy -= yjiku;
	ex -= xjiku;
	ey -= yjiku;

	int nsx = (FtoI((sx) *MathAngle->Cos(deg) - (sy) *MathAngle->Sin(deg) ) + xjiku);
	int nsy	= (FtoI((sx) *MathAngle->Sin(deg) + (sy) *MathAngle->Cos(deg) ) + yjiku);

	int nex = (FtoI((ex) *MathAngle->Cos(deg) - (ey) *MathAngle->Sin(deg) ) + xjiku);
	int ney	= (FtoI((ex) *MathAngle->Sin(deg) + (ey) *MathAngle->Cos(deg) ) + yjiku);

	RGraphics::Line(ioCanvas , nsx ,  nsy , nex , ney , color , size );
}

