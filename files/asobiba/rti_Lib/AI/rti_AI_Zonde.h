#include <StartWithExeFile.h>
#ifdef ZONDE_LIFE

#ifndef ___ZONDEH
#define ___ZONDEH

#define ALL_CHAR	256	//�F���\�ȕ�����
#define MAX_PATTERN	10	//��̍��ڂɂ��ċL�^�ł��鎖��.
#define ZONDE_LINE	12
#define LOG_NAME	"Zonde.dat"
//�֐��錾.
class TAIZonde
{
	//�L�������.
	struct _CharPattern
	{
		int	LineHit[ZONDE_LINE];	//���C���ƃq�b�g������
		int	OkPattern;				//�F���ɐ���������
		bool DataAlive;				//���̃f�[�^���L�����ǂ���.
	};

	struct _TestPattern
	{
		int	 LineHit[ZONDE_LINE];	//���C���ƃq�b�g������
		bool BigLine[ZONDE_LINE];	//�ł������C���� 1 �Ƃ݂Ȃ�.
	};

	//�L���̈�̍쐬
	struct _CharPattern CharPattern[ALL_CHAR][MAX_PATTERN];
	//�F���e�X�g�p�̃f�[�^�ł�.
	struct _TestPattern	TestPattern;
	unsigned char Char;	//�F�����Ă��镶��
	int			  Char_PatternNo;	//���̕����̃f�[�^��Ԃ��Ă��ꂽ�Ƃ���
	//�ߋ��̋L�^���Q�b�g���܂�.
	bool LoadLog(char *filename);
	//�ߋ��̋L�^���܂�.
	bool SaveLog(char *filename);
	//�L���r���ɂ��܂�(��
	void ClearLog();
	//�����Q�b�g���܂�.
	void ZondeGetData(BMD bmd,int MojiColor);
	//�`�F�b�N���܂�!
	void ZondeCheck(BMD bmd,int x,int y,int n , int MojiColor);
	//�`�F�b�N�������e���r���Ă݂������.
	void ZondeMatting();
	//�f�[�^�x�[�X�ƌ��݂̃f�[�^�̔�r. (���������߂�) 2��a�����߂�.
	int ZondeDistance(int i,int l);
	//�����̃f�[�^�𖄂ߍ���
	void InsertData(unsigned char SekaiMoji);
public:
	TAIZonde(){};
	//�]���f�@�̏�����
	void Init();
	//�]���f�@�ŋ��߂����ʂɂ������č��ۂ������n��!!
	void HumanCheck(unsigned char SekaiMoji);
	//�]���f�@�̏I��
	void End();
	//�w�肵���G�ɂ�����Ă��镶���f�[�^��F�����܂�!!
	void Start(BMD bmd,int MojiColor);
	//�R�[�h�擾
	unsigned char GetCode(){return Char;};
};

#endif

#endif	//END LIFE
