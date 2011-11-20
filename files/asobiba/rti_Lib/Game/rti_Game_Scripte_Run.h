#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_RUN
#define ___SCRIPTE_RUN

//STL �ō��`
#include <stack>
#include <vector>
#include <string>
#include <memory.h>
//�����ϐ����Ȃǂւ̌x����ق点��. STL���g���Ƃł�A���ł�.
#pragma warning(disable: 4786)

//�X�N���v�g����@���Ԍ���@�́@���ߏ���. 8086 ���@�ɋ߂��ł� (^^;
struct Action
{
    unsigned char Nim;
    int O1;
    int O2;
    int O3;

    friend bool operator == (const Action& A,const Action& B){return A.Nim == A.Nim;};
    friend bool operator <  (const Action& A,const Action& B){return A.Nim < A.Nim;};
};

//�X�N���v�g���Ԍ���̖���.
enum Operation
{
    E_MOV,E_MOVI,E_MOVII,E_ADD,E_ADDI,E_ADDII,E_SUB,E_SUBI,E_SUBII,
    E_MUL,E_MULI,E_MULII,E_DIV,E_DIVI,E_DIVII,E_MOD,E_MODI,E_MODII,
    E_SHL,E_SHLI,E_SHLII,E_SHR,E_SHRI,E_SHRII,E_AND,E_ANDI,E_ANDII,
    E_OR,E_ORI,E_ORII,   E_XOR,E_XORI,E_XORII,
    E_A,E_AI,E_AII,E_B,E_BI,E_BII,
    E_AE,E_AEI,E_AEII,E_BE,E_BEI,E_BEII,
    E_NE,E_NEI,E_NEII,E_E,E_EI,E_EII,
    E_NOT,E_NOTI,E_NOTII,
    E_PUSH,E_PUSHI,E_POP,E_POPI,
    E_JMP,E_IF,E_HAL,E_FUNK,
};
typedef std::vector<Action> ACTION_VECTOR;
typedef std::stack<int> ACTION_STACK;
typedef std::vector<std::string>    USE_STRING;

class TScripeRunArea;

typedef int (*USER_FUNKTION)(TScripeRunArea *SC,int o1,int o2,int o3,int o4,int o5);
#define USER_FUNKTION_MAX 5    //���[�U����`�ł���֐��̈����̍ő�l.
//���ʃ��W�X�^.
#define REG_S      (5)
#define REG_S_EX   (6)
#define ACTION_MAX  256

//Run �̖߂�l�ɂ���.
#define RET_RESERVED_WORD_RUN   0   //�\�������s����,
#define RET_END_WORD_RUN   -1       //�I���R�[�h�A�y�і��߂��Ō�܂Ŏ��s����.
#define RET_DISHONEST_WORD_RUN   -10//�s���Ȗ��߂����s���܂���. ���Z�b�g���Ă������� (��

class TScripteComple;
class TScripteRun
{
    //�X�N���v�g�f�[�^.
    ACTION_VECTOR MAction[ACTION_MAX];
    //������f�[�^
    USE_STRING UseString;

    bool FunktionSearch(char *buf);
    int  FunctionParaCals(char *str);
	//UserFunk �̕ۑ�
	void UserFunKeep(USER_FUNKTION *Funk,char **StrCommand);
    //���[�U��`�̖���
    virtual int UserOperation(Action *A,TScripeRunArea *SRA){return RET_DISHONEST_WORD_RUN;};
    USER_FUNKTION *UserFunk;
public:
	TScripteRun() : UserFunk(NULL) {};
	~TScripteRun(){delete UserFunk;};
    //���̃t�@�C�����R���p�C�����ăf�[�^���i�[
    bool Comple(char *filename,USER_FUNKTION *Funk = NULL,char **Str = NULL);
    //���̃t�@�C�����R���p�C�����ăf�[�^���i�[
    bool Comple(char *filename,TScripteComple *SC,
                USER_FUNKTION *Funk = NULL,char **Str = NULL);
    //�폜.
    void Breaker(int c){MAction[c].clear();};
    //���ׂăN���A
    void All_Breaker(){for(int i = 0 ; i < ACTION_MAX ;i ++ )Breaker(i);};
    //Run
    int __fastcall Run(int c,TScripeRunArea *SRA);
    //�}�b�s���O���ꂽ�ꏊ����A����������߂�
    const char* GetString(int MappingNo){return (UseString[MappingNo]).c_str();};
    //�}�b�s���O���ꂽ�ꏊ����A����������߂�
    std::string GetStlString(int MappingNo){return (UseString[MappingNo]);};
    //Debug
    void Debug(int c);
};

//�X�N���v�g���s�G���A.
class TScripeRunArea
{
    //TScripteRun �N���X(���߃f�R�[�_�����)����A���̃N���X�̃��W�X�^�A
    //�X�^�b�N�Ɏ��R�ɃA�N�Z�X���ł���悤�ɂ��܂�. (^^;;;;
    friend class TScripteRun;
protected:
    int IP;                     //���ݎ��s���Ă���A�h���X(?)
    int R[7];                   //���W�X�^
	int RunNumber;			    //���s����X�N���v�g
    ACTION_STACK ActionStack;   //�X�^�b�N
public:
    TScripeRunArea(){};
    void ClearIP(){IP = 0;};
    void ClearR(){memset(R,0,sizeof(int)*7);};
    void ClearStack(){/*ActionStack.clear();*/};
    void ClearAll(){ClearIP(); ClearR(); ClearStack(); };
	//�V�K���ߎ��s�J�n.
	void NewScripteRun(int No){ ClearAll(); RunNumber = No; };
};

#endif
#endif //LIFE END

