#include "StartWithExeFile.h"
#ifdef EFFECT_LIFE

#ifndef ___EFFECTH
#define ___EFFECTH

#include <list>

class TEffectObject;

//�G�t�F�N�g�Ǘ�
class TEffect
{
    bool all_breaker; //���ׂĔj������Ƃ��ɂ���
    std::list<TEffectObject*> EffectList;
public:
    void Make(TEffectObject *My);
    void Destroy(TEffectObject *My) ;
    void AllDestroy();
    TEffect();
    ~TEffect();
};
extern TEffect Effect;

////////////////////////////////
//�G�t�F�N�g
// ���ׂẴG�t�F�N�g�͂�����p�����Ȃ��Ă͂Ȃ�܂���.
////////////////////////////////
class TEffectObject
{
protected:
    void EffectMake(){Effect.Make(this); };
    void EffectDestroy(){Effect.Destroy(this); };
public:
    TEffectObject(){};
    ~TEffectObject(){};
    virtual void VEffectDestroy() = 0;
};

#endif

#endif //LIFE END
