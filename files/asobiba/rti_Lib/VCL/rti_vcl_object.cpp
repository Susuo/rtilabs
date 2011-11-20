/**********************************************************/
// VCL�̊�{
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef OBJECT_LIFE
#include <list>
#include <algorithm>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>

/**********************************************************/
// �C�x���g�̏��������R���X�g���N�^�ł��
/**********************************************************/
TObject::TObject()
{
	OwnerUse        = 0;
    OwerKill        = false;
}

/**********************************************************/
//���������L���Ă���R���|�[�l���g��������
/**********************************************************/
void TObject::OwnerAdd(TObject* hOwner,TObject* hMy)
{
    TObject* hhOwner;

    //�I�[�i�[�� NULL �������炷�ׂăA�v���P�[�V�������Ђ�������
    if (!hOwner) hhOwner = Application;
    else         hhOwner = hOwner;

    //���X�g�ɒǉ�
	hhOwner->ObjectList.push_back(hMy);

    if (!hOwner)    hMy->hMyOwner = NULL;
    else            hMy->hMyOwner = hhOwner;
}

/**********************************************************/
// ���������L���Ă��邷�ׂẴR���|�[�l���g�̔j��
/**********************************************************/
void TObject::ChildDestroy()
{
    std::list<TObject*>::iterator it;

    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = ObjectList.begin() ; it != ObjectList.end() ; it ++)
    {
		ObjectTrancerationAndBreak((*it));
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    ObjectList.clear();
}

/**********************************************************/
// �������j�����ꂽ���Ƃɂ�鏈��
/**********************************************************/
void TObject::IamDestroy()
{
    if (hMyOwner == NULL)   return;
    //�����̃I�[�i�[��������j�󂵂Ă����ꍇ�A
    //�����̔j�����E�l�S�ł���I�[�i�[�ɂ�����K�v�͂Ȃ����낤.(:-
    if (hMyOwner->OwerKill)
    {
        return;
    }
    
    std::list<TObject*>::iterator it =
            std::find(hMyOwner->ObjectList.begin(),
                      hMyOwner->ObjectList.end(),this);
    if(it != hMyOwner->ObjectList.end())
    {
            ObjectList.erase(it);
    }
}

/**********************************************************/
/* ���̃I�u�W�F�N�g�̔j������Ƃ��̂���ׂ��p�ɃL���X�g���ĎE��*/
/**********************************************************/
void TObject::ObjectTrancerationAndBreak(TObject* hObject)
{
    hObject->UserDestroy();
}



#endif  //LIFE END
