#include "StartWithExeFile.h"
#ifdef BASE_MAP

#ifndef ___MAPH
#define ___MAPH
/**************************************************************************************/
//�}�b�v (���̃}�b�v�͉~���X�N���[���͂��܂��� (^^;; ) 
//�~���X�N���[�����Ăق����Ƃ��ɂ́A ���� TMap �N���X�̏�ʃN���X TMapRotate ���g���Ă�������
/**************************************************************************************/
#include <.\Graphics\rti_graphics.h>

//�}�b�v�`�b�v�̃T�C�Y�� 16 * 16����(��)
#define MAP_CHIP_SIZE		16
#define MAP_CHIP_POW_SIZE	4
#define MAP_FILENAME	"No%d.def"
#define MAP_HEDER "This is MapFile"

//��ʊ֌W
#define SCREENXSIZE (ScreenX)
#define SCREENYSIZE (ScreenY)
#define SCREENXSIZEHARF	(SCREENXSIZE>>1)
#define SCREENYSIZEHARF	(SCREENYSIZE>>1)
#define MAP_SCREEN_XSIZE (SCREENXSIZE >> MAP_CHIP_POW_SIZE) //���ʕ�
#define MAP_SCREEN_YSIZE (SCREENYSIZE >> MAP_CHIP_POW_SIZE) //���ʕ�
//�}�b�v�̑��s�N�Z����
#define MAP_FULLX	(MapW << MAP_CHIP_POW_SIZE)
#define MAP_FULLY	(MapH << MAP_CHIP_POW_SIZE)

//�`�b�v�֌W
#define MAP_CHIPNO_TO_MAP_X(no) ((no) % 20) //�}�b�v�`�b�v�i���o�[���`�b�v�p���b�g���W�ɕϊ�
#define MAP_CHIPNO_TO_MAP_Y(no) ((no) / 20) //�}�b�v�`�b�v�i���o�[���`�b�v�p���b�g���W�ɕϊ�
#define MAP_NOW_POINT(x,y) ((x) + (y) * MapW) //���ݓǂݏ����ł���Ƃ���
//�X�N���[�����~�߂�ꏊ
#define MAP_STOP_XSCROLL ((MapW-MAP_SCREEN_XSIZE-1)<<MAP_CHIP_POW_SIZE)
#define MAP_STOP_YSCROLL ((MapH-MAP_SCREEN_YSIZE-1)<<MAP_CHIP_POW_SIZE)
//�V�т̕���
#define NOMOVEX (80)
#define NOMOVEY (64)
//���̃`�b�v���ʂ�邩�ǂ������ׂ�.
#define MAP_THIS_CHIP_THUR(Chip) ( (Chip) >=MAP_SCREEN_YSIZE && (Chip) <= MAP_SCREEN_YSIZE*4)

class TMap
{
	int MapNo;	//�������Ă���}�b�v�i���o�[.
	char *ChipFileName;	//�`�b�v�̊G�̃f�[�^�������Ă�����.
	BMD	 Chip;
protected:
	int ScreenX,ScreenY;
	int Nowx,Nowy;		//���݂̍���̍��W.
	short MapW,MapH;		//�}�b�v�̕��ƍ���
	short *Map;			//�}�b�v�f�[�^

	void MapDrawing(BMD bmd,int Dx,int Dy,int Sx,int Sy,int W,int H);		//�}�b�v �`��
public:
	TMap(){ScreenX = ScreenY = Nowx =  Nowy = 0;};
	~TMap(){Destroy();};
	int GetNowx(){return Nowx;};
	int GetNowy(){return Nowy;};
	void SetScreen(int x,int y){ScreenX = x; ScreenY = y;};	//�X�N���[���T�C�Y��`
	virtual int GetScreenX(int x){return x - Nowx;};
	virtual int GetScreenY(int y){return y - Nowy;};

	bool Read(int no);	//�}�b�v���[�h
	void Destroy();		//�}�b�v�f�[�^�̔j��
	void DrawMain(BMD bmd,int w,int h);					//�}�b�v���G�悵�܂�
	virtual void Draw(BMD bmd){TMap::DrawMain(bmd,SCREENXSIZE,SCREENYSIZE);};	//�}�b�v���G�悵�܂�
	virtual void MapMasterMove(short nx,short ny,int speed);//��l���ړ��Ȃǂɂ��}�b�v�X�N���[��
	virtual void SetCeneter(short nx,short ny);				//�w�肵���|�C���g����ʂ̒��S�ɂ���.
	virtual bool If(short nx,short ny);			//���̃|�C���g��ʉ߂ł��邩�`�F�b�N
	bool Clipping(int x,int y,int w,int h);			//�}�b�v�̊O�ɔ�яo���Ȃ��悤��... ��яo���Ă����� true
	bool IamNotDisplay(short x,short y,short faj);	//���͕\������Ă��Ȃ��̂���? ����ĂȂ� true
};


#endif

#endif  //LIFE END

