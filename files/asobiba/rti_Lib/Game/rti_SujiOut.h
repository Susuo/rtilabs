
#ifndef __SUJI_OUT
#define __SUJI_OUT

#include <./Graphics/rti_Graphics.h>

class TSujiOut
{
public:
	//�����f�[�^���Z�b�g.
	void Set(BMD bmd , int width ,int hight);
	//�����\������.
	void Draw(int Suji , int DrawX, int DrawY);
protected:
	BMD m_Bmd;		//�摜�f�[�^
	int m_Width;	//��̐����̉���.
	int m_Hight;	//��̐����̏c��.
};


#endif					//
