#ifndef	___LIM3D_MASTER__H	//�d���h�~
#define ___LIM3D_MASTER__H

#include <./rti_define.h>

#include  <./Lim3D/rti_Lim3DEyeCaretaker.h>		//���_�Ǘ�
#include  <./Lim3D/rti_Lim3DModelCaretaker.h>	//���f���Ǘ�
#include  <./Lim3D/rti_Lim3DLightCaretaker.h>	//�����Ǘ�
#include  <./Lim3D/rti_Lim3DMaterialCaretaker.h>//�}�e���A���Ǘ�.
#include  <./Graphics/rti_zbuffer.h>			//ZBUffer
#include  <./Lim3D/rti_Lim3DZSortWithZBuffer.h>	//�|���S���� Z �\�[�g.
#include  <./Base/rti_NameServer.h>				//3D�G���W���̈ꎞ�l�[���e�[�u��.
#include  <./Graphics/rti_bitmaplib.h>			//�r�b�g�}�b�v���C�u����.
#include  <./Lim3D/rti_Lim3DAnime.h>			//�A�j���[�V����.


#define LIM3D_USING_AREAW	320
#define LIM3D_USING_AREAH	240

//�}�X�^�[3D�N���X.
class TLim3DMaster
{
public:
	static void Init()
	{
		//ZBuffer�m��.
		m_ZBuffer.Alloc(LIM3D_USING_AREAW , LIM3D_USING_AREAH);
	}
	static void End()
	{
		//�e��Ǘ��N���X
		m_EyeCaretaker.Clear();
		m_ModelCaretaker.Clear();
		m_LightCaretaker.Clear();
		m_MaterialCaretaker.Clear();
		//�e�N�X�`���[
		m_TextureCaretaker.Clear();
		//�l�[���T�[�o�[
		NameServer.Release();
		//ZBuffer
		m_ZBuffer.Destroy();
		//�|���S���� ZBuffer
		m_ZSortBuffer.Clear();
		//�A�j���[�V�����Ǘ�.
		m_AnimeControl.Clear();
	}
	//�I������Ă��鎋�_�𓾂�.
	static TLim3DEye*	GetSelectEye(){	return m_SelectEye;	}
	//���_��I������.
	static void			SetSelectEye(TLim3DEye* SelectEye){	m_SelectEye = SelectEye;}
	//ZBuffer �擾.
	static TZBuffer*	GetZBuffer(){	return &m_ZBuffer;}
	//�G��̈�擾.
	static BMD			GetOutBuffer(){	return m_SelectBMD;	}
	//�G��̈�ݒ�.
	static void			SetOutBuffer(BMD SelectBMD){	m_SelectBMD = SelectBMD;	}
	//�}�e���A���ɃA�N�Z�X.
	static TLim3DMaterial* GetMaterial(int inNo){return m_MaterialCaretaker.At(inNo);}

	//�g�b�v���f���̍쐬.
	static TLim3DModel*	NewModel(){ return m_ModelCaretaker.New(); }
	//���_�̍쐬.
	static TLim3DEye*	NewEye(){ return m_EyeCaretaker.New(); }
	//���C�g���f���̍쐬.
	static TLim3DLight*	NewLight(){ return m_LightCaretaker.New(); }
	//�}�e���A���̍쐬.
	static TLim3DMaterial*	NewMaterial(){ return m_MaterialCaretaker.New(); }
	//�e�N�X�`���[�̍쐬.
	static BMD	NewTexture(const char* inTextureName){ return m_TextureCaretaker.Make(inTextureName); }
	//�A�j���[�V�����^�X�N.
	static TLim3DAnimeTask*	NewAnimeTask
			(const char* inSeenTag){return m_AnimeControl.TaskCreate(inSeenTag);}	
	//�A�j���[�V������ǂݍ���
	static bool	NewAnimeData
			(const char* inAnimeFile){return m_AnimeControl.Read(inAnimeFile);}	

	//���J�n.
	static void Cls()
	{
		//ZBuffer ���N���A.
		m_ZBuffer.Clear();
		//�|���S���� ZBuffer
		m_ZSortBuffer.Clear();
		//���_��]�}�g���b�N�X�̍쐬.
		m_SelectEye->MakeMatrix();
	}
	//���I��.
	static void Ref()
	{
		//�|���S���� ZBuffer �̃\�[�g
		m_ZSortBuffer.Sort();
		//�|���S���� ZBuffer �̊G��
		m_ZSortBuffer.AllDraw();
	}

	//���_�Ǘ��N���X�Ƀx�^�A�N�Z�X.
	static TLim3DEyeCaretaker* GetEyeCaretaker(){return &m_EyeCaretaker;}
	//���C�g�Ǘ��N���X�Ƀx�^�A�N�Z�X.
	static TLim3DLightCaretaker* GetLightCaretaker(){return &m_LightCaretaker;}
	//���f���Ǘ��N���X�ɂׂ��A�N�Z�X.
	static TLim3DModelCaretaker* GetModelCaretaker(){return &m_ModelCaretaker;}
	//�}�e���A���Ǘ��N���X�ɂׂ��A�N�Z�X.
	static TLim3DMaterialCaretaker* GetMaterialCaretaker(){return &m_MaterialCaretaker;}
	//�|���S���� Z �\�[�g�֒ǉ�.
	static void AddZSortPolygon(TLim3DPolygon* inPolygon)
									{m_ZSortBuffer.Add( inPolygon ) ;}

	static TStringHashTable			NameServer;
protected:
	//�e��Ǘ��N���X
	static TLim3DEyeCaretaker		m_EyeCaretaker;
	static TLim3DModelCaretaker		m_ModelCaretaker;
	static TLim3DLightCaretaker		m_LightCaretaker;
	static TLim3DMaterialCaretaker	m_MaterialCaretaker;
	static TLim3DAnimeControl		m_AnimeControl;
	
	//�e�N�X�`���[.
	static TBmpChar				m_TextureCaretaker;
	//ZBuffer
	static TZBuffer				m_ZBuffer;
	//�I������Ă��鎋�_.
	static TLim3DEye*			m_SelectEye;
	//�G�悷���ʗ̈�.
	static BMD					m_SelectBMD;
	//�|���S���� Z�\�[�g
	static TLim3DZSortWithZBuffer	m_ZSortBuffer;
private:
};


#endif							//�d���h�~
