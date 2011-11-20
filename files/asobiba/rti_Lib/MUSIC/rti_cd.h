#include "StartWithExeFile.h"
#ifdef CD_LIFE

#ifndef ___CDH
#define ___CDH

#pragma comment(lib,"winmm.lib")

#define CD_MAX 99+1

class TCDPlayer
{
	void Init();
    void End();
    //  �Ȃ̍Đ� ( ���Ԏw�� )
	void Play(int,int,int,int,int,int);
    //  �Ȃ̃��[�v�Đ�
	void Replay(void);

    // �Đ����
    bool PlayCD;
	bool LoopCD;
    // ���[�v�Đ��t���O
	bool LoopMode;

    // �ꎞ��~��Ԃł̎���
	int PauseM;
	int PauseS;
	int PauseF;

    // �J�n�Ȕԍ��ƏI���Ȕԍ�
	int StartSongNo;
	int EndSongNo;

	// �Ȃ̐�
    int SongMax;

    // CD���
    struct _CDInfo
    {
    	// �Ȃ̊J�n����
	    struct start
	    {
    		int m,s,f; // ���E�b�E�t���[��
	    }
	    Start;
		// �Ȃ̎���
    	struct time
    	{
	    	int m,s,f; // ���E�b�E�t���[��
	    }
	    Time;
    }CDInfo[CD_MAX];

public:
    TCDPlayer(){Init();};
    ~TCDPlayer(){End();};

    //���擾
	void GetInfo(void);
    //���ׂčĐ�
	void AllPlay();
    //�w�肵���g���b�N�Đ�
	void Play(int);
    //�w�肵���g���b�N�̊Ԃ��Đ�
	void Play(int,int);
    //�X�g�b�v
	void Stop(void);
    //�z�[�Y
	void Pause(bool);
    //�ȃX�L�b�v (�w�肵���ȃX�L�b�v����)
	void Skip(int);
    //������(�b)
	void Forward(int);
    //�����߂�(�b)
	void Backward(int);
    //CD �h���C�u�Ɂ@�u�c���͂����Ă��邩?
	bool Ready(void);
    //CD�h���C�u�I�[�v��
	void Open(void);
    //CD�h���C�u�N���[�Y
	void Close(void);
    //���[�v���Ȃ�
	void NoLoop(void);
    //�ő�̋ǐ�
	int  MaxSong(void);
    //���ݍĐ����Ă����
	int  NowSong(void);
    //����ɍĐ����I�������Ƃ��ɌĂ΂��
    void MciSuccessfulEnd();
};
#endif

#endif  //LIFE END
