#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

#ifndef ___RTI_WAVE
#define ___RTI_WAVE
#include <windows.h>
#include <list>
#pragma warning(disable: 4786)

struct WAVE_DATA_TABLE
{
    char *wv;       //���ۂ̃f�[�^
    DWORD FSize;    //����̃T�C�Y
};

typedef WAVE_DATA_TABLE* WVD;

class TWaveLib
{
private:
    std::list<WVD> WvdList;
public:
    TWaveLib();
    ~TWaveLib();
    WVD Make(char *filename);
    void All_Breaker();
    void Breaker(WVD puf);
};

extern TWaveLib *WaveChar;


#endif


#endif //LIFE END
