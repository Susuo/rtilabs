#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <.\Game\rti_Game_Scripte_RevPorland.h>
#include <stdio.h>

/*****************************************************/
//�v�Z�L�����X�^�b�N�ɓo�^
/*****************************************************/
void  TScripteRevPorland::NewClasCode(unsigned char N,int O)
{
	struct CalsCode A;
	A.Code = N;	A.O = O;
    CalsCodeStack.push(A);
}
/*****************************************************/
//�v�Z�f�[�^���X�^�b�N�ɓo�^
/*****************************************************/
void  TScripteRevPorland::NewClasData(bool Suji,int O)
{
	struct CalsData A;
	A.Suji = Suji;	A.O = O;
    CalsDataStack.push(A);
}

/*****************************************************/
//�v�Z�L�����X�^�b�N����擾
/*****************************************************/
bool TScripteRevPorland::GetCommandCode(struct CalsCode* CC )
{
    if (CalsCodeStack.size() <= 0) return false; //�X�^�b�N����ۂ̗�O

    *CC = CalsCodeStack.top();    CalsCodeStack.pop();
    return true;
}

/*****************************************************/
//�v�Z�f�[�^���X�^�b�N����擾
/*****************************************************/
bool TScripteRevPorland::GetCommandData(struct CalsData* CD)
{
    if (CalsDataStack.size() <= 0) return false; //�X�^�b�N����ۂ̗�O

    *CD = CalsDataStack.top();    CalsDataStack.pop();
    return true;
}
/*****************************************************/
//�����ɕϊ�.
/*****************************************************/
int TScripteRevPorland::SujiGet(char* &s)
{
    int ret = 0;            //�Ԃ茌
    int zero = 1000000;     //�Ƃ肠�����A�S�� (^^; �����͕����񂾂ƁA�t�Ɋi�[����Ă���̂ł˂�
    //������̒������擾.
    for(; *s >= '0' && *s <= '9' && zero >=0; s++)
    {
        ret += (*s - '0' ) * zero;
        zero /= 10; //�ʂ��������.
    }
    // s ����߂�(�����̍Ō�̕��� 1234 �������� 4 �Ƀ|�C���^�����킹��.)
    s --;
   if (zero == 0) return ret; // zero �� 0 �ɂȂ�悤�Ȓl������.
   //��������l�̃J�b�g�Ƃ��ď�����.
    return ret / (zero*10);
}

/*****************************************************/
//���l�v�Z
/*****************************************************/
void TScripteRevPorland::ClasProc(int i1,int Code,int i2)
{
        switch(Code)
        {
        case P_EQU:
            printf("%d",i2);
            break;
        case P_ADD:
            NewClasData(true,i1 + i2);   break;
        case P_SUB:
            NewClasData(true,i1 - i2);   break;
        case P_MUL:
            NewClasData(true,i1 * i2);   break;
        case P_DIV:
            NewClasData(true,i1 / i2);   break;
        case P_MOD:
            NewClasData(true,i1 % i2);   break;
        case P_SHL:
            NewClasData(true,i1 << i2);   break;
        case P_SHR:
            NewClasData(true,i1 >> i2);   break;
        case P_AND:
            NewClasData(true,i1 & i2);   break;
        case P_OR:
            NewClasData(true,i1 | i2);   break;
        case P_XOR:
            NewClasData(true,i1 ^ i2);   break;
        case P_A:
            NewClasData(true,i1 < i2);   break;
        case P_B:
            NewClasData(true,i1 > i2);   break;
        case P_AE:
            NewClasData(true,i1 <= i2);   break;
        case P_BE:
            NewClasData(true,i1 >= i2);   break;
        case P_NE:
            NewClasData(true,i1 != i2);   break;
        case P_E:
            NewClasData(true,i1 == i2);   break;
        case P_NOT:
            break;
        }
}

/*****************************************************/
//���̌v�Z���߃R�[�h�����ۂ̖��߃R�[�h�ɕϊ�
/*****************************************************/
unsigned char TScripteRevPorland::GetCode(int Op)
{
        switch(Op)
        {
        case P_EQU:            return E_MOV;
        case P_ADD:            return E_ADD;
        case P_SUB:            return E_SUB;
        case P_MUL:            return E_MUL;
        case P_DIV:            return E_DIV;
        case P_MOD:            return E_MOD;
        case P_SHL:            return E_SHL;
        case P_SHR:            return E_SHR;
        case P_AND:            return E_AND;
        case P_OR:             return E_OR;
        case P_XOR:            return E_XOR;
        case P_A:              return E_A;
        case P_B:              return E_B;
        case P_AE:             return E_AE;
        case P_BE:             return E_BE;
        case P_NE:             return E_NE;
        case P_E:              return E_E;
        case P_NOT:
            break;
        }
        printf("�R���p�C���s�\�ȃ��W�X�^�̖��߂���������܂���.");
        throw 0;    //��O����. RevPorlandMain�܂Ő�����΂��܂� (^^;;

        return 0xff;
}

/*****************************************************/
//���W�X�^�[�����ތv�Z.
/*****************************************************/
void TScripteRevPorland::RegisterProc(struct CalsData *A,int Code,struct CalsData *B)
{
    //���Z�q�����߂�.
    unsigned char Op  = GetCode(Code);

    //      Reg , Reg +0
    //      Reg , 1   +1
    //      1   , Reg +2
    // �̋K��ɂ��A�@�����@���W�X�^�̊֌W�ŉ��Z���߂̏C��.
    //�Ȃ��A  1 , 1 �̏ꍇ�́A�����������̃��[�`���ɂ��Ȃ�.
    //�����āA�@���W�X�^���܂܂�鉉�Z�݂̂������ł���Ă��邩���. (^^;
    if (A->Suji == false && B->Suji == true  ) Op +=1;//Reg , 1   +1
    if (A->Suji == true  && B->Suji == false ) Op +=2;//1   , Reg +2

    // S ���W�X�^��K�v�Ƃ���v�Z.
    if (B->Suji == false && B->O == REG_S)
    {
        //�v�Z���ʂ� S ���W�X�^�̕ی�.
        NewCommand(E_POP,REG_S_EX); //  �� 6 ���W�X�^��.
    }
    // S ���W�X�^��K�v�Ƃ���v�Z.
    if (A->Suji == false && A->O == REG_S)
    {
        //�v�Z���ʂ� S ���W�X�^�̕ی�.
        NewCommand(E_POP,REG_S); //  �� 5 ���W�X�^��.
    }

    //�V�K���ߔ���.
    NewCommand(Op,A->O,B->O);
    //�v�Z���ʂ� S ���W�X�^�̕ی�.
    NewCommand(E_PUSH,REG_S);
    //�t�|�[�����h�@���������邽�߂ɁA�X�^�b�N�Ɍv�Z�R�[�h������.
    NewClasData(false,REG_S);
}


//�v�Z����
bool TScripteRevPorland::StackClasProc()
{
    struct CalsData A;
    struct CalsData B;
    struct CalsCode Code;

	if ( ! GetCommandData(&B) )	return false;	//�X�^�b�N����.
	if ( ! GetCommandData(&A) )
	{
		// A ���������A B �͂������ꍇ.
		//���̎��́A Abs(10) �� 10 �̂Ƃ��̂悤�Ȃ��Ƃł���Ƃ�����.
		//�䂦�ɁA B �̒l�� REG_S ���W�X�^�Ɋi�[.
		if (B.Suji)	NewCommand(E_MOVI,REG_S,B.O);	//��������
		else		NewCommand(E_MOV,REG_S,B.O);	//���W�X�^����.
 		return false;
	}
	if ( ! GetCommandCode(&Code))  return false;	//���Z�q�X�^�b�N����.
    // A ���Z B ������.
    //printf("\n%d  %d  %d\n",A.O,Code.Code,B.O);

    if (A.Suji && B.Suji)
    {   //�������m��������v�Z�ł���̂Ōv�Z
        ClasProc(A.O,Code.Code,B.O);
    }
    else
    {   //���W�X�^�[�̂���ތv�Z.
        RegisterProc(&A,Code.Code,&B);
    }
        
    return true;
}


/*****************************************************/
//���Z�q�Ȃǂ̃`�F�b�N
/*****************************************************/
void TScripteRevPorland::CodeCheck(int Code)
{
    struct CalsCode A;
    //���݂̃R�[�h�̃��x�� 
    int NowCodeLevel = CodeLevel[Code] + Kaco_Next_Add_Level;
    while(1)
    {
        if (CalsCodeStack.size() <= 0) break; //�X�^�b�N�����
        A = CalsCodeStack.top();              //�O��̉��Z�q���擾.
        if (A.O < NowCodeLevel) break;

        //���݂�荂���x���̉��Z�q�������珈��.
        if ( ! StackClasProc() ) break;
    }
    //�v�Z�L�����X�^�b�N�ɓo�^
    NewClasCode(Code,NowCodeLevel);
}

/*****************************************************/
//���W�X�^�[���擾
/*****************************************************/
void TScripteRevPorland::Register(char* &s)
{
    s++;
    if (*s >= '0' && *s <= '6')
    {
        NewClasData(false,(*s - '0') );
    }
    else
    {
        printf("�����������W�X�^�[�w��ł�. %d",*s - '0');
        throw 0;    //��O����. RevPorlandMain�܂Ő�����΂��܂� (^^;;
    }
}

/*****************************************************/
//���݂̃A���S���Y���ł́A�Ō�ɂ悯���� push ������̂ł�����폜.
//�܂��A���łɁ@���ʂȃX�^�b�N PUSh POP �����폜���čœK�����s�� (�Ղ�
/*****************************************************/
void TScripteRevPorland::PushKiller(int StartMActionNumber)
{
    enum _YokeinaPushPop{ YOKEI_NONE , YOKEI_POP};
    int IP;                         //���݂̖��߂̃A�h���X.
    _YokeinaPushPop Yokei5,Yokei6;  //�悯���� PUSH POP �폜�̃t���O

    bool LastPushKill = false; //�Ō�� Push ���폜�����Ƃ����t���O
    IP = _MAction.size() - 1;   //���[�v�����.
    Yokei5 = Yokei6 = YOKEI_NONE;   //�悯���� PUSH POP �폜�̃t���O��������.
    //�œK�����[�v. (�z����t���ǂ��Ă����܂�.)
    for( ; StartMActionNumber <= IP; IP--)
    {
        switch(_MAction[IP].Nim)
        {
		case E_MOV:
			if ( _MAction[IP].O1 == _MAction[IP].O2)
			{
                _MAction.erase(&_MAction[IP]);
			}
            Yokei5 = Yokei6 = YOKEI_NONE; break;
        case E_POP:
            switch(_MAction[IP].O1)
            {
            case REG_S: //5�ԃ��W�X�^
                Yokei5 = YOKEI_POP; break;
            case REG_S_EX: //6�ԃ��W�X�^
                Yokei6 = YOKEI_POP; break;
            default:
                Yokei5 = Yokei6 = YOKEI_NONE; break;
            }
        case E_PUSH:
            if (! LastPushKill)
            {
                //�Ō�� push ����������.
                _MAction.erase(&_MAction[IP]);
                LastPushKill = true;    //�폜���F.
            }
            else
            {
                if (Yokei5 == YOKEI_POP || Yokei6 == YOKEI_POP)
                {   //PUSH �������̃X�e�b�v�� POP ���Ă���̂Ŗ��ʂƔ��f.
                    _MAction.erase(&_MAction[IP]);        //����. PUSH
                    _MAction.erase(&_MAction[IP-1]);        // POP
                }
            }
        default:
            Yokei5 = Yokei6 = YOKEI_NONE; break;
        }
    }
}

/*****************************************************/
//�t�|�[�����h�@�̃��C��
/*****************************************************/
void TScripteRevPorland::RevPorlandMain(char *Target)
{
    char *s = Target;
    int SujiBuf;

    //���ʃl�X�g�ő�����D��x�̏�����
    Kaco_Next_Add_Level = 0;
    int StartMActionNumber;  //�R���p�C�����f���o�����߃R�[�h�̍ŏ��̖��߂̏ꏊ
    //�ŏ��̖��߂̏ꏊ���L�^���Ă����܂�.
    //���ƂŁA�œK������Ƃ��Ɏg�p�����肵�܂�. ���ւ�
    StartMActionNumber = 0;
    //���X�g�N���A.
    _MAction.clear();
    //��̓��[�v
    while(*s)
    {   
        switch(*s )
        {
        case '=':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_E); s++; } 
            else CodeCheck(P_EQU);  //=
            break;
        case '+':            CodeCheck(P_ADD);            break;
        case '-':            CodeCheck(P_SUB);            break;
        case '*':            CodeCheck(P_MUL);            break;
        case '/':            CodeCheck(P_DIV);            break;
        case '%':            CodeCheck(P_MOD);            break;
        case '>':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_AE); s++; } 
            else if (*(s+1) == '>' ) //>>
            { CodeCheck(P_SHR); s++;}
            else CodeCheck(P_A); 
            break;
        case '<':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_BE); s++; } 
            else if (*(s+1) == '<' ) //<<
            { CodeCheck(P_SHL); s++;}
            else CodeCheck(P_B); 
            break;
        case '&':            CodeCheck(P_AND);            break;
        case '|':            CodeCheck(P_OR);             break;
        case '!':            CodeCheck(P_NOT);            break;
        case '~':            CodeCheck(P_XOR);            break;
        case '(':            Kaco_Next_Add_Level += KACO_NEST_LEVEL;           break;
        case ')':            Kaco_Next_Add_Level -= KACO_NEST_LEVEL;           break;
        case 'R':        case 'r':            Register(s);                 break;
        case '0':        case '1':        case '2':        case '3':
        case '4':        case '5':        case '6':        case '7':
        case '8':        case '9':
            //����.
            SujiBuf = SujiGet(s);
            NewClasData(true,SujiBuf);        break;
        }
        //�������i�߂�.
        s ++;
    }
    //�X�^�b�N����ɂȂ�܂ŏ������܂�. �ォ�牟���Ă����݂����Ȋ����ł���.
    while( StackClasProc( ) );
    //���݂̃A���S���Y���ł́A�Ō�ɂ悯���� push ������̂ł�����폜.
    //�܂��A�œK�����s��.
    PushKiller(StartMActionNumber);
}

/*****************************************************/
//�t�|�[�����h�@�����
/*****************************************************/
bool TScripteRevPorland::RevPorland(char *Target,ACTION_VECTOR &MasterAction)
{

    try
    {
        RevPorlandMain(Target);
    }
    catch(...)
    {
        printf("��̓��[�`���̒��ŃG���[���������܂�. �ւ�");
        return false;
    }
    //�A��.
    for(int i = 0; i < _MAction.size() ; i++ )
    {
        MasterAction.push_back( _MAction[i] );
    }
    
    return true;
}

/*****************************************************/
//�R���X�g���N�^
/*****************************************************/
TScripteRevPorland::TScripteRevPorland()
{
    //�R�[�h���x��.
    static unsigned char _CodeLevel[] = 
    {
        1,  //EQU
        5,  //ADD
        5,  //SUB
        7,  //MUL
        7,  //DIV
        7,  //MOD
        3,  //SHL
        3,  //SHR
        3,  //AND
        3,  //OR
        0,  //A
        0,  //B
        0,  //AE
        0,  //BE
        0,  //NE
        0,  //E
        3,  //NOT
    };
    CodeLevel = _CodeLevel ;
}
#endif //LIFE END
