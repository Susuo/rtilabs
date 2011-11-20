#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_REVPORLAND
#define ___SCRIPTE_REVPORLAND

/***********************************************************/
//�e�L�X�g�ŗ^����ꂽ���̌v�Z
//  �y�с@�X�N���v�g���Ԍ���@�ϊ�.
/***********************************************************/
#include <.\Game\rti_Game_Scripte_Run.h>
#include <stack>

#define KACO_NEST_LEVEL 10  //���ʂ��l�X�g����x�ɑ�����D��x
    //�v�Z����ϊ����邽�߂Ɏg��.  �R�[�h (X - * / % �Ȃ�.)
    struct CalsCode
    {
        unsigned char Code;    //���Z�L��
        int O;                 //�D�揇��
        friend bool operator == (const CalsCode& A,const CalsCode& B){return A.O == B.O;};
        friend bool operator <  (const CalsCode& A,const CalsCode& B){return A.O < B.O;};
    };
    //�v�Z����ϊ����邽�߂Ɏg��.  �����@���W�X�^�Ȃ�.
    struct CalsData
    {
        bool Suji;             //�������A true �������琔�������.
        int O;                 //�f�[�^���e ����or���W�X�^�i���o�[
        friend bool operator == (const CalsData& A,const CalsData& B){return A.O == B.O;};
        friend bool operator <  (const CalsData& A,const CalsData& B){return A.O < B.O;};
    };
class TScripteRevPorland
{
    //�v�Z���̖���.
    enum 
    {
        P_EQU,P_ADD,P_SUB,P_MUL,P_DIV,P_MOD,P_SHL,P_SHR,P_AND,P_OR,P_XOR,
        P_A,P_B,P_AE,P_BE,P_NE,P_E,
        P_NOT,
    };
    //�R�[�h���x��.
    unsigned char *CodeLevel; 
    //�l�X�g�������ɂ�郌�x��
    int Kaco_Next_Add_Level;

    typedef std::stack<CalsCode> CALS_CODE_STACK;     //�L����p�X�^�b�N
    typedef std::stack<CalsData> CALS_DATA_STACK;     //�v�Z�f�[�^��p�X�^�b�N
    CALS_CODE_STACK        CalsCodeStack;       //�v�Z�L��
    CALS_DATA_STACK        CalsDataStack;       //�����������̓��W�X�^������X�^�b�N
    ACTION_VECTOR _MAction;

    //�R���p�C�����ߔ���
    void NewCommand(int Nim,int O1 = 0,int O2 = 0,int O3 = 0)
    {
    	struct Action A;
	    A.Nim = Nim;
    	A.O1 = O1;
	    A.O2 = O2;
	    A.O3 = O3;
        _MAction.push_back(A);
    };
    //�v�Z�L�����X�^�b�N�ɓo�^
    void  NewClasCode(unsigned char N,int O);
    //�v�Z�f�[�^���X�^�b�N�ɓo�^
    void  NewClasData(bool Suji,int O);
    //�v�Z�L�����X�^�b�N����擾
    bool  GetCommandCode(struct CalsCode* CC );
    //�v�Z�f�[�^���X�^�b�N����擾
    bool  GetCommandData(struct CalsData *CD);
    //�����ɕϊ�.
    int SujiGet(char* &s);
    //���l�v�Z
    void ClasProc(int i1,int Code,int i2);
    //���̌v�Z���߃R�[�h�����ۂ̖��߃R�[�h�ɕϊ�
    unsigned char GetCode(int Op);
    //���W�X�^�[�����ތv�Z.
    void RegisterProc(struct CalsData *A,int Code,struct CalsData *B);
    //�v�Z����
    bool StackClasProc();
    //���Z�q�Ȃǂ̃`�F�b�N
    void CodeCheck(int Code);
    //���W�X�^�[���擾
    void Register(char* &s);
    //���݂̃A���S���Y���ł́A�Ō�ɂ悯���� push ������̂ł�����폜.
    //�܂��A���łɁ@���ʂȃX�^�b�N PUSh POP �����폜���čœK�����s�� (�Ղ�
    void PushKiller(int StartMActionNumber);
    //�t�|�[�����h�@�̃��C��
    void RevPorlandMain(char *Target);
public:
    TScripteRevPorland();
    //�t�|�[�����h�@�����
    bool RevPorland(char *Target,ACTION_VECTOR &MasterAction);
};

#endif
#endif //LIFE END
