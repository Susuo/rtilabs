#include <./Lim3D/rti_Lim3DMaster.h>


//���C�e�B���O
DWORD TLim3DLight::Lighting(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	//���C�g�̌��ʂɂ���ĐU�蕪��.
	switch( inMaterial->GetLighting() )
	{
	case NONE_LIGHTING:		//���C�e�B���O�Ȃ�
		break;
	case DEFUSE_LIGHTING:	//�f�B�t���[�Y
		return Diffuse( inPoly ,  inPos , inMaterial);
		break;
	case SPECULAR_LIGHTING:	//�X�y�L�����[
		return Specular( inPoly ,  inPos , inMaterial);
		break;
	case AMBIENT_LIGHTING:	//�A���r�G���g
		return Ambient( inPoly , inMaterial);
		break;
	case LAMBERT_LIGHTING:	//�����o�[ (�f�B�t���[�Y + �A���r�G���g)
		return Lambert( inPoly ,  inPos , inMaterial);
		break;
	case PHONG_LIGHTING:	//�t�H��   (�f�B�t���[�Y + �A���r�G���g + �X�y�L�����[)
		return Phong( inPoly ,  inPos , inMaterial);
		break;
	}
	return 0;
}

//�f�B�t���[�Y
DWORD TLim3DLight::Diffuse(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	/*
	*  ^     ^
	*   \ cos|
	*    \ ��|
	*revL \  |N       revL �́A����Ɍ������Ă܂�.
	*      \ |        N    �́A��ɂނ����Ă܂�.
	*       \|
	*------------------                      
	*       �|���S�������
	*/
	XYZFixed		theRevLight;			//�t���C�g�x�N�g��
	//pos �ɑ΂��邱�̃��C�g�̋t���C�g�x�N�g�������߂�.
    GetRevLightVector(&theRevLight,inPos);
	Fixed theDiffuse;
	{
		register Fixed	theCosA;
		// revLight � HousenVector  �� ���邳(-1�`1)�����܂�܂�.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
        if (theCosA < 0)
        {	//���͂������Ă܂���.
    		return 0;
        }
		theDiffuse = FixedMul( theCosA, inMaterial->GetDiffuseReflexFactor() );
	}
	if (theDiffuse <= 0)
	{	//���ׂ� 0
   		return 0;
	}
	if (theDiffuse >= IF(1) ) 
	{	//���̂܂�܂̐F
		return m_RGB;
	}

	//���̃��C�g�ɂ��F�����.
	return MulColor(m_RGB,theDiffuse);
}

//�X�y�L�����[
DWORD TLim3DLight::Specular(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	/*
	* �X�y�L�����[�͂������Ƃ�₱�����ł�.
	*       ^   ^N  ^       N �́A�@���x�N�g�� ��Ɍ������Ă܂�.
	*       L\  |  \R	   	L �́A�t�����x�N�g�� ����Ɍ������Ă܂�.
	*         \ | \         R �́A���ˌ��x�N�g�� �E��Ɍ������Ă܂�.
	*          \|\          �܂��A  R �����߂܂�.
	*  -------------------
	*    �|���S���̖�
	*Tag1
	*
	*
	*    ^cosa^
	*     \   |N          L�N �ɂ��A Cos a �����߂�.
	*     L\  |           
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag2
	*         ^
	*         |
	*    ^ ---|P		L * CosA�ɂ��A N �� L �𓊉e���܂�.
	*     \   |         ����ƁA N �̑傫���́A CP �ƂȂ�܂�.
	*     L\  |N        CP �� 2 �{���܂�.   
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag3
	*
	*
	*         ^	           2�{����CP  �ƁA L �𑫂��Z���܂�.
	*        /|            ����ƁA�x�N�g�� G �����܂�܂�.
	*     G / |            �܂�A�񓙕ӎO�p�`��������킯�ł�.
	*      /  |
	*     /   |            �����āA �Ȃ�ƁA G �́A R �Ɠ����ɂȂ�̂ł�.
	*    ^ ---|P		    �x�N�g���͕���������킷���̂Ȃ̂ŁA�ʒu�͊֌W����܂���.
	*     \   |         
	*     L\  |N        
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag4
 	*   ^    ^   
	*  N|  R/   /         R �� E (���_�x�N�g��) �̓��ς����߂܂�.
	*   |  /b /E          ����ƁA
	*   | / /  Cos b      Cos b �����܂�܂�.
	*   |/<               
	*---------------------
	*
	*Tag5               
	*     ���̌�A CosB^n * ks �ƂȂ�A�X�y�L�����[�̋��������܂�.
	*     n �́A���̂̏_�炩���ł�.  �������ƃX�y�L�����[�͈̔͂��L����A
	*     �傫����΁A�͈͂͋����Ȃ�܂�.
	*
	*/
	XYZFixed  theRevLight;					//�t���C�g�x�N�g��
	XYZFixed  theR;							//�x�N�g��R
	XYZFixed  theRevE;						//�t���_�x�N�g��
	//�J�����̈ʒu.
	XYZFixed* theEyePos = (TLim3DMaster::GetSelectEye())->GetPoint()->GetPos();
	//inPos �ɑ΂��邱�̃��C�g�̋t���C�g�x�N�g�������߂�.
    GetRevLightVector(&theRevLight,inPos);
	//inPos �ɑ΂��邱�̎��_�̋t���_�x�N�g�������߂܂�.
	VectorSub( &theRevE , theEyePos ,  inPos);
	Fixed theSpecular;
	{
		register Fixed	theCosA;
		// theRevLight � �@��  �� ���邳(-1�`1)�����܂�܂�.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
		if (theCosA < 0)
		{	//���ׂ� 0
	   		return 0;
		}
		//�x�N�g��CP
		XYZFixed theCP;	
		//theCosA * theRevLight�ɂ�� CP �����߂܂�.
		VectorMul( &theCP , &theRevLight , theCosA);
		//�x�N�g�� CP �� 2 �{���܂�.
		theCP.x *= 2;		theCP.y *= 2;		theCP.z *= 2;
		//�x�N�g�� theCP + theRevLight �ɂ��A�x�N�g��R (�x�N�g��G) �����߂܂�.
		VectorAdd( &theR , &theCP , &theRevLight);
		//theR , theRevE �̐��K��.
        Normalize(&theR);        Normalize(&theRevE);
		//�x�N�g�� theR �ƁA�t���_�x�N�g���ɂ��A theCosB (�ϐ���CosA (^^;) �����߂�.
        theCosA = DotProduct(&theR,&theRevE);
		if (theCosA < 0)
		{	//���ׂ� 0
	   		return 0;
		}
		//CosB ^ ���̂̓�炩��(0�`) * ���ʔ��ˌW��
		theSpecular = FixedMul(fxpow(theCosA,inMaterial->GetVelvetyReflexFactor() )
												, inMaterial->GetSpecularReflexFactor() );
	}
	if (theSpecular <= 0)
	{	//���ׂ� 0
   		return 0;
	}
	if (theSpecular >= IF(1) ) 
	{	//���̂܂�܂̐F
   		return m_RGB;
	}
	//���̃��C�g�ɂ��F�����.
	return MulColor(m_RGB,theSpecular);
}

//�A���r�G���g
DWORD TLim3DLight::Ambient(POLYGON inPoly , TLim3DMaterial* inMaterial)
{
	//���̃��C�g�ɂ��F�����.
	return MulColor(m_RGB, inMaterial->GetAmbientReflexFactor() );
}

//�����o�[
DWORD TLim3DLight::Lambert(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	XYZFixed		theRevLight;			//�t���C�g�x�N�g��
	//pos �ɑ΂��邱�̃��C�g�̋t���C�g�x�N�g�������߂�.
    GetRevLightVector(&theRevLight,inPos);
	Fixed theLambert;
	{
		register Fixed	theCosA;
		// revLight � HousenVector  �� ���邳(-1�`1)�����܂�܂�.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
        if (theCosA < 0)
        {	//���͂������Ă܂���.
    		return 0;
        }
		theLambert = FixedMul( theCosA, inMaterial->GetDiffuseReflexFactor() ) 
											+ inMaterial->GetAmbientReflexFactor();
	}
	if (theLambert <= 0)
	{	//���ׂ� 0
   		return 0;
	}
	if (theLambert >= IF(1) ) 
	{	//���̂܂�܂̐F
   		return m_RGB;
	}

	//���̃��C�g�ɂ��F�����.
	return MulColor(m_RGB,theLambert);
}

//�t�H��
DWORD TLim3DLight::Phong(POLYGON inPoly,
				XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	XYZFixed  theRevLight;					//�t���C�g�x�N�g��
	XYZFixed  theR;							//�x�N�g��R
	XYZFixed  theRevE;						//�t���_�x�N�g��
	//�J�����̈ʒu.
	XYZFixed* theEyePos = (TLim3DMaster::GetSelectEye())->GetPoint()->GetPos();
	//inPos �ɑ΂��邱�̃��C�g�̋t���C�g�x�N�g�������߂�.
    GetRevLightVector(&theRevLight,inPos);
	//inPos �ɑ΂��邱�̎��_�̋t���_�x�N�g�������߂܂�.
	VectorSub( &theRevE , theEyePos ,  inPos);
	Fixed thePhong = 0;
	while(1)
	{
		register Fixed	theCosA;
		// theRevLight � �@��  �� ���邳(-1�`1)�����܂�܂�.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );

		if (theCosA < 0)	break;
		thePhong = FixedMul( theCosA , inMaterial->GetDiffuseReflexFactor() )
											+ inMaterial->GetAmbientReflexFactor();
		//�x�N�g��CP
		XYZFixed theCP;	
		//theCosA * theRevLight�ɂ�� CP �����߂܂�.
		VectorMul( &theCP , &theRevLight , theCosA);
		//�x�N�g�� CP �� 2 �{���܂�.
		theCP.x *= 2;		theCP.y *= 2;		theCP.z *= 2;
		//�x�N�g�� theCP + theRevLight �ɂ��A�x�N�g��R (�x�N�g��G) �����߂܂�.
		VectorAdd( &theR , &theCP , &theRevLight);
		//theR , theRevE �̐��K��.
        Normalize(&theR);        Normalize(&theRevE);
		//�x�N�g�� theR �ƁA�t���_�x�N�g���ɂ��A theCosB (�ϐ���CosA (^^;) �����߂�.
        theCosA = DotProduct(&theR,&theRevE);
		if (theCosA < 0)	break;

		//CosB ^ ���̂̓�炩��(0�`) * ���ʔ��ˌW��
		thePhong = FixedMul(fxpow(theCosA,inMaterial->GetVelvetyReflexFactor() )
												, inMaterial->GetSpecularReflexFactor() );
		break;
	}
	if (thePhong <= 0)
	{	//���ׂ� 0
		return 0;
	}
	if (thePhong >= IF(1) ) 
	{	//���̂܂�܂̐F
		return m_RGB;
	}
	//���̃��C�g�ɂ��F�����.
	return MulColor(m_RGB,thePhong);
}


//���C�g�̉�]�p�x���w��.
void			TLim3DLight::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point.SetAngleX(x);
	m_Point.SetAngleY(y);
	m_Point.SetAngleZ(z);
}
//���C�g�̏ꏊ���w��.
void			TLim3DLight::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(x);
	m_Point.SetPosY(y);
	m_Point.SetPosZ(z);
}
//���C�g�̉�]�p�x���擾.
void			TLim3DLight::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point.GetAngleX();
	*y = m_Point.GetAngleY();
	*z = m_Point.GetAngleZ();
}
//���C�g�̏ꏊ���擾.
void			TLim3DLight::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point.GetPosX();
	*y = m_Point.GetPosY();
	*z = m_Point.GetPosZ();
}
//���C�g�̉�]�p�x�𑝂₷.
void			TLim3DLight::AddRotate(int x,int y,int z)
{
	m_Point.SetAngleX(m_Point.GetAngleX() + x);
	m_Point.SetAngleY(m_Point.GetAngleY() + y);
	m_Point.SetAngleZ(m_Point.GetAngleZ() + z);
}
//���C�g�̏ꏊ���ړ�����.
void			TLim3DLight::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(m_Point.GetPosX() + x);
	m_Point.SetPosY(m_Point.GetPosY() + y);
	m_Point.SetPosZ(m_Point.GetPosZ() + z);
}

//�ꏊ���N���X�ւ̃x�^�A�N�Z�X
TLim3DPoint* TLim3DLight::GetPoint()
{
	return &m_Point;
}

//�t�����x�N�g�������߂�.
void TLim3DLight::GetRevLightVector(XYZFixed* outRevLightVector , XYZFixed const *inPos)
{
	//���C�g�̈ʒu��茻�ݒl�������āA�t���_�x�N�g�������߂܂�.
	VectorSub(outRevLightVector, GetPoint()->GetPos() , inPos);
	//����𐳋K�����܂�.
	Normalize(outRevLightVector);
}

//���̃��C�g�ɂ��F�����.
DWORD TLim3DLight::MulColor(DWORD inOrignalRGB,Fixed inParsent)
{
	int theR,theG,theB;

	theR = (inOrignalRGB & 0x00ff0000) >> 16;
	theG = (inOrignalRGB & 0x0000ff00) >>  8;
	theB = (inOrignalRGB & 0x000000ff)      ;

	theR = FI(theR*inParsent);
	theG = FI(theG*inParsent);
	theB = FI(theB*inParsent);

	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	return (theR << 16) + (theG << 8) + (theB);
}


