#include "StartWithExeFile.h"
#ifdef DRAW_CANVAUS_LIFE

#ifndef ___DRAWCANVAUSH
#define ___DRAWCANVAUSH

/****************************************************************/
//���R�G��
/****************************************************************/
#include <.\Graphics\rti_graphics.h>
class TDrawCanvaus
{
	BMD	Canvaus;	//�L�����o�X
	int	Size;		//�M�̃T�C�Y
	int SizeHarf;
	int	Color;		//�M�̂���
	int	Es;			//�����S���̂���
public:
	//�R���X�g���N�^
	TDrawCanvaus(){};
	//�L�����o�X���s
	void Draw(BMD TarGet,int x,int y);
	//�L�����o�X�ݒ�
	void SetCanvaus(BMD c){Canvaus = c;};
	//�M�̃T�C�Y�ݒ�
	void SetSize(int c){Size = c; SizeHarf = c >> 1; };
	//�M�̂���ݒ�
	void SetColor(int c){Color = c;};
	//�����S���̂���ݒ�
	void SetEs(int c){Es = c;};
	BMD GetCanvaus(){return Canvaus;};
};

#endif

#endif  //LIFE END
