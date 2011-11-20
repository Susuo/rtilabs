/*****************************************************************/
/*****************************************************************/
//�G���[���b�Z�[�W
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef ERROR_MESSAGE_LIFE

#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_error_message.h>
#include <.\VCL\rti_vcl_apprication.h>

static int MsgOut(const char *Title,int Flg,char* Format,...);

int MsgOut(const char *Title,int Flg,char* Format,...)
{
	char Buffer[1024];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	return MessageBox(NULL,Buffer,Title,Flg);
}

void MsgError3(char* Format,...)
{
	char Buffer[512];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	MessageBox(NULL,Buffer,"�G���[���ɂ�[",MB_OK | MB_TASKMODAL | MB_ICONSTOP | MB_SETFOREGROUND	);
}

int MsgOr3(char* Format,...)
{
	char Buffer[512];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	return MessageBox(NULL,Buffer,"�m�F!!",MB_OKCANCEL | MB_TASKMODAL);
}

void FriendlyError(const char *FunktionName,const char *Inpor,const char *Reason,const char *Support)
{
    MsgError3("�ꏊ  : �֐� %s \n�s��  : %s�����s�����ɂ�[.\n���R  : %s\n�Ή�  : %s",FunktionName,Inpor,Reason,Support);
}

void FriendlyNameError(const char *FunktionName,const char *Name,const char *Reason,const char *Support)
{
    MsgError3("�ꏊ  : �֐� %s \n�s��  : %s�͂��������ɂ�[.\n���R  : %s\n�Ή�  : %s",FunktionName,Name,Reason,Support);
}

void FileNotOpen(const char *FunktionName,const char *Filename)
{
    MsgError3("�ꏊ  : �֐� %s \n�s��  : �t�@�C�� %s �I�[�v���Ɏ��s�����ɂ�[",FunktionName,Filename);
}

void MemoryDeficient(const char *FunktionName,const char *Inpor,int Size)
{
    MsgError3("�ꏊ  : �֐� %s \n�s��  : %s�����s�����ɂ�[.\n���R  : �������� %d �o�C�g�s�����Ă���Q�}\n�Ή�  : �������������Ɗm�ۂ��Ăق����ɂ�[",FunktionName,Inpor,Size);
}

void ProgramError(const char *FunktionName,const char *Inpor,const char *Reason)
{
    MsgError3("�ꏊ  : �֐� %s \n�s��  : %s�����s�����ɂ�[.\n���R  : %s\n�Ή�  : �v���O�����G���[�Ȃ̂ō�҂ɃN���[��������܂��傤(��)",FunktionName,Inpor,Reason);
}


#endif //LIFE END
