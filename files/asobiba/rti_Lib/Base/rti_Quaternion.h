#ifndef ___QUATERNIONH
#define ___QUATERNIONH

/************************************************************************/
//�N�E�H�[�^�j�I��
/************************************************************************/
/*
#include <.\Base\rti_glookup.h>
#include <.\rti_define.h>
#include <.\Base\rti_vector.h>

struct Quaternion
{
	Fixed w;
	XYZFixed v;
};


//��]�s����쐬.
void QuaternionMatrix(MATRIX3D Ma, Quaternion const *lpQuaternion);
//�����Z. qA+qB
void QuaternionAdd(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//�����Z. qA-qB
void QuaternionSub(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//�|���Z qA * qB
void QuaternionMul(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//����Z qA / qB
void QuaternionDiv(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//����
void Conjugate(Quaternion *lpQuaternion);
//�m����
Fixed Norm(Quaternion const *lpQuaternion);
//��Βl.
Fixed Abs(Quaternion const *lpQuaternion);
//���K��
void Normalize(Quaternion *lpQuaternion);
//�t�N�E�H�[�^�j�I��
void RevQuaternion(Quaternion *lpQuaternion);
//�w�肵�� jiku �ɂ��A Angle(�f�B�O���[)�������Ƃ��̃N�E�H�[�^�j�I�������߂�.
void QuaternionFromRotation(Quaternion *lpQuaternion,XYZFixed  const *jiku,Angle256 Angle);
//���ʐ��`�⊮
void QuaternionSlerp(Quaternion *lpQuaternion,
	 Quaternion const *lpQuaternionStart,Quaternion const *lpQuaternionEnd, Fixed alpha);
//�P�ʃN�E�H�[�^�j�I��
void QuaternionUnit(Quaternion *lpQuaternion);

//�f�o�b�N
#define DEBUG_QUATERNION(q) DEBUG3(#q "	%lf	,	%lf	,	%lf	,	%lf",\
						Fixed2Float(q.w),Fixed2Float(q.v.x),Fixed2Float(q.v.y),Fixed2Float(q.v.z) )
*/
#endif
