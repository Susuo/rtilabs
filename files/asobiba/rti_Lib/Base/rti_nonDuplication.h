#ifndef ___NON_DUPLICATIONH
#define ___NON_DUPLICATIONH

/*****************************************************************/
//���d�N���̖h�~    �A�g����(�S�r����Ȃ���(����) )
/*****************************************************************/
#include <windows.h>

class TNonDuplication
{
    ATOM Atom;
public:
    TNonDuplication(){ Atom = NULL;};
    ~TNonDuplication();
    //���d�N�����Ă��邩�ǂ������ׂ�. ���Ă��� = true
    bool IsDuplication(char *SeriStr);
};

#endif
