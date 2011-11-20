#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <.\Game\rti_Game_Scripte_String.h>

//�����ȕ����� "    " �̂�����̐؂�o��.
void TScripteString::GetPureString(char *pStr,char* &str)
{
    int Pure = false;
    while(*str)
    {
        if (*str == '"')
        {
            if(!Pure) Pure = true;
            else      
            {
                *pStr = 0;  //�k��
                break;    //���[�v�E�o.
            }
        }
        else
        {
            if (Pure) //'"' �̒����������^���Ă���.
            {
                *pStr = *str;
                pStr++;
            }
        }
        str++;
    }
}

//�������z��Ƀ}�b�s���O
int TScripteString::StringMapping(char* &str,USE_STRING *Us)
{
    char pureString[256];
    int MappingNo;
    //�����ȕ�����̐؂�o��.
    GetPureString(pureString,str);
    //�}�b�s���O����ꏊ���擾.
    MappingNo = Us->size();
    Us->push_back( std::string(pureString) );
    //�}�b�s���O�����ꏊ��Ԃ�.
    return MappingNo;
}

/*
//������̎擾
const char* GetMappingString(int No)
{
    return (UseString[No]).c_str();
}
*/

#endif //LIFE END
