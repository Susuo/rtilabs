#include <./Lim3D/rti_Lim3DMaster.h>

#define __ROKU_FACE_LINE_MAX	4	//�ʂ��\�z������̍ő�l.


class TReadRoku
{
	struct TRokuVec
	{
		int		No;				//  �_�ԍ�.
		int		GroupNum;		//�@�O���[�v���������̃O���[�v�ԍ��i�O���[�v�͊K�w���s�j
		int		IsHide;			//�@�O�Ȃ�\���B�P�Ȃ�B���B
		int		SymmetricalNum;	//�@�O�Ȃ�Q�����̓_�B�O�ȊO�Ȃ獶�E�Ώ̂ȓ_�̔ԍ��B
		Fixed	x,y,z;			//	���W
	};

	struct TRokuLine
	{
		int		VecNum[2];		//�@�Ȃ���_�i����1,2�j�̔ԍ�
		int		GroupNum;		//�@�O���[�v���������̃O���[�v�ԍ��i�O���[�v�͊K�w���s�j
		int		SymmetricalNum;	//�@�O�Ȃ�Q�����̓_�B�O�ȊO�Ȃ獶�E�Ώ̂ȓ_�̔ԍ��B
	};

	struct TRokuFace
	{
		int		FaceBlock;		//�@�ʂ̉搔
		int		ColorNum;		//�@�F�̔ԍ�
		int		No;				//  �ʔԍ�.
		int		LineNum[__ROKU_FACE_LINE_MAX];		//  �ʔԍ�
	};
public:
	//�R���X�g���N�^
	TReadRoku();
	//�f�X�g���N�^
	~TReadRoku();

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
	bool	CheckHeader(char ** ioReadPointer);
	//���̍s��.
	char*	NextLine(char * inLine);
	//�w�肵�����A���̍s�ɐi��.
	char*	NextLine(char * inLine , int inSkipLines);
	//�_���擾����.
	bool	GetPointes(char ** ioReadPointer);
	//�����擾����.
	bool	GetLines(char ** ioReadPointer);
	//�ʂ��擾����.
	bool	GetFaces(char ** ioReadPointer);
	//�F�̎擾(�}�e���A��)
	bool	GetMaterial(char ** ioReadPointer);

	//�g�p����|���S���̐��ƒ��_���Z�o���܂�.
	bool	GetModelCompriseCache(int inNo ,  MODEL outModel);
	//���f���̏ڍׂȏ����擾���܂�.
	bool	GetModelDetail(int inNo , MODEL outModel);
	//���_���擾���܂�.
	bool	SearchVec(int inVecNo , XYZFixed * outXYZ);

private:
	//�}�e���A��.
	MATERIAL*	m_Material;
	//�}�e���A���̐�.
	int			m_MaterialNum;
	//�f�[�^�̒���.
	int			m_FileDataLen;
	//���_���.
	_TCaretaker<TRokuVec>	m_VecCaretaker;
	//�����.
	_TCaretaker<TRokuLine>	m_LineCaretaker;
	//�ʏ��.
	_TCaretaker<TRokuFace>	m_FaceCaretaker;
};
