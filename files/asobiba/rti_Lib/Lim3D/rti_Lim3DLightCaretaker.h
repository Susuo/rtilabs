#ifndef	___LIM3D_LIGHTCARETAKER__H	//�d���h�~
#define ___LIM3D_LIGHTCARETAKER__H

//#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_SuperList.h>
#include <./Lim3D/rti_Lim3DLight.h>

class TLim3DLightCaretaker : public __TSuperListServer<TLim3DLight>
{
public:
	//���̐F�����߂�.
	DWORD GetColor(DWORD inRGB,
		POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
protected:
private:
};

#endif				//�d���h�~