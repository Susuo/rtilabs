#include <StartWithExeFile.h>
#ifdef TIMEACTIONH

#include <windows.h>
#include <.\Base\rti_time_action.h>
/*******************************************************************/
//�������e
/*******************************************************************/

TTimerAction TimeAction;

/*******************************************************************/
//�C�x���g�ǉ�(�\���̃o�[�W����)
/*******************************************************************/
void TTimerAction::Add(TimerActionStruct *TAS)
{
    TimerActionStruct *LpTAS = SearchEmpty();
    if (LpTAS)
    {
        CopyMemory(LpTAS,TAS,sizeof(TimerActionStruct) );
        LpTAS->alive = true;
    }
}

/*******************************************************************/
//�C�x���g�ǉ�(�����o�[�W����)
/*******************************************************************/
void TTimerAction::Add(int timer,TimerActionStructEvent BomeEvent,
        TimerActionStructEvent EveEvent,
        unsigned int BomeEventTag,unsigned int EveEventTag)
{
    TimerActionStruct *LpTAS = SearchEmpty();
    if (LpTAS)
    {
        LpTAS->alive = true;
        LpTAS->Timer = timer;
        LpTAS->BomeEvent = BomeEvent;
        LpTAS->EveEvent = EveEvent;
        LpTAS->BomeEventTag = BomeEventTag;
        LpTAS->EveEventTag = EveEventTag;
    }
}

/*******************************************************************/
//�����֐��@�󂫂�T��
/*******************************************************************/
TimerActionStruct* TTimerAction::SearchEmpty()
{
    TimerActionStruct *LpTAS = &TASTable[0];
    for(int i = 0; i < TIMER_ACTION_MESSAGE_MAX ; i++)
    {
        if (!LpTAS->alive) return LpTAS;
        LpTAS ++;
    }
    return NULL;
}

/*******************************************************************/
//�C�x���g�폜
/*******************************************************************/
void TTimerAction::Del(TimerActionStruct *TAS)
{
    TAS->alive = false;
}

/*******************************************************************/
//�C�x���g�����s�������ŕK�v�ȃ��c
/*******************************************************************/
void TTimerAction::Action()
{
    TimerActionStruct *LpTAS = &TASTable[0];
    for(int i = 0; i < TIMER_ACTION_MESSAGE_MAX ; i++)
    {
        if (LpTAS->alive)
        {
            LpTAS->Timer --;
            //������s
            if (LpTAS->EveEvent)  LpTAS->EveEvent(LpTAS->EveEventTag);
            //���Ԃ͑��v ?
            if (LpTAS->Timer <= 0)
            {   //�{���o�[
                if (LpTAS->BomeEvent)
                    LpTAS->BomeEvent(LpTAS->BomeEventTag);
                LpTAS->alive = false;
            }
        }
        LpTAS ++;
    }
}


#endif  //LIFE END
