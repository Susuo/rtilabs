#include <StartWithExeFile.h>
#ifdef TIMEACTIONH

#ifndef ___TIMEACTIONH
#define ___TIMEACTIONH

/*******************************************************************/
//�������e
/*******************************************************************/
#include <list>

typedef void (*TimerActionStructEvent)(unsigned int Tag);

struct TimerActionStruct
{
    bool alive;
    int Timer; //���ꂪ 0 �ɂȂ�����w�肳�ꂽ�C�x���g���s.
    TimerActionStructEvent BomeEvent;    //���ԂŎ��s�����C�x���g
    TimerActionStructEvent EveEvent;     //������s�����C�x���g
    unsigned int BomeEventTag;
    unsigned int EveEventTag;
};
#define TIMER_ACTION_MESSAGE_MAX 20

class TTimerAction
{
    TimerActionStruct TASTable[TIMER_ACTION_MESSAGE_MAX];
    //�󂫂�T��
    TimerActionStruct* SearchEmpty();
public:
    void Add(TimerActionStruct *TAS);
    void Add(int timer,TimerActionStructEvent BomeEvent,
        TimerActionStructEvent EveEvent,
        unsigned int BomeEventTag,unsigned int EveEventTag);
    void Del(TimerActionStruct *TAS);
    void Action();
};

extern TTimerAction TimeAction;

#endif

#endif //LIFE END
