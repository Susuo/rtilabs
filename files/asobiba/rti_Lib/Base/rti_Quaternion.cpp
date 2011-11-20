
/************************************************************************/
//�N�E�H�[�^�j�I��
/************************************************************************/
/*
#include <.\Base\rti_Quaternion.h>



//��]�s����쐬.
void QuaternionMatrix(MATRIX3D Ma, Quaternion const *lpQuaternion)
{
	
	//lpQuaternion �́A���K������Ă��邱��.
	Fixed x,y,z,w;
	Fixed xx,yy,zz,xy,xz,wx,wy,wz,yz;
	x = lpQuaternion->v.x;	y = lpQuaternion->v.y;	z = lpQuaternion->v.z;	w = lpQuaternion->w;
	xx = FixedMul(x,x);
	yy = FixedMul(y,y);
	zz = FixedMul(z,z);
	xy = FixedMul(x,y);
	xz = FixedMul(x,z);
	wx = FixedMul(w,x);
	wy = FixedMul(w,y);
	wz = FixedMul(w,z);
	yz = FixedMul(y,z);
	Ma[0] = IF(1) - (yy+zz) * 2;
	Ma[1] = (xy-wz) * 2;
	Ma[2] = (xz+wy) * 2;

	Ma[3] = (xy+wz) * 2;
	Ma[4] = IF(1) - (xx+zz) * 2;
	Ma[5] = (yz-wx) * 2;

	Ma[6] = (xz-wy) * 2;
	Ma[7] = (yz+wx) * 2;
	Ma[8] = IF(1) - (xx+yy) * 2;
	
}

//�����Z. A+B
void QuaternionAdd(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	// q1 + q2 = (a1+a2 , V1+V2)
	//�X�J���[��.
	lpQuaternion->w = lpQuaternionA->w + lpQuaternionB->w;
	//�x�N�g����.
	VectorAdd( &lpQuaternion->v , &lpQuaternionA->v , &lpQuaternionB->v );
}
//�����Z. A-B
void QuaternionSub(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	//q1 - q2 = (a1 - a2 , V1 - V2)
	//�X�J���[��.
	lpQuaternion->w = lpQuaternionA->w - lpQuaternionB->w;
	//�x�N�g����.
	VectorSub( &lpQuaternion->v , &lpQuaternionA->v , &lpQuaternionB->v );
}

//�|���Z A * B
void QuaternionMul(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	//q1q2 = (a1a2 - V1.V2   , a1V2 + a2V1 + V1*V2)
	//�Ƃ肠�����A�����Ƀx�N�g�������v�Z.
	XYZFixed Va,Vb,VCross;
	VectorMul(&Va, &lpQuaternionB->v , lpQuaternionA->w);
	VectorMul(&Vb, &lpQuaternionA->v , lpQuaternionB->w);
	CrossProduct ( &VCross , &lpQuaternionA->v , &lpQuaternionB->v);
	VectorAdd(&Va ,&Va , &Vb);			//����
	VectorAdd(&Va ,&Va , &VCross);		//Va �́A�x�N�g����.
	//�X�J���[���̌v�Z.
	lpQuaternion->w = FixedMul(lpQuaternionA->w , lpQuaternionB->w)
							- DotProduct(&lpQuaternionA->v,&lpQuaternionB->v);
	//�x�N�g�����̑��
	lpQuaternion->v = Va;
}
//����Z A / B
void QuaternionDiv(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{
	//����Z q1 / q2 = q1 q2^-1
	Quaternion Q = *lpQuaternionB;	//B �͉󂵂Ă͂����Ȃ�.
	RevQuaternion(&Q);				//�t�N�E�H�[�^�j�I���ɕϊ�.

	QuaternionMul(lpQuaternion , lpQuaternionA , &Q);
}
//����
void Conjugate(Quaternion *lpQuaternion)
{	//�x�N�g���������������] 'q = (a , -V)
	lpQuaternion->v.x = -lpQuaternion->v.x;	lpQuaternion->v.y = -lpQuaternion->v.y;	lpQuaternion->v.z = -lpQuaternion->v.z;
}
//�m����
Fixed Norm(Quaternion const *lpQuaternion)
{	//norm(q) = |q| = (a^2 + V.V)
	return FixedMul(lpQuaternion->w , lpQuaternion->w) 
		+ DotProduct(& (lpQuaternion->v) , &(lpQuaternion->v) );
}
//��Βl.
Fixed Abs(Quaternion const *lpQuaternion)
{	// ||q|| = abs(q) = sqrt(a^2+V.V)
//	return sqrt( Norm(lpQuaternion) );
	return Float2Fixed( sqrt( Fixed2Float(Norm(lpQuaternion)) ) );
}
//���K��
void Normalize(Quaternion *lpQuaternion)
{	//normalize(q) = q / ||q||
	Fixed abs = Abs(lpQuaternion);
	lpQuaternion->w = FixedDiv(lpQuaternion->w , abs);
	VectorDiv( &lpQuaternion->v , &lpQuaternion->v , abs);
}	
//�t�N�E�H�[�^�j�I��
void RevQuaternion(Quaternion *lpQuaternion)
{	// q^-1 = q' / |q| = ( a/(a^2+V.V) , -V/(a^2+V.V) )
	Fixed norm = Norm(lpQuaternion);//�m�����쐬.
	Conjugate(lpQuaternion);		//�����ɕϊ�.

	FixedDiv(lpQuaternion->w , norm);
	VectorDiv( &lpQuaternion->v , &lpQuaternion->v , norm);
}

//�w�肵�� jiku �ɂ��A Angle(�f�B�O���[)�������Ƃ��̃N�E�H�[�^�j�I�������߂�.
void QuaternionFromRotation(Quaternion *lpQuaternion,XYZFixed  const *jiku,Angle256 Angle)
{
	Angle /= 2;					//���̉�]�� 2����]���܂�.	������A���̔��������܂�.
	Fixed SinHarf = FixedLTBSin256[Angle];
	//q(��) = ( cos(��/2) , Vsin(��/2) )
	lpQuaternion->w = FixedLTBCos256[Angle];
	VectorMul( &lpQuaternion->v , jiku , SinHarf);

}

//���ʐ��`�⊮
void QuaternionSlerp(Quaternion *lpQuaternion,
	 Quaternion const *lpQuaternionStart,Quaternion const *lpQuaternionEnd, Fixed alpha)
{

}

//�P�ʃN�E�H�[�^�j�I��
void QuaternionUnit(Quaternion *lpQuaternion)
{
	lpQuaternion->w = IF(1);
	lpQuaternion->v.x = 0;
	lpQuaternion->v.y = 0;
	lpQuaternion->v.z = 0;
}


*/
