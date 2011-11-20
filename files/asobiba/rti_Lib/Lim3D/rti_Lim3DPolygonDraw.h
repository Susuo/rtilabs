#ifndef	___LIM3D_DRAW__H	//�d���h�~
#define ___LIM3D_DRAW__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Lim3DMaterial.h>

class TLim3DPolygon;

class TLim3DPolygonDraw
{
public:
	void DoDraw(TLim3DPolygon* Poly , DrawUse* du,int Many);
protected:
	//���C�e�B���O�Ȃ�,�e�N�X�`���Ȃ�.
	void NonLightNonTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//���C�e�B���O�Ȃ�,�e�N�X�`������.
	void NonLightTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//���C�e�B���O����,�e�N�X�`���Ȃ�.
	void LightNonTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//���C�e�B���O����,�e�N�X�`������.
	void LightTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
private:
	//���C�e�B���O���ꂽ�F�����߂܂�.
	int ClasFillLightingColor
		(int inRGB , TLim3DPolygon* inPoly , XYZFixed* const inPos, TLim3DMaterial* inMaterial);
};

#include <./Lim3D/rti_Lim3DPolygon.h>

#endif				//�d���h�~