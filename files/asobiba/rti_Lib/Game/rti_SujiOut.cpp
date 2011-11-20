
#include <./Game/rti_SujiOut.h>
#include <./Graphics/rti_BackSurface.h>

//�����f�[�^���Z�b�g.
void TSujiOut::Set(BMD bmd , int width ,int hight)
{
	m_Bmd	=	bmd;		//�摜�f�[�^
	m_Width	=	width;	//��̐����̉���.
	m_Hight	=	hight;	//��̐����̏c��.
}

#define MAX_SUJI_LEN 20

//�����\������.
void TSujiOut::Draw(int Suji , int DrawX, int DrawY)
{
	int Show;
	char Buffer[MAX_SUJI_LEN];
	wsprintf(Buffer , "%d" ,Suji);

	for(int i = 0; Buffer[i] != '\0' ; i++ )
	{
		//�\�����鐔.
		Show = Buffer[i] - '0';
		//�\��.
		RtiGraphics::Draw( m_Bmd , Show * m_Width , 0 ,  
			m_Width , m_Hight , Sprite , DrawX + i * m_Width,DrawY );
	}
}

