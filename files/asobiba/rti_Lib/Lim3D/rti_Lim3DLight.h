#ifndef	___LIM3D_LIGHT__H	//�d���h�~
#define ___LIM3D_LIGHT__H

#include <./Lim3D/rti_Lim3DPoint.h>
#include <./Lim3D/rti_Matrix.h>
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./Lim3D/rti_Lim3DMaterial.h>
#include <./Lim3D/rti_SuperListClient.h>

//���C�g��{�^
class TLim3DLight : public __TSuperListClient
{
public:
	//======= ���\�b�h ===========
	//���C�e�B���O
	DWORD Lighting(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//�����̂�����(�_�݂ɂ��炸)��ݒ�
	void SetLightColor(DWORD c) { m_RGB = c ;} 
	//�f�B�t���[�Y
	DWORD Diffuse(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//�X�y�L�����[
	DWORD Specular(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//�A���r�G���g
	DWORD Ambient(POLYGON inPoly , TLim3DMaterial* inMaterial);
	//�����o�[
	DWORD Lambert(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//�t�H��
	DWORD Phong(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
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
protected:
	//���̃��C�g�ɂ��F�����.
	DWORD MulColor(DWORD inOrignalRGB,Fixed inParsent);

	TLim3DPoint				m_Point;			//�ʒu
	DWORD					m_RGB;				//�F.
	
	//�t�����x�N�g�������߂�.
	void GetRevLightVector(XYZFixed* outRevLightVector , XYZFixed const *inPos);
private:
};

//���f����`
typedef TLim3DLight* LIGHT;


#endif				//�d���h�~
