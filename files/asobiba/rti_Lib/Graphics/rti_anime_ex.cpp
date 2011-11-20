#include "StartWithExeFile.h"
#ifdef ANIME_LIFE

/*****************************************************************/
/*****************************************************************/
/*�A�j���[�V��������									     	 */
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Graphics\rti_anime_ex.h>
/**********************************************************/
// �A�j���\�[�X�ݒ�
/**********************************************************/
void TAnimeSrc::Set(Anime_Src_struct *a1,Anime_Src_struct *a2 ,
					Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,
					Anime_Src_struct *a5)
{
	int i;
	i = 0;
	while(1)
	{
		if (a1== NULL || a1->bmd == NULL) break;
		memcpy(&AS[i++],a1,sizeof(Anime_Src_struct) );
		if (a2== NULL || a2->bmd == NULL) break;
		memcpy(&AS[i++],a2,sizeof(Anime_Src_struct) );
		if (a3== NULL || a3->bmd == NULL) break;
		memcpy(&AS[i++],a3,sizeof(Anime_Src_struct) );
		if (a4== NULL || a4->bmd == NULL) break;
		memcpy(&AS[i++],a4,sizeof(Anime_Src_struct) );
		if (a5== NULL || a5->bmd == NULL) break;
		memcpy(&AS[i++],a5,sizeof(Anime_Src_struct) );
		break;
	}
	AnimeMax = i;
}

/**********************************************************/
// �A�j���\�[�X �w�肳�ꂽ�p�^�[�������.
/**********************************************************/
void TAnimeSrc::Draw(GraphicsProc Proc,int x,int y,short Pattern)
{
	if (AnimeMax < Pattern) return;
	Anime_Src_struct *puf = &(AS[Pattern]) ;
    Proc(puf->bmd,puf->x,puf->y,puf->w,puf->h,Sprite,x,y);
}


/**********************************************************/
// �A�j��Ex�ݒ�
/**********************************************************/
void TAnimeEx::Set(TAnimeSrc* _SrcData,short _speed)
{
	AnimePatternCounter = 0; //�A�j���p�^�[���J�E���^�[��������
	AnimeCounter = 0;        //�A�j���J�E���^�[������
	AnimeSpeed = _speed;     //�A�j���X�s�[�h
	AnimeSrcData = _SrcData; //�\�[�X�f�[�^
}

/**********************************************************/
// �A�j��Ex �J�E���^�[���w��X�s�[�h�𒴂�����A�j���p�[�� ++
/**********************************************************/
bool TAnimeEx::AnimeStepUp()
{
	if (AnimeCounter >= AnimeSpeed)
	{
		AnimeCounter = 0;

		//�A�j���p�^�[�����������
		AnimePatternCounter++;
		//�ő�̃p�^�[����������ŏ��ɖ߂�
		if (AnimePatternCounter >= AnimeSrcData->GetAnimeMax() )
		{
			AnimePatternCounter = 0;
			return true;
		}
	}
	AnimeCounter ++;
	return false;
}

/**********************************************************/
// �A�j��Ex �w�肵���ʒu�ɊG��
/**********************************************************/
bool TAnimeEx::Draw(GraphicsProc Proc,int x,int y)
{
	//���݂̃A�j���p�^�[�����G�悵�Ă��炤.
	AnimeSrcData->Draw(Proc,x,y,AnimePatternCounter);
	return AnimeStepUp();
}

/**********************************************************/
//�@�A�j���f�[�^�̎擾.
/**********************************************************/
Anime_Src_struct* TAnimeEx::Get()
{
    Anime_Src_struct* Anime_Src;
	//���݂̃A�j���p�^�[�����G�悵�Ă��炤.
	Anime_Src = AnimeSrcData->Get(AnimePatternCounter);
	AnimeStepUp();
    return Anime_Src;
}
#endif //LIFE END
