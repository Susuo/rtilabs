#ifndef	___LIM3D_MODEL__H	//�d���h�~
#define ___LIM3D_MODEL__H

#include <windows.h>

#include <./rti_define.h>
#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Matrix.h>

class	TLim3DVecCaretaker;
class	TLim3DPolygonCaretaker;
class	TLim3DMake;
class	TLim3DPoint;
class	TLim3DPolygon;

class TLim3DModel
{
public:
	/*****************/
	//�N���X�ێ��̂��߂̃��\�b�h
	/*****************/
	//�R���X�g���N�^
	TLim3DModel();
	//�f�X�g���N�^
	~TLim3DModel();
	//�K�y�[�W�R���N�V�������폜.
	bool	DeleteCaretation(TLim3DModel* Model);
	//�e���f����������.
	bool Mount(TLim3DModel * Model)
	{
		if (m_ParenceModel != NULL)	return false;	//��q������.
		m_ParenceModel = Model;
		return true;
	}

	/*****************/
	//���[�U�[�̂��߂̍����x�����\�b�h.
	/*****************/
	//���̃��f���ȉ��̃��f���̊G��.
	void			Drawing();
	//���̃��f���ȉ��̃��f���̊G��.
	void			Drawing(TMatrix * Ma);
	//�j��
	bool			Release();
	//���_���̐ݒ�.
	bool			SetVec(XYZFixed* _c ,	int Many , bool SizeNotChange = true);
	//���_���̒ǉ�.
	bool			AddVec(XYZFixed* _c ,	int Many , bool SizeNotChange = true);
	//���ׂĂ̒��_�̔j��
	void			ClearVec();
	//�|���S�����̐ݒ�.
	TLim3DPolygon*	NewPolygon();
	//���f���ɑ��݂��邷�ׂẴ|���S���̔j��.
	void			ClearPolygon();
	//�|���S�������J�n
	TLim3DPolygon* 	FindFirstPolygon();
	//�|���S���������s.
	TLim3DPolygon* 	FindNextPolygon();
	//���f���̉�]�p�x���w��.
	void			SetRotate(Angle x,Angle y,Angle z);
	//���f���̏ꏊ���w��.
	void			SetPos(Fixed x,Fixed y,Fixed z);
	//���f���̉�]�p�x���擾.
	void			GetRotate(Angle *x,Angle *y,Angle *z);
	//���f���̏ꏊ���擾.
	void			GetPos(Fixed *x,Fixed *y,Fixed *z);
	//���f���̉�]�p�x�𑝂₷.
	void			AddRotate(int x,int y,int z);
	//���f���̏ꏊ���ړ�����.
	void			AddPos(Fixed x,Fixed y,Fixed z);
	//�q���f�����쐬����.
	TLim3DModel*	NewModel();
	//���ׂĂ̎q���f�����폜
	void			ClearModel();
	//���f�������J�n
	TLim3DModel*	FindFirstModel();
	//���f���������s.
	TLim3DModel*	FindNextModel();
	//�d�����_�̐؂藣��(����댯����ˁ[)
	void			Optimize();
	//�w�肵�����f���̃R�s�[�Ɏ������Ȃ�.
	void			CopyForm(TLim3DModel * inModel);

	/*****************/
	//���[�U�[�̂��߂̒჌�x�����\�b�h.
	/*****************/
	//�v�Z�̂�(���_�v�Z)
	void					ClasOnly();
	//�v�Z�̂�(���_�v�Z)
	void					ClasOnly(TMatrix * inParenceMatrix);
	//�G��̂�(���_�v�Z�����łɂ���Ă��邱�Ƃ�����)
	void					DrawOnly();
	//�d�����钸�_�̍폜.
	void					Smart();
	//�e���f���擾.
	TLim3DModel*			GetParenceModel();
	//���_���N���X�ւ̃x�^�A�N�Z�X
	TLim3DVecCaretaker*		GetVecCaretaker();
	//�|���S�����N���X�ւ̃x�^�A�N�Z�X
	TLim3DPolygonCaretaker*	GetPolygonCaretaker();
	//�e�q���N���X�ւ̃x�^�A�N�Z�X
	TLim3DMake*				GetMake();
	//�ꏊ���N���X�ւ̃x�^�A�N�Z�X
	TLim3DPoint*			GetPoint();

	/*****************/
	//���[�U�[�̂��߂̒�^2 ���x�����\�b�h. (�댯!!)
	/*****************/
	//�|���S���̎w�肳�ꂽ�Ȃ���f�[�^(inTarget)��V�����f�[�^(inNew)�ɕύX���܂�.
	void					OptimizePolygonLinkChange
								(VecWorldEye* inTarget ,VecWorldEye* inNew);
	//���_�f�[�^�Ǘ��N���X�ŁA���_�f�[�^�̍ăA���P�[�V���������s���ꂽ.
	void					ReAllocMemoryVec(VecWorldEye* OldMemoryAddress ,
								VecWorldEye* NewMemoryAddress ,  int OldUseSize);

protected:

	TLim3DModel*	m_ParenceModel;			//�e���f��.
	bool			m_Breaking;				//���������󂵂Ă���Ȃ� true

	TLim3DVecCaretaker*		m_Vec;			//���_���
	TLim3DPolygonCaretaker*	m_Polygon;		//�|���S�����
	TLim3DMake*				m_Make;			//�e�q�֌W
	TLim3DPoint*			m_Point;		//�ʒu

private:
};

//���f����`
typedef TLim3DModel* MODEL;

#include <./Lim3D/rti_Lim3DVecCaretaker.h>
#include <./Lim3D/rti_Lim3DPolygonCaretaker.h>
#include <./Lim3D/rti_Lim3DMake.h>
#include <./Lim3D/rti_Lim3DPoint.h>

#endif				//�d���h�~