#include "StartWithExeFile.h"
#ifdef DRAW_CANVAUS_LIFE

/****************************************************************/
//���R�G��
/****************************************************************/
#include <.\Graphics\rti_DrawCanvaus.h>
#include <.\Base\rti_key.h>

//�L�����o�X���s
void TDrawCanvaus::Draw(BMD TarGet,int x,int y)
{
	int DX,DY;
	DX = MouseRX - x;
	DY = MouseRY - y;
	if (DX >= 0 && DX <= Canvaus->w && DY >= 0 && DY <= Canvaus->h)
	{
		if (MouseEL)
		{
			Box(Canvaus,DX-SizeHarf,DY-SizeHarf,DX+SizeHarf,DY+SizeHarf,Color);
		}
	}
	//�L�����o�X�̊G��.
	::DirectDraw(Canvaus,0,0,Canvaus->w,Canvaus->h,TarGet,x,y);
}


#endif
