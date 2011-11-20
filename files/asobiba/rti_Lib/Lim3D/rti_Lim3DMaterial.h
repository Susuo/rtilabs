#ifndef	___LIM3D_MATERIAL__H	//�d���h�~
#define ___LIM3D_MATERIAL__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_SuperListClient.h>

//���̂̃��C�g�̌���.
enum LightLevel
{
	NONE_LIGHTING,		//���C�e�B���O�Ȃ�
	DEFUSE_LIGHTING,	//�f�B�t���[�Y
	SPECULAR_LIGHTING,	//�X�y�L�����[
	AMBIENT_LIGHTING,	//�A���r�G���g
	LAMBERT_LIGHTING,	//�����o�[ (�f�B�t���[�Y + �A���r�G���g)
	PHONG_LIGHTING,		//�t�H��   (�f�B�t���[�Y + �A���r�G���g + �X�y�L�����[)
};

//�h��Ԃ����@.
enum DrawingLevel
{
	BETA_DRAW		 ,	//�ׂ��h��
	TEXTURE_DRAW	 ,	//�e�N�X�`���[.
};


class TLim3DMaterial : public __TSuperListClient
{
public:
	TLim3DMaterial()
	{
		m_Drawing		= BETA_DRAW;						//�h�肩��.
		m_Lighting		= NONE_LIGHTING;					//���C�g����.
	}

	/*****************/
	//�V�F�[�f�B���O�֌W
	/*****************/
	//�h��Ԃ����@.
	void			SetDrawing(DrawingLevel c){ m_Drawing = c;}
	DrawingLevel	GetDrawing(){ return m_Drawing ;}
	//�F
	void			SetColor(DWORD c){ m_Color = c;}
	DWORD			GetColor(){ return m_Color ;}
	//�e�N�X�`���[
	void			SetTexture(BMD c){ m_Texter = c;}
	BMD				GetTexture(){ return m_Texter ;}

	/*****************/
	//���C�g�֌W
	/*****************/
	//���C�g����
	void			SetLighting(LightLevel l){ m_Lighting = l;};
	LightLevel		GetLighting(){return m_Lighting;};
	//�g�U���ˌW�� (0�`1)
	void			SetDiffuseReflexFactor(Fixed n){ m_DiffuseReflexFactor = n;};	
	Fixed			GetDiffuseReflexFactor(){return m_DiffuseReflexFactor;};
	//���ʔ��ˌW�� (0�`1)
	void			SetSpecularReflexFactor(Fixed n){ m_SpecularReflexFactor = n;};
	Fixed			GetSpecularReflexFactor(){return m_SpecularReflexFactor;};
	//���ʔ��˂̋��x�W��(���炩�� 0 �`)
	void			SetVelvetyReflexFactor(Fixed n){ m_VelvetyReflexFactor = n;};	
	Fixed			GetVelvetyReflexFactor(){return m_VelvetyReflexFactor;};
	//�����ˌW�� (0�`1)
	void			SetAmbientReflexFactor(Fixed n){ m_AmbientReflexFactor = n;};	
	Fixed			GetAmbientReflexFactor(){return m_AmbientReflexFactor;};


protected:
	DrawingLevel m_Drawing;						//�h�肩��.
	DWORD	m_Color;						//�|���S���̐F.
    BMD m_Texter;						//�e�N�X�`���[�}�b�s���O

	LightLevel m_Lighting;						//���C�g����.
	Fixed m_DiffuseReflexFactor;			    //�g�U���ˌW�� (0�`1)
	Fixed m_SpecularReflexFactor;	            //���ʔ��ˌW�� (0�`1)
	Fixed m_VelvetyReflexFactor;			    //���ʔ��˂̋��x�W��(���炩�� 0 �`)
	Fixed m_AmbientReflexFactor;	            //�����ˌW�� (0�`1)
private:
};

typedef class TLim3DMaterial*	MATERIAL;

#endif				//�d���h�~