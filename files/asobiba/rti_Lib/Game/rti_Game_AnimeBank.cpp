#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE


#include <.\Game\rti_Game_AnimeBank.h>
#include <stdio.h>
#include <algorithm>

TGameAnimeBank GameAnimeBank;

/*******************************************************************/
//�V�K�쐬
/*******************************************************************/
TAnimeSrc* TGameAnimeBank::New(int *MappingNo,Anime_Src_struct *a1,Anime_Src_struct *a2 ,
		Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,Anime_Src_struct *a5)
{
	//�A�j���\�[�X�쐬
	TAnimeSrc* AS = new TAnimeSrc();
	//�\�[�X�f�[�^�Z�b�g
	AS->Set(a1,a2,a3,a4,a5);
	//�o�^
	AnimeBank.push_back(AS);
    if (MappingNo != NULL) *MappingNo = AnimeBank.size()-1;

	return AS;
}

/*******************************************************************/
//�폜
/*******************************************************************/
void TGameAnimeBank::Destroy(TAnimeSrc* puf)
{
    std::vector<TAnimeSrc*>::iterator it =
        std::find(AnimeBank.begin(),AnimeBank.end(),puf);
    if(it != AnimeBank.end())
    {
        	delete puf;
            AnimeBank.erase(it);
    }
}

/*******************************************************************/
//�S�č폜
/*******************************************************************/
void TGameAnimeBank::All_Breaker()
{
    std::vector<TAnimeSrc*>::iterator it;

    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = AnimeBank.begin() ; it != AnimeBank.end() ; it ++)
    {
        delete (*it);
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    AnimeBank.clear();
}


#endif //LIFE END
