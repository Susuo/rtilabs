
#include <stdio.h>
#include <string.h>
#include <.\Base\rti_data_list.h>

//�T�[�`
int TCommandList::search(char *str,char **NextChar)
{
    //�R�}���h���
    for(int i = 0; CL[i] != NULL ; i++)
    {
        //�R�}���h����
        char *p = strstr(str,CL[i]);
        if (p)
        {   //��������!!
            *NextChar += (strlen(CL[i])+1) ;  //�R�}���h�̎��̕����Ɉړ�
//            *NextChar += CL[i].Len ;  //�R�}���h�̎��̕����Ɉړ�
            return i;
        }
    }
    //������Ȃ�.
    return -1;
}

