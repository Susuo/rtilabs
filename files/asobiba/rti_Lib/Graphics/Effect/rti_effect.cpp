#include "StartWithExeFile.h"
#ifdef EFFECT_LIFE
/*****************************************************************/
/*****************************************************************/
//   �O���t�B�b�N�G�t�F�N�g�Ǘ�
/*****************************************************************/
/*****************************************************************/

#pragma warning(disable: 4786)
#include <list>
#include <algorithm>
#include <.\Graphics\Effect\rti_effect.h>
#include <.\Base\rti_debug.h>

TEffect Effect;

/********************************************************/
//�R���X�g���N�^
/********************************************************/
TEffect::TEffect()
{
    all_breaker = false;
}

/********************************************************/
//�f�X�g���N�^
/********************************************************/
TEffect::~TEffect()
{
    AllDestroy();
}


/********************************************************/
//�G�t�F�N�g�̍쐬
/********************************************************/
void TEffect::Make(TEffectObject *My)
{
    EffectList.push_back(My); 
}

/********************************************************/
//�G�t�F�N�g�̔j��
/********************************************************/
void TEffect::Destroy(TEffectObject *My) 
{
    if(all_breaker) return ;

	//���X�g�̂Ȃ�����
    std::list<TEffectObject*>::iterator it =
        std::find(EffectList.begin(),EffectList.end(),My);
    if(it != EffectList.end())
    {
            //�f�[�^�̔j��
            EffectList.erase(it);
    }
}

/********************************************************/
//���ׂẴG�t�F�N�g�̔j��
/********************************************************/
void TEffect::AllDestroy()
{
    all_breaker = true;
    std::list<TEffectObject*>::iterator it;

    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = EffectList.begin() ; it != EffectList.end() ; it ++)
    {
        (*it)->VEffectDestroy();
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    EffectList.clear();
}

#endif //LIFE END
