#include "StartWithExeFile.h"
#ifdef MUSIC_LIFE

#ifndef ___MUSICH
#define ___MUSICH

#pragma comment(lib,"winmm.lib")

class TMusic
{
	bool PlayMusic; //���ݍĐ���
	bool LoopMusic; //���[�v�Đ���
	bool LoopMode;

    //������
	bool Init();
    //�I������
    void End();
public:
    TMusic(){Init();};
    ~TMusic(){End();};
	void Play(char* FileName);
	void Replay();
	void Stop();
	void NoLoop();
    void MciSuccessfulEnd();    //�Đ����I�����܂�������.

};


#endif

#endif //LIFE END


