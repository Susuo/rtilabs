#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_COMPLE
#define ___SCRIPTE_COMPLE

#include <.\Game\rti_Game_Scripte_Run.h>
#include <.\Game\rti_Game_Scripte_RevPorland.h>
#include <.\Game\rti_Game_Scripte_String.h>
#include <stack>

class TScripteComple
{
protected:
    //�R���p�C�����ߔ���
    void NewCommand(int Nim,int O1 = 0,int O2 = 0 ,int O3 = 0)
    {
    	struct Action A;
	    A.Nim = Nim;
    	A.O1 = O1;
	    A.O2 = O2;
	    A.O3 = O3;
        MAction->push_back(A);
    };
    ACTION_VECTOR *MAction;   //�R���p�C�����f���o�����߃R�[�h
    USE_STRING    *UseString; //������

    TScripteString      ScripteString;
    TScripteRevPorland  ScripteRevPorland;
    typedef std::stack<int> COMPLE_ANA_STACK;     //��͗p�̃X�^�b�N
    COMPLE_ANA_STACK        Comple_Ana_Stack;        //��͗p�̃X�^�b�N
    char **FunktionStr;                             //���[�U��`�̃t�@���N�V�����̖��O(������)
    char *CompleFilename;                           //���݃R���p�C�����Ă���t�@�C����
    int  FileLen;                                   //�t�@�C���̍s��

    //�R���p�C���̂��߂̃��[�`��.
    bool CompleDefine(char *next_str,ACTION_VECTOR *Ac);//#DEFINE���̉���.
    bool CompleElse(char *next_str);                    //#ELSE���̉���.
    bool CompleEndIf(char *next_str);                   //#ENDIF���̉���.
    bool CompleEndWhile(char *next_str);                //#ENDIF���̉���.
    bool CompleBreak(char *next_str);                   //#BREAK���̉���.
    bool CompleUserFunktion(char *buf);                 //���[�U��`���̉���.
    virtual bool UserOperation(char *str){return false;};      //���[�U�[��`
    bool FunktionSearch(char *buf);                            //���[�U�w��̃t�@���N�V�����T�[�`
    int  FunctionParaCals(char *str);
    bool AnalysisString(char* str);                            //���̕�������v�Z���@�y�сA�����̂ǂ��炩�œW�J.
	void Saitekika(ACTION_VECTOR *Ac);						   //�œK��.

public:
    TScripteComple(){FunktionStr = NULL;};
    //���̃t�@�C�����R���p�C�����ăf�[�^���i�[
    bool Comple(char *filename,ACTION_VECTOR *Ac,USE_STRING *Us);
    //�t�@���N�V�����o�^.
    void SetFunktion(char **Funk){ FunktionStr = Funk;};
};

#endif

#endif //LIFE END
