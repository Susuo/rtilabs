#ifndef	___LIM3D_EYE__H	//�d���h�~
#define ___LIM3D_EYE__H

#include <./Lim3D/rti_Lim3DPoint.h>
#include <./Lim3D/rti_Matrix.h>
#include <./Lim3D/rti_SuperListClient.h>

class TLim3DEye : public __TSuperListClient
{
public:
	//���̎��_���g���ă����_�����O����.
	void			Select();
	//��]�s��̍쐬.
	void			MakeMatrix();
	//���_�ϊ�.
	void			EyeConbert(XYZFixed * Eye , XYZFixed * World );
	//�t���_�ϊ�.
	void			RevEyeConbert(XYZFixed * Eye , XYZFixed * World );
	//���_�̉�]�p�x���w��.
	void			SetRotate(Angle x,Angle y,Angle z);
	//���_�̏ꏊ���w��.
	void			SetPos(Fixed x,Fixed y,Fixed z);
	//���_�̉�]�p�x���擾.
	void			GetRotate(Angle *x,Angle *y,Angle *z);
	//���_�̏ꏊ���擾.
	void			GetPos(Fixed *x,Fixed *y,Fixed *z);
	//���_�̉�]�p�x�𑝂₷.
	void			AddRotate(int x,int y,int z);
	//���_�̏ꏊ���ړ�����.
	void			AddPos(Fixed x,Fixed y,Fixed z);
	//�ꏊ���N���X�ւ̃x�^�A�N�Z�X
	TLim3DPoint*	GetPoint();
	//�s��N���X�ւ̃x�^�A�N�Z�X
	TMatrix*		GetMatrix();
	//�t�s��N���X�ւ̃x�^�A�N�Z�X
	TMatrix*		GetRevMatrix();
protected:
	TLim3DPoint				m_Point;		//�ʒu
	TMatrix					m_Matrix;		//���_��]�s��.
	TMatrix					m_RevMatrix;	//���_��]�t�s��.
private:
};

//���f����`
typedef TLim3DEye* EYE;


#endif				//�d���h�~