#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

#ifndef ___RTI_WAVE_PLAYER
#define ___RTI_WAVE_PLAYER


#include <.\Music\rti_wave.h>
#include <list>
#pragma warning(disable: 4786)
#pragma comment(lib,"winmm.lib")

struct PlayingWaveList
{
    WVD wvd;                //�Đ�����ȃf�[�^
    unsigned long Offset;   //���݂̍Đ������o�C�g��
};
class TWavePlayer
{
    HWAVEOUT    hWaveOutHandle;	// �o�̓f�o�C�X�̃n���h�����擾����ϐ�
    WAVEHDR     *FWaveHdr[2];    //���t�o�b�t�@�̃A�h���X.(�_�u���o�b�t�@)
    WAVEFORMATEX    WaveFormat;  //

    std::list<PlayingWaveList*> PlayingList;
    bool        WavePlayOk;
    bool        Playing;    //���ݍĐ���.
	unsigned char BufUsing;	//�o�b�t�@���g���Ă���(0��������o�b�t�@�͋�)
    unsigned char Pool;     //�v�[�����Ă���E�F�[�u�̗�

    //���t��~���牉�t���Ăˁ@�ɂ�������Ƃ��ɂ͂�����.
    void FirstPlay();
    //���̃f�[�^ �v���[�Y
    void WriteNext(WAVEHDR *hdr);
    //�\�񂳂�Ă��� �f�[�^���[�h
    int  WVDDataLoadBuf(WAVEHDR *hdr);
    //�\�񂳂�Ă��� �f�[�^���[�h�����s
    int WVDDataLoadBuf_WaveAmp(WAVEHDR *hdr);
    //�\�񂳂�Ă����f�[�^�̒��ōŌ�܂ōĐ��������c��\�񂩂�폜
    void WVDDataLoadBuf_EndPlayDel();
public:
    TWavePlayer();
    ~TWavePlayer();
    //�v�[���������r�����čĐ�������.
    void Ref();
    //���̃f�[�^���Đ�����.
    void Play(WVD wvd);
    //�Đ��������������Ă���. 
    void WaveSuccessfulEnd(WAVEHDR *hdr);
	//���ׂĔj��
	void All_Breaker();
};

#endif

#endif //LIFE END
