#include "StartWithExeFile.h"
#if defined(GAME_ANIME_LIFE) && defined(SCRIPTE_LIFE)

#ifndef ___GAME_CHARH
#define ___GAME_CHARH

#include <.\Game\rti_Game_AnimeBankForFile.h>
#include <.\Game\rti_Game_Scripte_Run.h>

class TGame_CharBase
{
public:
	int x,y,z;	//���W.
	int sx,sy,sz;	//�ړ���.
	TGame_CharBase(){x = y = z = sx = sy = sz = 0;};
    bool alive;
	int Tag;		//�^�O
};

class TGame_CharAnime : public TGame_CharBase
{
	TAnimeEx AEx[8];	//�A�j���[�V�����f�[�^
	int Animes;	//�����ɂ��A�j���p�^�[�� (1�p�^�[���A4���ʁA8���� �̂����ꂩ.)
	int Muki;	//
public:
	TGame_CharAnime(){};
	//���̕����̃A�j���f�[�^���Z�b�g. no �́A GameAnimeBank ����.
	void SetAnimeSrc(int muki,int no,int speed){ AEx[muki].Set( GameAnimeBankForFile.Get(no) , speed); };
    //���݂̃A�j���f�[�^��Ԃ�.
    TAnimeEx* GetAnime(){return &AEx[Muki];}
    //���݂̃A�j���f�[�^�̃A�j���\���̂�Ԃ�.GetAnime() �̃p���[�A�b�v��
    Anime_Src_struct* GetAnimeSrc(){ return AEx[Muki].Get(); };
};

class TScripeRunAreaRunPlus : public TScripeRunArea
{
public:
	void Run(TScripteRun* Sr){ while( ! Sr->Run(RunNumber,this) ); };
};


class TGameChar : public TGame_CharAnime , public TScripeRunAreaRunPlus
{
};

//�^�X�N�V�X�e����⏕����N���X.
class TBasicTaskFriend
{
    bool alive;
};

//�^�X�N�V�X�e�� �`  �Ȃ񂩁A�f�B���N�V�X�e���݂����Ȗ��O�ŗǂ��ł���.
//       ----------
//       |        |
//       |        |     << ����Ȃ̂��o�Ă��āA�}�~�I�ƃ��~�[�W��
//       ----------        �S����������񂾂��. ���ւ�
//           ||

//_T �́A���̃^�X�N�������������G��ʂ̃N���X.
//_NUM �́A���̐�(�ő吔�ł�.)
//_T �̃N���X�́A�Œ� 2 �̎��𖞂����Ă��Ȃ���΂Ȃ�܂���.
//        1 ,  bool alive;  �ƁA���� public �A�N�Z�X���\�ȃ����o��ێ����Ă���.
//              �܂��́A TBasicTaskFriend �N���X���p�����Ă���N���X.
//        2 ,  void Action(); �ƁA�����@�ȉ�����.
//          
template<class _T,int _NUM>

class TBasicTask
{
    friend class TBasicTaskFriend;  //�����������Ă��� alive �����o�� public �A�N�Z�X�ł���悤��.
protected:
    _T  Task[_NUM];
public:
    TBasicTask(){};

    //������
    void clear()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            puf->alive = false;
        }
    };
    //�^�X�N�����s������.
    void dispach()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            if ( puf->alive )
            {
                puf->Action();
            }
        }
    };
    //�j��.
    void destroy(_T* puf)
    {
        puf->alive = false;
    };
    //�o�^
    _T* attach()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            if ( puf->alive ) continue;
            //�o�^.
            puf->alive = true;
            return puf; //���[�ށA�A�h���X�͕Ԃ������Ȃ��񂾂��ǂȂ�
        }
        return NULL;
    };
};


#endif

#endif      //LIFE END

