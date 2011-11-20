#include <./Lim3D/rti_Lim3DMaster.h>

class TReadMetasequoia
{
public:
	//�R���X�g���N�^
	TReadMetasequoia();
	//�f�X�g���N�^
	~TReadMetasequoia();

public:
	//�t�@�C���̓ǂݍ���.
	bool Open(const char *inFileName);
	//�N���[�Y.
	bool Close();
	//�I�u�W�F�N�g��ǂݍ���.
	bool GetModel(const char* inOnjectName , MODEL outModel);
	//���ׂẴ��f���̎擾.
	bool GetAllModel();

	//������.
	void Init();

private:
	//�w�b�_�`�F�b�N.
	bool CheckHeader();
	//�}�e���A���擾.
	bool GetMaterial();
	//�}�e���A���擾.
	bool GetMaterialCaseByCase(const char* inMaterialData , MATERIAL* outMaterial);
	//�J���[���擾.
	DWORD ClasColor( float inR , float inG , float inB , float inEmi);
	//�e�N�X�`���[�}�b�s���O������ ? ���ꍇ�́A���� �e�N�X�`���[�� BMD��Ԃ�.
	BMD ClasTexture(const char * inTextureName);
	//���C�e�B���O���ʂ̐ݒ�.
	LightLevel ClasLighting(float inDif ,float inAmb ,float inSpc ,float inPow);

	//���f���̏ڍׂȏ����擾���܂�.
	void GetModelDetail(const char* inObjectData , MODEL outModel);
	//�V�F�[�f�B���O���̎擾.
	void GetModelDetailShading(const char* inTagData , MODEL outModel);
	//���_���̎擾.
	void GetModelDetailVertex(const char* inTagData , MODEL outModel);
	//���̎��̍s�ɂ͒��_��񂪂���͂��Ȃ̂ŁA�ǂݍ��݂܂�.
	void GetVertexCaseByCase(const char* inVertexData , XYZFixed * outVertex);
	//�ʏ��̎擾.
	void GetModelDetailFace(const char* inTagData , MODEL outModel);
	//�ʏ��̎擾.
	bool GetFaceCaseByCase(const char* inFaceData , MODEL outModel);
	//���̂Ȃ���f�[�^���A�|���S�����쐬����.
	void GetFaceCaseByCasePolygonCreate
						(MODEL outModel , int inNum , 
						 const int* inLink , int inMaterial , const float* inUV);



private:
	//�}�e���A��.
	MATERIAL*	m_Material;
	//�}�e���A���̐�.
	int			m_MaterialNum;
	//�t�@�C������ǂݍ��񂾃f�[�^.
	char*		m_FileData;
	//�f�[�^�̒���.
	int			m_FileDataLen;
};
