#ifndef	___LIM3D_CLIPPING__H	//�d���h�~
#define ___LIM3D_CLIPPING__H

#include <./Lim3D/rti_Lim3DDefine.h>

//�N���b�v���ʂ̈ʒu.
#define Z_CLIPPING	15

class TLim3DPolygonClipping
{
public:
	static int DoClipping(DrawUse * du , const PolygonDataEx* pdp , int Many);
protected:
	//�l�p�`�ɂ����� 1�_�N���b�v�����.	>>�܊p�`�ɂȂ�.
	static bool ClippingShiftIntoPentagon
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//�l�p�`�ɂ����� 2�_�N���b�v�����.	>>�l�p�`�ɂȂ�.
	static bool ClippingShiftIntoSquare
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//�l�p�`�ɂ����� 3�_�N���b�v�����.	>>�O�p�`�ɂȂ�.
	static bool ClippingShiftIntoTriangle
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//�O�p�`�ɂ����� 1�_�N���b�v�����.	>>�l�p�`�ɂȂ�.
	static bool ClippingTriangleShiftIntoSquare
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//�O�p�`�ɂ����� 2�_�N���b�v�����.	>>�O�p�`�ɂȂ�.
	static bool ClippingTriangleShiftIntoTriangle
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//pdp1 �� pdp2 �̊Ԃɂ��� �N���b�v���� (Z_CLIPPING) �ɂ�镪��.
	static void LineCut( DrawUse * du ,
					const PolygonDataEx* pdp1 , const PolygonDataEx* pdp2 );
	//pdp �� du �ɂӂ[�ɃR�s�[����.
	static void NormalCopy( DrawUse * du ,	const PolygonDataEx* pdp );
private:
};

#endif				//�d���h�~