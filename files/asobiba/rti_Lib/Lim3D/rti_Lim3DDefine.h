#ifndef	___LIM3D_DEFINE__H	//�d���h�~
#define ___LIM3D_DEFINE__H

#include <./rti_define.h>

//���_�f�[�^
struct VecWorldEye
{
	XYZFixed	Vec;	//���_�I�Ȉʒu
	XYZFixed	World;	//���[���h�ł̈ʒu
	XYZFixed	Eye;	//���_�ł̈ʒu
};

//�|���S���������Ă���f�[�^
struct PolygonData
{
	int		Link;	//���_�̏ꏊ
	XY		Texture;//�e�N�X�`��
};

//�|���S���������Ă���f�[�^
struct PolygonDataCoexist
{
	void*	Model;	//���̒��_�������Ă��郂�f��
	int		Link;	//���_�̏ꏊ
	XY		Texture;//�e�N�X�`��
};

//�|���S���������Ă���f�[�^
struct PolygonDataEx
{
	VecWorldEye*	LinkVecPointer;	//���_�̏ꏊ�̃|�C���^.
	XY				Texture;//�e�N�X�`��
};

//���ۂɊG�悷��Ƃ��Ɏg���f�[�^
struct DrawUse
{
	XYZFixed	Pos;	//���_���W�̍ŏI�`��.
	XYZFixed	Display;//�G�悷��ʒu.
	XY			Texture;//�e�N�X�`�����W.
};

#endif			//�d���h�~

