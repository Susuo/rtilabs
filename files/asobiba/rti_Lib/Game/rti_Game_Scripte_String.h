#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_STRING
#define ___SCRIPTE_STRING

#include <.\Game\rti_Game_Scripte_Run.h>

class TScripteString
{
    //�����ȕ����� "    " �̂�����̐؂�o��.
    void GetPureString(char *pStr,char* &str);
public:
    TScripteString(){};
    //�������z��Ƀ}�b�s���O
    int StringMapping(char* &str,USE_STRING *Us);
};

#endif

#endif //LIFE END
