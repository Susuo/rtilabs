#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <stdio.h>
#include <.\Game\rti_Game_Scripte_Comple.h>
#include <.\Base\rti_data_list.h>
#include <.\Base\rti_error_message.h>

#define COMPLE_MAX_BUFFER_SIZE 256
#define COMPLE_ERROR_OUT(INPORMATION)    (MsgError3("CompleError File %s (%dLines)\n %s",CompleFilename,FileLen,INPORMATION))

/***********************************************************/
//���̃t�@�C�����R���p�C�����ăf�[�^���i�[
/***********************************************************/
bool TScripteComple::Comple(char *filename,ACTION_VECTOR *Ac,USE_STRING *Us)
{
    FILE *fp;
    char buf[COMPLE_MAX_BUFFER_SIZE];
    //�\���.
    char *CL[] =
    {
        "#*",           //�R�����g  �H������Ŏg���̂́A�Z�����g ... (��װ)
        "#DEFINE",      //��`
        "#CALS",        //�v�Z
        "#IF",          //IF
        "#ELSE",        //ELSE
        "#ENDIF",       //ENDIF
        "#WHILE",       //WHILE
        "#ENDWHILE",    //ENDWHILE
        "#SWITCH",      //SWITCH
        "#ENDSWITCH",   //ENDSWITCH
        "#CASE",        //CASE
        "#BREAK",       //BREAK
        "#END",         //END
        NULL,
    };
    TCommandList TCL;   //�����G���W�� (^^;

    char *next_str;     //�\���̎��̕�����.
    //�\���������Ώۂɏ��F����.
    TCL.set(CL);
    //��������}�b�s���O����Ƃ���.
    UseString = Us;
    //�t�@�C���������������Ă���.
    CompleFilename = filename;
    //�t�@�C���̍s����������
    FileLen = 0;

    fp = fopen(filename,"rb");
    if (!fp)
    {
        FileNotOpen("TScripteComple::Comple",filename);
        return false;
    }
    try
    {
        while (! feof(fp) )
        {
            FileLen++;  //�t�@�C���s�����J�E���g���܂�.
            fgets(buf,COMPLE_MAX_BUFFER_SIZE,fp);
            next_str = buf;
            switch (TCL.search(buf,&next_str) )
            {
            case 0: //�R�����g
                break;
            case 1: //��`.
                if ( !CompleDefine(next_str,Ac) ) throw 0;
                break;
            case 2: //�v�Z
                if ( !AnalysisString(next_str) )  throw 0;//���̕��������͂���
                break;
            case 3: //IF
                if ( !AnalysisString(next_str) )  throw 0;//���̕��������͂���
                Comple_Ana_Stack.push( MAction->size() );   //���� IF �̂������ꏊ���X�^�b�N�ɓo�^.
                NewCommand(E_IF);   //���򖽗ߔ���.
                break;
            case 4: //ELSE
                if ( !CompleElse(next_str) ) throw 0;
                break;
            case 5://ENDIF
                //�X�^�b�N���A��� if , else �̈ʒu���擾.
                if ( !CompleEndIf(next_str) ) throw 0;
                break;
            case 6://WHILE
                if (!AnalysisString(next_str) )  throw 0;//���̕��������͂���
                Comple_Ana_Stack.push( MAction->size() );   //���� WHILE �̂������ꏊ���X�^�b�N�ɓo�^.
                NewCommand(E_IF);   //���򖽗ߔ���.
                break;
            case 7://ENDWHILE
                if ( !CompleEndWhile(next_str) ) throw 0;
                break;
            case 8://SWITCH
                break;
            case 9://ENDSWITCH
                break;
            case 10://CASE
                break;
            case 11://BREAK
                if ( !CompleBreak(next_str) ) throw 0;
                break;
            case 12://END
                NewCommand(E_HAL);   //END �I������
                break;
            default:    //���[�U��`�� ?
                if ( !CompleUserFunktion(buf)) throw 0;
                break;
            }
        }
    }
    catch(...)
    {   //�r���ňُ픭��!!
        fclose(fp);
        return false;
    }

    fclose(fp);

	//�œK��.
	Saitekika(Ac);

    return true;
}

/***********************************************************/
//#DEFINE���̉���.
/***********************************************************/
bool TScripteComple::CompleDefine(char *next_str,ACTION_VECTOR *Ac)
{
    int Def;
    //#DEFINE �̒l�̎擾.
    sscanf(next_str,"%d",&Def);
    if (Def >= ACTION_MAX) 
    {
        COMPLE_ERROR_OUT("#DEFINE �̌��E�𒴂�����`�ł���.");
        return false;
    }
    //����R���p�C�����Ă����ꏊ������. MAction �́Aprivate �����o
    MAction = & Ac[Def];
    return true;
}

/***********************************************************/
//#ELSE���̉���.
/***********************************************************/
bool TScripteComple::CompleElse(char *next_str)
{
    //�X�^�b�N���A��� if �̈ʒu���擾.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //�O�̂��ߊm�F.
    if ( (*MAction)[w].Nim != E_IF)
    {
        COMPLE_ERROR_OUT("#ELSE �� #IF ���Ή����Ă܂ւ�.");
        return false;
    }

    //�s�����̎��́A�����̎��̖��߂�.
    (*MAction)[w].O1 = MAction->size() + 1;
    Comple_Ana_Stack.push( MAction->size() );   //���� ELSE ���������ʒu���L�^
    NewCommand(E_JMP);   //ELSE �̎��̕��򖽗�.

    return true;
}

/***********************************************************/
//#ENDIF���̉���.
/***********************************************************/
bool TScripteComple::CompleEndIf(char *next_str)
{
    //�X�^�b�N���A��� if , else �̈ʒu���擾.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //�O�̂��ߊm�F.
    if (! ((*MAction)[w].Nim == E_IF || (*MAction)[w].Nim == E_JMP) )
    {
        COMPLE_ERROR_OUT("#ENDIF �� �����Ȃ�Ă΂�ďo�Ă��ăY�h�h�h�[���ł�.");
        return false;
    }
    //�s�����̎��́A�����̎��̖��߂�.
    (*MAction)[w].O1 = MAction->size() ;
    return true;
}

/***********************************************************/
//#ENDIF���̉���.
/***********************************************************/
bool TScripteComple::CompleEndWhile(char *next_str)
{
    //�X�^�b�N���A��� while �̈ʒu���擾.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //BREAK �� �O�̂��ߊm�F.
    if ((*MAction)[w].Nim == E_JMP)
    {   //BREAK!
        (*MAction)[w].O1 = MAction->size() ;
        //����́A WHILE
        w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    }
    //�O�̂��ߊm�F.
    if ((*MAction)[w].Nim == E_IF)
    {   //�s�����̎��́A�����̎��̖��߂�.
        (*MAction)[w].O1 = MAction->size() + 1;
        NewCommand(E_JMP,w);   //ENDWHILE �̎��� WHILE �܂ŃW�����v
    }
    else
    {
        COMPLE_ERROR_OUT("#ENDWHILE �� �s���ɏo�����܂���.");
        return false;
    }
    return true;
}

/***********************************************************/
//#BREAK���̉���.
/***********************************************************/
bool TScripteComple::CompleBreak(char *next_str)
{
    //�X�^�b�N���A��� while �̈ʒu���擾.
    int w = Comple_Ana_Stack.top(); //�擾���邾�����Ă��Ƃɒ���.
    //�O�̂��ߊm�F.
    if ((*MAction)[w].Nim == E_IF)
    {
        Comple_Ana_Stack.push( MAction->size() );   //���� BREAK �̂������ꏊ���X�^�b�N�ɓo�^.
        NewCommand(E_JMP);   //BREAK �̎��̕��򖽗�.
    }
    else
    {
        COMPLE_ERROR_OUT("#BREAK �� �s���ɏo�������ɂ�[.");
        return false;
    }
    return true;
}
/***********************************************************/
//���[�U��`���̉���.
/***********************************************************/
bool TScripteComple::CompleUserFunktion(char *buf)
{
    if ( !UserOperation(buf) )   //���[�U����`�����j���j�b�N ?
    {
        if (! FunktionSearch(buf) )    //���[�U����`�����֐� ? 
        {
            COMPLE_ERROR_OUT("��͕s�\���ɂ�[.");
            return false;
        }
    }
    return true;
}

/***********************************************************/
//���[�U��`�̃t�@���N�V�����������A�W�J���܂�.
/***********************************************************/
bool TScripteComple::FunktionSearch(char *buf)
{
    TCommandList TCL;
    int ope,w;
    char *next_str;

    if ( ! FunktionStr) return false;

    TCL.set(FunktionStr);
    next_str = buf;
    //���[�U���߂�I��.
    ope =  TCL.search(buf,&next_str) ;
    if (ope < 0)    return false; //��Ǖs�\

    //��ǂł���!
    w = FunctionParaCals(next_str); //�p�����^��W�J.
    NewCommand(E_FUNK,ope,w);       //���[�U��`�t�@���N�V�����Ăяo��.
    return true;    //ok.
}

/***********************************************************/
//���[�U��`�̃t�@���N�V�����̃p�����[�^�̓W�J.
/***********************************************************/
int TScripteComple::FunctionParaCals(char *str)
{
    char buf[COMPLE_MAX_BUFFER_SIZE];
    int ParaNum = 0;
    int buf_counter = 0;
    for( ; *str ; str ++)
    {
        switch(*str)
        {
        case ',':
            buf[buf_counter] = 0;  //���̕��̏C���p.
            AnalysisString(buf);   //���̕��������͂���
            NewCommand(E_PUSH,REG_S);       //S ���W�X�^���X�^�b�N�ɐς�.
            buf_counter = 0;
            ParaNum ++;
            break;
        default:
            buf[buf_counter++] = *str;
            break;
        }
    }
    if (buf_counter > 0)
    {
        buf[buf_counter] = 0;  //���̕��̏C���p.
        AnalysisString(buf);   //���̕��������͂���
        NewCommand(E_PUSH,REG_S);       //S ���W�X�^���X�^�b�N�ɐς�.
    }
    return ParaNum;
}

/***********************************************************/
//���̕�������v�Z���@�y�сA�����̂ǂ��炩�œW�J.
/***********************************************************/
bool TScripteComple::AnalysisString(char* str)
{
    //������̏؋� '"' ��T��.
    int Cnt = 0;
    char *StrChk = str;
    for( ; *StrChk ; StrChk++) if (*StrChk == '"') Cnt ++;

    if (Cnt > 0)
    {   //������.
        NewCommand(E_MOVI,REG_S,
            ScripteString.StringMapping(str,UseString) );       //S ���W�X�^�Ƀ}�b�s���O�����ꏊ��Ԃ�.
    }
    else
    {   //����
        ScripteRevPorland.RevPorland( str , *MAction);
    }
    return true;
}


/***********************************************************/
//�œK��.
/***********************************************************/
void TScripteComple::Saitekika(ACTION_VECTOR *Ac)
{
	int ActionNo ;
	int OpeNo ;
	int NextOpNo;
	int MaxActiones;
	Action* lpTempAction;
	Action* lpNextTempAction;
	ACTION_VECTOR *NowAction;
	//��{�I�ȍœK��.
	//	MOV  REG_S,1		PUSH REG_S		=> PUSH  1
	//	MOVI REG_S,1		PUSH REG_S		=> PUSHI 1

	for(ActionNo = 0 ; ActionNo < ACTION_MAX ; ActionNo++)
	{
		NowAction = & Ac[ActionNo];
		MaxActiones = (*NowAction).size() - 1;	//���[�v��
		//�e���߂̓��e���ᖡ���܂�.
		for(OpeNo = 0 ; OpeNo < MaxActiones ; OpeNo ++)
		{
			lpTempAction = &(*NowAction)[OpeNo];
			if ( (lpTempAction->Nim == E_MOV || lpTempAction->Nim == E_MOVI) && (lpTempAction->O1 == REG_S) )
			{
				NextOpNo = OpeNo+1;
				if (NextOpNo >= MaxActiones) break; //���̖��߂͂Ȃ��̂ł��˂� �ӂւ�
				//���̖��߂��擾.
				lpNextTempAction = &(*NowAction)[NextOpNo];
				//�����A PUSH REG_S ��������.�œK������.
				if (lpNextTempAction->Nim == E_PUSH && lpNextTempAction->O1 == REG_S)
				{
					//lpNextTempAction�̂Ƃ���� �œK�������R�[�h�ɉ������܂�.
					if (lpTempAction->Nim == E_MOV)		lpNextTempAction->Nim = E_PUSH;
					else								lpNextTempAction->Nim = E_PUSHI;
					lpNextTempAction->O1 = lpTempAction->O2;

					(*NowAction).erase(lpTempAction);
					MaxActiones--;
				}
			}	// E_MOV E_MOVI �̔���� IF
		}		// 
	}
}

#endif //LIFE END
