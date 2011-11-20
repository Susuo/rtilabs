#include "StartWithExeFile.h"
#ifdef ANIME_LIFE

#ifndef ___ANIMEH
#define ___ANIMEH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>

/*****************************************************************/
/*****************************************************************/
/*�A�j���[�V��������									     	 */
/*****************************************************************/
/*****************************************************************/
#include "rti_graphics.h"
#include "rti_bitmaplib.h"

struct Anime_Src_struct;
struct Anime_Src_struct
{
	BMD bmd;
	short x;
	short y;
	short w;
	short h;
};

//�A�j���̊�{�ƂȂ�f�[�^�������ǂ�Ƃ��� 
class TAnimeSrc
{
private:
	short AnimeMax;         //�A�j���̃}�b�N�X
	Anime_Src_struct AS[6];
public:
    TAnimeSrc(){AnimeMax=0;};
	//�A�j���̍ő�l���擾
	short GetAnimeMax(){return AnimeMax;};
	//�f�[�^�Z�b�g
	void Set(Anime_Src_struct *a1,Anime_Src_struct *a2 ,
		Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,Anime_Src_struct *a5);
	//�G�施��
	void Draw(GraphicsProc Proc,int x,int y,short Pattern);
    //�p�^�[���̃f�[�^�̎擾
    Anime_Src_struct* Get(int Pattern){return &AS[Pattern];};
};

//�A�j���[�V���������邽�߂̃^�C�}�[�������ǂ�.
class TAnimeEx
{
private:
	short AnimePatternCounter ;
	short AnimeCounter ;
	short AnimeSpeed ;

	TAnimeSrc* AnimeSrcData;
	// �A�j��Ex �J�E���^�[���w��X�s�[�h�𒴂�����A�j���p�[�� ++
	bool AnimeStepUp();
public:
	TAnimeEx(){};
	// �A�j��Ex�ݒ�
	void Set(TAnimeSrc* _SrcData,short _speed);
	//�J�E���^�[���Z�b�g
	void ReSetCounter(){AnimePatternCounter = AnimeCounter = 0;};
	// �A�j��Ex �w�肵���ʒu�ɊG��
	bool Draw(GraphicsProc Proc,int x,int y);
    //�@�A�j���f�[�^�̎擾.
    Anime_Src_struct* Get();
};

#endif

#endif //LIFE END
