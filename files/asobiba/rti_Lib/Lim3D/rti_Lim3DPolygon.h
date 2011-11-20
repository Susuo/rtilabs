#ifndef	___LIM3D_POLYGON__H	//�d���h�~
#define ___LIM3D_POLYGON__H

#include <windows.h>
#include <./Lim3D/rti_Lim3DLinkData.h>
#include <./Lim3D/rti_Lim3DPolygonClipping.h>
#include <./Lim3D/rti_Lim3DPolygonDraw.h>
#include <./Lim3D/rti_SuperListClient.h>


//�V�F�[�f�B���O����
enum ShadingLevel
{
	NONE_SHADING ,	//�V�F�[�f�B���O�Ȃ�.
	FLAT_SHADING ,	//�t���b�g�V�F�[�f�B���O
	GROW_SHADING ,	//�O���[�V�F�[�f�B���O
};

class TLim3DModel;
class TLim3DLinkData;

class TLim3DPolygon : public __TSuperListClient
{
public:
	//�R���X�g���N�^
	TLim3DPolygon()
	{
		m_Shading		= NONE_SHADING;						//�V�F�[�f�B���O
		m_Material		= NULL;
	}
	/*****************/
	//���[�U�[�̂��߂̍����x�����\�b�h.
	/*****************/
	//�G��
	void		Draw();
	//�Ȃ�����Z�b�g.
	bool		SetLink(PolygonData *_c , int Many , bool SizeNotChange = true);
	//�Ȃ�����Z�b�g.
	bool		AddLink(PolygonData *_c , int Many , bool SizeNotChange = true);
	//�Ȃ�����Z�b�g.
	bool		SetLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange = true);
	//�Ȃ�����Z�b�g.
	bool		AddLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange = true);
	//�Ȃ���N���A
	void		ClearLink();
	//�e�N�X�`���[�̍ă}�b�s���O(AUTO).
	bool		ReMapTexture();
	//�e�N�X�`���[�̍ă}�b�s���O.
	bool		ReMapTexture(int X1 ,int Y1,
						int X2,int Y2, int X3,int Y3, int X4,int Y4);
	//�@���x�N�g���̎擾.
	XYZFixed*	GetHousenVector(){return &m_HousenVector;}
	//�d�S�̎擾.
	XYZFixed*	GetAvg(){return &m_AvgPoint;}
	//���_���W�ł̏d�S�� Z 
	Fixed	GetEyeAvgZ(){return m_Eye_Avg_Z;}
	/*****************/
	//���[�U�[�̂��߂̒჌�x�����\�b�h.
	/*****************/
	//�Ȃ���f�[�^���x�^�Ŏ擾.
	TLim3DLinkData*	GetLinkData();
	//�e�̃��f���擾.
	TLim3DModel* GetParenceModel();


	//���ۊG�敔��
	void TrueDraw();

	/*****************/
	//�V�F�[�f�B���O�֌W
	/*****************/
	//�V�F�[�f�B���O
	void			SetShading(ShadingLevel c){ m_Shading = c;}
	ShadingLevel	GetShading(){ return m_Shading ;}
	//�}�e���A���֌W.
	void			SetMaterial(TLim3DMaterial* c){ m_Material = c;}
	TLim3DMaterial*	GetMaterial(){ return m_Material ;}


protected:
	ShadingLevel		m_Shading;					//�V�F�[�f�B���O
	TLim3DMaterial*		m_Material;					//�}�e���A��.

	//�e���f��.
	//TLim3DModel*	m_ParenceModel;
	//�Ȃ���f�[�^.
	TLim3DLinkData	m_LinkData;
	//�N���b�s���O
	TLim3DPolygonClipping m_PolygonClipping;
	//�G��
	TLim3DPolygonDraw	m_PolygonDraw;

	//�@���x�N�g���̎Z�o.
	void ClasHousenVector(PolygonDataEx* inUsing);
	//���ʂ��ǂ������f. true = ����
	bool IsBackstairs(PolygonDataEx* inUsing);
	//�Ȃ�����̓o�^.
	bool LinkEntry(PolygonData *_c , int Many , bool SizeNotChange , bool IsSet);
	//�Ȃ�����̓o�^.
	bool LinkEntry(PolygonDataCoexist *_c , int Many , bool SizeNotChange , bool IsSet);

	XYZFixed m_AvgPoint;						//���̃|���S���̏d�S.(world)
	Fixed m_Eye_Avg_Z;							//���̃|���S���̎��_���W�ł̏d�S��Z.
	XYZFixed m_HousenVector;					//���̃|���S���̖@���x�N�g��(world/�P��)

private:
};

//���f����`
typedef TLim3DPolygon* POLYGON;


#include <./Lim3D/rti_Lim3DPolygonCaretaker.h>


#endif				//�d���h�~