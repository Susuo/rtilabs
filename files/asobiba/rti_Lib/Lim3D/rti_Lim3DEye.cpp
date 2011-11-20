#include <./Lim3D/rti_Lim3DMaster.h>

//���̎��_���g���ă����_�����O����.
void TLim3DEye::Select()
{
	//���_�Z���N�g.
	TLim3DMaster::SetSelectEye(this);
	//��]�s������.
	MakeMatrix();
}

//��]�s��̍쐬.
void TLim3DEye::MakeMatrix()
{
	//���_�s����쐬.
	m_Matrix.MakeTransYXZ( __ANGLE_REV( m_Point.GetAngleX() ),
						   __ANGLE_REV( m_Point.GetAngleY() ),
						   __ANGLE_REV( m_Point.GetAngleZ() ),
						m_Point.GetPosX() , m_Point.GetPosY() , m_Point.GetPosZ() );
	//�t�s����쐬���܂�.
	m_RevMatrix.FastRev( &m_Matrix );
}

//���_�ϊ�.
void TLim3DEye::EyeConbert(XYZFixed * Eye , XYZFixed * World )
{
        //���_�́A Y X Z �̏��ŉ�]�����Ȃ��Ă͂Ȃ�Ȃ�.
        //���̂́A Z X Y �̏��ŉ�]�������ł����ǂ�.
        //���Y�C�ł���.
        
        //��]���v�Z
		m_Matrix.VectorMul(Eye,World);
}

//�t���_�ϊ�.
void TLim3DEye::RevEyeConbert(XYZFixed * Eye , XYZFixed * World )
{
        //��]���v�Z
		m_RevMatrix.VectorMul(Eye,World);
}

//���_�̉�]�p�x���w��.
void			TLim3DEye::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point.SetAngleX(x);
	m_Point.SetAngleY(y);
	m_Point.SetAngleZ(z);
}
//���_�̏ꏊ���w��.
void			TLim3DEye::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(x);
	m_Point.SetPosY(y);
	m_Point.SetPosZ(z);
}
//���_�̉�]�p�x���擾.
void			TLim3DEye::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point.GetAngleX();
	*y = m_Point.GetAngleY();
	*z = m_Point.GetAngleZ();
}
//���_�̏ꏊ���擾.
void			TLim3DEye::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point.GetPosX();
	*y = m_Point.GetPosY();
	*z = m_Point.GetPosZ();
}
//���_�̉�]�p�x�𑝂₷.
void			TLim3DEye::AddRotate(int x,int y,int z)
{
	m_Point.SetAngleX(m_Point.GetAngleX() + x);
	m_Point.SetAngleY(m_Point.GetAngleY() + y);
	m_Point.SetAngleZ(m_Point.GetAngleZ() + z);
}
//���_�̏ꏊ���ړ�����.
void			TLim3DEye::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(m_Point.GetPosX() + x);
	m_Point.SetPosY(m_Point.GetPosY() + y);
	m_Point.SetPosZ(m_Point.GetPosZ() + z);
}

//�ꏊ���N���X�ւ̃x�^�A�N�Z�X
TLim3DPoint* TLim3DEye::GetPoint()
{
	return &m_Point;
}

//�s��N���X�ւ̃x�^�A�N�Z�X
TMatrix* TLim3DEye::GetMatrix()
{
	return &m_Matrix;
}


//�t�s��N���X�ւ̃x�^�A�N�Z�X
TMatrix* TLim3DEye::GetRevMatrix()
{
	return &m_RevMatrix;
}

